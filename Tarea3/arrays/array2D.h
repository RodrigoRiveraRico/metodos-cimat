#ifndef ARRAY2D_H
#define ARRAY2D_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/** 
 * @brief Estructura para representar un arreglo 2D
 */
typedef struct {
    double **data;   /**< Arreglo plano con los datos */
    size_t   ndim;   /**< Número de dimensiones. Debe ser 2 */
    size_t   rows;   /**< Número de filas */
    size_t   cols;   /**< Número de columnas */
} Array2d;


// Reserva memoria para una arreglo 2D de tamaño nr x nc
Array2d *array2d_alloc(int nr, int nc);

// Lectura de las entradas de un arreglos 2D almacenadas en un archivo binario.
Array2d *readArray2d(const char *cfile);

// Almacena las entradas del arreglo 2D en un archivo binario.
int writeArray2d(Array2d *array, const char *cfullname);

// Libera la memoria del arreglo bidimensional
void freeArray2d(Array2d *array);

// Imprime la fila i-esima del arreglo 
void printRow(double **data, int i, int nc, const char *format, int nshow);

// Imprime en la consola las entradas del arraglo 2D
void printArray2d(Array2d *array, const char *format, int nshow);

#endif

