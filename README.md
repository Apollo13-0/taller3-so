Aquí tienes el README completo que puedes copiar y pegar directamente en GitHub:

```markdown
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

## Licencia

Este proyecto está bajo la Licencia MIT. Ver el archivo LICENSE para más detalles.

```

Este README cubre todos los aspectos importantes del proyecto, incluyendo instrucciones de compilación y ejecución, así como una explicación detallada del código. Simplemente cópialo y pégalo en tu repositorio de GitHub.
