#ifndef ARRAY1D_H
#define ARRAY1D_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


/** 
 * @brief Estructura para representar un arreglo 1D
 */ 
typedef struct {
    double* data;   /**< Arreglo plano con los datos */
    size_t  ndim;   /**< Número de dimensiones. Debe ser 1 */
    size_t  n;      /**< Número de elementos */
} Array1d;

// Reserva memoria para un arreglo de tamaño n
Array1d *array1d_alloc(size_t n);

// Crea un arreglo 1D a partir de la información en un archivo binario
Array1d *readArray1d(const char *cfullname);

// Escribe las entradas de un arreglo en un archivo binario
int writeArray1d(Array1d *array, const char *cfullname);

// Escribe las entradas de un arreglo en un archivo binario
int writeArray1d(Array1d *array, const char *cfullname);

// Libera la memoria del arreglo unidimensional
void freeArray1d(Array1d *array);

// Imprime los elementos de un vector usando el formato especificado
// en la cadena format.
void printArray1d(Array1d *array, const char *format, int nshow);


#endif

