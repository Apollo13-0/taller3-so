Este repositorio contiene tres programas en C: `matmul.c`, `sobel.c` y `sobelMPI.c`, los cuales utilizan procesamiento paralelo con MPI (Message Passing Interface) y procesamiento de imágenes. A continuación se describen las instrucciones para compilar y ejecutar cada uno de estos programas.

## Requisitos

1. **MPI**: Asegúrate de tener instalada una implementación de MPI como OpenMPI o MPICH.
2. **libjpeg**: Es necesario tener la librería `libjpeg` instalada para leer y escribir archivos JPEG. En sistemas basados en Debian/Ubuntu, puedes instalarla con:
   ```bash
   sudo apt-get install libjpeg-dev
   ```

## Archivos

### 1. `matmul.c` - Multiplicación de matrices con MPI

Este programa realiza la multiplicación de dos matrices de tamaño 4x4 usando procesamiento paralelo con MPI.

#### Compilación
Para compilar el archivo `matmul.c` con MPI, utiliza el siguiente comando:
```bash
mpicc -o matmul matmul.c
```

#### Ejecución
Para ejecutar el programa con 4 procesos (ajusta el número de procesos según tu sistema):
```bash
mpirun -np 4 --hostfile hosts ./matmul
```

### 2. `sobel.c` - Filtro Sobel en una imagen JPEG

Este programa aplica un filtro Sobel para detección de bordes en una imagen JPEG. Convierte la imagen a escala de grises antes de aplicar el filtro.

#### Compilación
Para compilar el archivo `sobel.c`:
```bash
gcc -o sobel sobel.c -ljpeg -lm
```

#### Ejecución
Para ejecutar el programa, pasa como parámetros el archivo de entrada (imagen JPEG) y el archivo de salida:
```bash
./sobel imagen_entrada.jpg imagen_salida.jpg
```

### 3. `sobelMPI.c` - Filtro Sobel en una imagen JPEG utilizando MPI

Este programa aplica el filtro Sobel de manera distribuida sobre una imagen JPEG utilizando MPI. Divide el trabajo de procesamiento de la imagen entre los procesos disponibles.

#### Compilación
Para compilar el archivo `sobelMPI.c`:
```bash
mpicc -o sobelMPI sobelMPI.c -ljpeg -lm
```

#### Ejecución
Para ejecutar el programa, usa el siguiente comando con el número de procesos adecuado:
```bash
mpirun -np 4 --hostfile hosts ./sobelMPI
```
Este comando asume que la imagen JPEG llamada `cat.jpeg` está disponible en el directorio actual. El resultado se guarda en un archivo de salida en formato RAW.

# Filtro Sobel con OpenMP

Este proyecto aplica el filtro Sobel a una imagen en formato JPEG utilizando OpenMP para la paralelización del procesamiento. El filtro Sobel es utilizado comúnmente en la detección de bordes en imágenes, y este código está optimizado para ser ejecutado en sistemas con soporte para paralelización mediante múltiples hilos.

## Requisitos

- GCC (GNU Compiler Collection)
- OpenMP
- libjpeg (para la manipulación de imágenes JPEG)

## Instrucciones de uso

### 1. Compilar el código

Para compilar el programa, utiliza el siguiente comando:

```bash
gcc -o sobelOpenMp sobelOpenMp.c -lm -ljpeg -fopenmp
```

- `-lm` es para enlazar con la librería matemática (utilizada por `sqrt` y otras funciones matemáticas).
- `-ljpeg` es para enlazar con la librería `libjpeg`, que se utiliza para leer y escribir imágenes JPEG.
- `-fopenmp` habilita el soporte de OpenMP para la paralelización.

### 2. Ejecutar el programa

Una vez que el código esté compilado, puedes ejecutar el programa con la siguiente sintaxis:

```bash
./sobelOpenMp <imagen_entrada.jpg> <imagen_salida.jpg>
```

- `<imagen_entrada.jpg>` es el archivo JPEG de entrada.
- `<imagen_salida.jpg>` es el archivo donde se guardará la imagen después de aplicar el filtro Sobel.

## Descripción del código

### Funciones principales

1. **`read_JPEG_file`**: Lee una imagen JPEG y devuelve los datos en un arreglo de píxeles.
2. **`write_JPEG_file`**: Guarda la imagen procesada como un archivo JPEG.
3. **`convert_to_grayscale`**: Convierte la imagen a escala de grises si no está ya en este formato.
4. **`sobel_filter`**: Aplica el filtro Sobel a la imagen, paralelizando el procesamiento usando OpenMP.
5. **`main`**: Controla la ejecución del programa, gestionando los argumentos y llamando a las funciones anteriores.

### Detalles del filtro Sobel

El filtro Sobel utiliza dos matrices de convolución (`Gx` y `Gy`) para calcular los gradientes en las direcciones horizontal y vertical. La magnitud del gradiente es calculada para cada píxel en la imagen y se utiliza para resaltar los bordes.

### Uso de OpenMP

El filtro Sobel se paraleliza utilizando OpenMP para acelerar el procesamiento de la imagen. La directiva `#pragma omp parallel for` permite que cada píxel se procese en paralelo, aprovechando múltiples núcleos del procesador.

## Ejemplo

Para aplicar el filtro Sobel a una imagen llamada `input.jpg` y guardar el resultado en `output.jpg`, ejecuta:

```bash
./sobelOpenMp input.jpg output.jpg
```

