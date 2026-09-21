#ifndef CHOLESKY_H
#define CHOLESKY_H

#include "../arrays/array2D.h"
#include "../arrays/array1D.h"

/// @brief Método de cholesky A = LL^T
///
/// El método funciona con matrices simétricas y definidas positivamente.
/// 
/// El método indica si la matriz no es definida positivamente
/// @param A Estructura 2D La matriz
/// @param n size_t Tamaño de la matriz nxn
/// @return Estrucutra 2D. La matriz L de la factorización LL^T
/// @note El método no determina si una matriz es simétrica o no, es resposabilidad del usuario usar el método solo con matrices simétricas.
Array2d *cholesky(Array2d *A, size_t n);

/// @brief Solucón al sistema LL^T x = b
///
/// El método usa sustitución hacia adelante y sustitución hacia atrás.
/// @param L Estructura 2D. Matriz triangular inferior.
/// @param b Estructur 1D. Coeficientes independientes.
/// @param n size_t El tamaño de la matriz nxn
/// @return Estructura 1D La solución x del sistema.
/// @note En la sustitución hacia adelante y hacia atrás se usa una tolerancia para evitar operaciones que dividan entre cero.
Array1d *solveLLT(Array2d *L, Array1d *b, size_t n);

/// @brief Obtención de la matriz transpuesta para matrices cuadradas
/// @param matriz Estructura 2D La matriz original
/// @param n size_t El tamaño de la matriz nxn
/// @return Estructura 2D. La matriz transpuesta
Array2d *transpose_cuadrada(Array2d *matriz,  size_t n);

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

/// @brief Cálculo de la norma de Frobenius ||A-B||
///
/// Ambas matrices deben ser del mismo tamaño
/// @param A Estrucutra 2D
/// @param B Estructura 2D
/// @return double La norma
double frobenius(Array2d *A, Array2d *B);

/// @brief Cálculo del error residual ||Ax-b|| del sistema Ax = b
///
/// ||.|| es la norma 2
/// 
/// @param A Estructura 2D. La matriz de coeficientes
/// @param x Estrcutura 1D. La solución del sistema
/// @param b Estructura 1D. El arreglo de valores independientes
/// @return double El error residual
double errorResidual(Array2d *A, Array1d *x, Array1d *b);

// Devuelve el épsilon de la máquina
double epsilon(void);
#endif