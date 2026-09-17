#ifndef SOL_ECU_LIN_H
#define SOL_ECU_LIN_H

#include "../arrays/array1D.h"
#include "../arrays/array2D.h"

#define EXITO 1 // Éxito en la factorización.
#define SINGULAR 0  // Matriz singular

/** 
 * @brief Estructura para la salida de la función LU
 */ 
typedef struct{
    Array2d *L; /**< Apuntador a matriz triangular inferior L */
    Array2d *U; /**< Apuntador a matriz triangular superior U */
    Array1d *p; /**< Apuntador a arreglo de permutación p */
    int res;    /**< 0 si la matriz es singular, 1 en caso contrario */
} outLU;

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

/** 
 * @brief Factorización LU con pivoteo parcial.
 * 
 * Sistema a resolver: Ax = b. 
 * 
 * Esta función devuelve la factorización LU de A para transformar el sistema a LUx = Pb. 
 * 
 * L tiene unos (1) en la diagonal. 
 * 
 * Representamos la matriz de permutación P como un arreglo unidimensional p cuyas entradas representan el intercambio de filas. 
 * 
 * @param A Apuntador simple a una estructura de matriz (2D)
 * @param tol Tolerancia
 * 
 * @return Apuntador simple a una estructura: 
 * Apuntador simple a matriz triangular inferior L. 
 * 
 * Apuntador simple a matriz triangular superior U.
 * 
 * Apuntador simple a arreglo de permutación p.
 * 
 * Entero 0 si la matriz A es singular, 1 en caso contrario.
 * @retval NULL si no hay memoria suficiente.
 */
outLU *LU(Array2d *A, double tol);

/** 
 * @brief Solución al sistema LUx = Pb
 * 
 * L matriz triangular inferior con unos (1) en la diagonal. 
 * 
 * U matriz triangular superior. 
 * 
 * P matriz de permutación; está representada como un arreglo unidimensional p cuyas entradas representan el intercambio de filas. 
 * 
 * Primero se calcula la solución y del sistema Ly = Pb con y = Ux empleando el métodos de sustitución hacia adelante.
 * 
 * Después se calcula la solución x del sistema Ux = y empleando el método de sustitucipon hacia atrás.
 * 
 * @param L Apuntador simple a una estructura de matriz (2D)
 * @param U Apuntador simple a una estructura de matriz (2D)
 * @param p Apuntador simple a una estructura de arreglo (1D)
 * @param b Apuntador simple a una estructura de arreglo (1D)
 * @param tol Tolerancia
 * 
 * @return Apuntador simple a una estructura de arreglo (1D)
 * @retval NULL si no hay memoria suficiente, si el método de sustitución hacia adelante falla o si el método de sustitución hacia atrás falla.
 */
Array1d *solveLU(Array2d *L, Array2d *U, Array1d *p, Array1d *b, double tol);

#endif