#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <jpeglib.h>
#include <mpi.h>

#define MAX_WIDTH 1024 // Tamaño máximo de la imagen, puedes ajustarlo según tu necesidad
#define MAX_HEIGHT 1024

// Máscara Sobel para la detección de bordes
int sobelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
int sobelY[3][3] = {{-1, -2, -1}, { 0,  0,  0}, { 1,  2,  1}};

// Función para aplicar el filtro Sobel
void apply_sobel(int *image, int *result, int width, int height, int start_row, int end_row) {
    int i, j, x, y;
    int gx, gy, g;

    for (i = start_row; i < end_row; i++) {
        for (j = 1; j < width - 1; j++) {
            gx = 0;
            gy = 0;
            // Aplicar las máscaras Sobel en 3x3
            for (x = -1; x <= 1; x++) {
                for (y = -1; y <= 1; y++) {
                    int pixel = image[(i + x) * width + (j + y)];
                    gx += pixel * sobelX[x + 1][y + 1];
                    gy += pixel * sobelY[x + 1][y + 1];
                }
            }
            g = (int)(sqrt(gx * gx + gy * gy));
            if (g > 255) g = 255;
            result[i * width + j] = g;
        }
    }
}

// Función para leer una imagen JPG usando libjpeg
void read_jpeg(const char *filename, int **image, int *width, int *height) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *infile;

    // Abrir el archivo JPEG
    if ((infile = fopen(filename, "rb")) == NULL) {
        fprintf(stderr, "No se pudo abrir el archivo %s\n", filename);
        exit(1);
    }

    // Configurar el descriptor de error y la estructura de compresión
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    // Inicializar el archivo JPEG
    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    // Obtener las dimensiones de la imagen
    *width = cinfo.output_width;
    *height = cinfo.output_height;

    // Asignar memoria para la imagen
    *image = (int *)malloc(*width * *height * sizeof(int));

    // Leer la imagen línea por línea
    unsigned char *row_buffer = (unsigned char *)malloc(cinfo.output_width * cinfo.output_components);
    int row_stride = cinfo.output_width * cinfo.output_components;
    int i = 0;

    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, &row_buffer, 1);
        for (int j = 0; j < *width; j++) {
            int r = row_buffer[j * 3];
            int g = row_buffer[j * 3 + 1];
            int b = row_buffer[j * 3 + 2];
            // Convertir RGB a escala de grises
            (*image)[i * (*width) + j] = (r + g + b) / 3;
        }
        i++;
    }

    // Liberar memoria y cerrar el archivo
    free(row_buffer);
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
}

int main(int argc, char *argv[]) {
    int rank, size, i, j;
    int *image, *result;
    int width, height;
    int rows_per_process, start_row, end_row;
    int *local_image, *local_result;

    // Inicializar MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Cargar la imagen JPEG
    if (rank == 0) {
        read_jpeg("cat.jpeg", &image, &width, &height);
        result = (int *)malloc(width * height * sizeof(int));
    }

    // Distribuir las filas entre los procesos
    rows_per_process = height / size;
    start_row = rank * rows_per_process;
    end_row = (rank == size - 1) ? height : start_row + rows_per_process;

    local_image = (int *)malloc(width * rows_per_process * sizeof(int));
    local_result = (int *)malloc(width * rows_per_process * sizeof(int));

    // Enviar las filas de la imagen a cada proceso
    MPI_Scatter(image, width * rows_per_process, MPI_INT, local_image, width * rows_per_process, MPI_INT, 0, MPI_COMM_WORLD);

    // Aplicar el filtro Sobel en las filas asignadas a cada proceso
    apply_sobel(local_image, local_result, width, height, start_row, end_row);

    // Recoger los resultados de todos los procesos
    MPI_Gather(local_result, width * rows_per_process, MPI_INT, result, width * rows_per_process, MPI_INT, 0, MPI_COMM_WORLD);

    // Guardar la imagen resultante (como RAW por simplicidad)
    if (rank == 0) {
        FILE *output_file = fopen("output_image.raw", "wb");
        fwrite(result, sizeof(int), width * height, output_file);
        fclose(output_file);

        free(image);
        free(result);
    }

    free(local_image);
    free(local_result);

    MPI_Finalize();
    return 0;
}
