#ifndef GAUSS_SEIDEL_H
#define GAUSS_SEIDEL_H

#include "../arrays/array2D.h"
#include "../arrays/array1D.h"

#define EXITO 1
#define FALLO 0

/// @brief Estructura para representar la salida de la función solveTridiagonal
typedef struct{
    Array1d *x; /*< Vector solución */
    Array1d *r; /*< Vector residual */
    int t;      /*< Número de iteraciones realizadas */
    int res;    /*< Entero para determinar el éxito o fallo del método*/
}output;

/// @brief Hace la multiplicación matriz por vector Ax
/// @param A Estructura 2D. Matriz cuadrada tridiagonal
/// @param x Estructura 1D. Vector
/// @return Estructura 1D. Vector resultante de Ax
Array1d *AxTridiagonal(Array2d *A, Array1d *x);

/// @brief Resuelve un sistema de ecuaciones para una matriz tridiagonal Ax = b
/// @param A Estructura 2D. Matriz cuadrada tridiagonal
/// @param b Estructura 1D. Vector de coeficientes independientes
/// @param n size_t El tamaño de la matriz nxn
/// @param xo Estructura 1D. Vector inicial
/// @param N int Número máximo de iteraciones
/// @param tol double Tolerancia
/// @return Estructura que almacena: El vetor solucion. El vector residual. Número de iteraciones realizadas. Valor entero que determinar el éxito o fallo del método.
output *solveTridiagonal(Array2d *A, Array1d *b, size_t n, Array1d *xo, int N, double tol);

/// @brief Cálculo || . || norma 2 de un vector
/// @param r Estructura 1d. Vector
/// @param n size_t. Tamaño del vector
/// @return double La norma 2 del vector
double norma2(Array1d *r, size_t n);

/// Devuelve el épsilon de la máquina
double epsilon(void);

#endif