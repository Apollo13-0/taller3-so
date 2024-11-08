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
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(A[i], N, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        // El proceso 0 también trabaja en su propia fila (la primera fila)
        for (int j = 0; j < N; j++) {
            local_row[j] = A[0][j];
        }
    } else {
        // Los procesos reciben su fila de A
        MPI_Recv(local_row, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Cada proceso realiza la multiplicación de la fila local por la matriz B
    for (int j = 0; j < N; j++) {
        result_row[j] = 0;
        for (int k = 0; k < N; k++) {
            result_row[j] += local_row[k] * B[k][j];
        }
    }

    // Recoger los resultados en la matriz C en el proceso raíz
    if (rank == 0) {
        // El proceso 0 almacena su resultado en la primera fila de C
        for (int j = 0; j < N; j++) {
            C[0][j] = result_row[j];
        }

        // Recibir las filas calculadas por otros procesos
        for (int i = 1; i < size; i++) {
            MPI_Recv(C[i], N, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Imprimir el resultado final
        printf("\nMatriz Resultante C (AxB):\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }
    } else {
        // Los procesos no raíz envían su fila calculada de C al proceso 0
        MPI_Send(result_row, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
