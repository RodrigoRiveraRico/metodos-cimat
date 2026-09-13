#ifndef SOL_ECU_LIN_H
#define SOL_ECU_LIN_H

#include "../arrays/array1D.h"
#include "../arrays/array2D.h"

/** 
 * @brief Método de sustitución hacia adelante para sistemas Lx = b
 * 
 * El método trabaja con una matriz cuadrada triangular inferior L.
 * 
 * @param L Apuntador simple a una estructura de matriz (2D)
 * @param b Apuntador simple a una estructura de arreglo (1D)
 * @param tol Tolerancia
 * 
 * @return Apuntador simple a una estructura de arreglo (1D)
 * @retval NULL si no hay memoria suficiente para guardar el vector solución 
 * o si hay división entre cero en el método.
 */
Array1d *forwardSubstitution(Array2d *L, Array1d *b, double tol);

/** 
 * @brief Método de sustitución hacia atrás para sistemas Ux = b
 * 
 * El método trabaja con una matriz cuadrada triangular superior U.
 * 
 * @param U Apuntador simple a una estructura de matriz (2D)
 * @param b Apuntador simple a una estructura de arreglo (1D)
 * @param tol Tolerancia
 * 
 * @return Apuntador simple a una estructura de arreglo (1D)
 * @retval NULL si no hay memoria suficiente para guardar el vector solución 
 * o si hay división entre cero en el método.
 */
Array1d *backwardSubstitution(Array2d *U, Array1d *b, double tol);

#endif