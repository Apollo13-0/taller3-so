#include <stdio.h>
#include <mpi.h>

#define N 4 // Dimensión de la matriz (4x4)

int main(int argc, char **argv) {
    int rank, size;
    int A[N][N], B[N][N], C[N][N];
    int local_row[N]; // Fila local para cada proceso
    int result_row[N]; // Resultado de la fila local

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Asegurarse de que el número de procesos sea igual a N
    if (size != N) {
        if (rank == 0) {
            printf("El número de procesos debe ser igual a %d\n", N);
        }
        MPI_Finalize();
        return 0;
    }

    // Inicializar las matrices en el proceso raíz
    if (rank == 0) {
        printf("Matriz A:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] = i + j + 1; // Ejemplo de inicialización
                printf("%d ", A[i][j]);
            }
            printf("\n");
        }

        printf("\nMatriz B:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                B[i][j] = i - j + 1; // Ejemplo de inicialización
                printf("%d ", B[i][j]);
            }
            printf("\n");
        }
    }

    // Enviar la matriz B a todos los procesos
    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Enviar una fila de la matriz A a cada proceso
    MPI_Scatter(A, N, MPI_INT, local_row, N, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada proceso realiza la multiplicación de la fila local por la matriz B
    for (int j = 0; j < N; j++) {
        result_row[j] = 0;
        for (int k = 0; k < N; k++) {
            result_row[j] += local_row[k] * B[k][j];
        }
    }

    // Recoger los resultados en la matriz C en el proceso raíz
    MPI_Gather(result_row, N, MPI_INT, C, N, MPI_INT, 0, MPI_COMM_WORLD);

    // Imprimir la matriz resultante en el proceso raíz
    if (rank == 0) {
        printf("\nMatriz Resultante C (AxB):\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}
