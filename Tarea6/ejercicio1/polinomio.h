#ifndef POLINOMIO_H
#define POLINOMIO_H

#include "../arrays/array2D.h"
#include "../arrays/array1D.h"

/**
 * @brief Estructura para representar puntos en 2D
 */
typedef struct {
    size_t m;               /**< Cantidad de puntos */
    Array1d *arreglo_x;     /**< Arreglo 1D para los puntos x */
    Array1d *arreglo_y;     /**< Arreglo 1D para los puntos y */
}Puntos2d;

/**
 * @brief Función que crea la matriz de diseño X para un ajuste de m puntos (x_{i}, y_{i}) en el plano por medio de un polinomio de grado n.
 * 
 * Cada i-ésimo renglón de la matriz X es [ x_{i}^n  x_{i}^(n-1)  ...  x_{i}^2  x_{i}  1 ]
 * 
 * @param arreglo_x Estructura 1D. Lista de la componente horizontal x_{i} de los puntos.
 * @param m int Tamaño de la lista.
 * @param n int Grado del polinomio.
 * @return Estructura 2D. La matriz de diseño X de tamaño m x (n+1).
 */
Array2d *crearMatrizDisenoPolinomio(Array1d *arreglo_x, int m, int n);

/**
 * @brief Función que calcula el producto matricial A^T A
 * 
 * Dada una matriz A de tamaño m x n la función calcula el pruducto A^T A
 * 
 * La matriz resultante es de tamaño n x n y simétrica.
 * 
 * La función recibe la matriz A y devuelve la matriz A^T A
 * 
 * @param matriz Estructura 2D. Matriz A
 * @return Estructura 2D. Matriz resultante A^T A
 */
Array2d *transpuestaPorMatriz(Array2d *matriz);

/**
 * @brief Función que calcula el producto matriz por vector A^T v
 * 
 * Dada una matriz A de tamaño m x n y un vector de tamaño m x 1 la función calcula el producto A^T v
 * 
 * El vector resultante es de tamaño n x 1
 * 
 * La función recibe la matriz A y devuelve el vector A^T v
 * @param matriz Estructura 2D. Matriz A
 * @param vector Estructura 1D. Vector v
 * @return Estructura 1D. Vector resultante A^T v
 */
Array1d *matrizTranspuestaPorVector(Array2d *matriz, Array1d *vector);

/**
 * @brief Ajuste de un polinomio por mínimos cuadrados usando Cholesky como método de solución.
 * 
 * Buscamos los coeficientes de un polinomio c_{n}x^(n) + c_{n-1}x^(n-1) + ... + c_{1}x + c_{0}
 * 
 * Sea X de tamaño m x (n+1) la matriz de diseño,
 * c el vector de los (n+1) coeficientes de un polinomio de grado n,
 * y el vector de las componentes verticales y_{i} de m puntos (x_{i}, y_{i}).
 * 
 * El sistema a resolver es X^T X c = X^T y
 * 
 * Con Cholesky factorizamos X^T X = L L^T
 * 
 * Resolvemos el sistema L L^T c = X^T y
 * 
 * Si el i-ésimo renglón de X es [ x_{i}^n  x_{i}^(n-1)  ...  x_{i}^2  x_{i}  1 ] 
 * entonces el vector de coeficientes c estará ordenado de mayor a menor grado [ c_{n}  c_{n-1}  ...  c_{1} c_{0} ]
 * 
 * @param matriz Estructura 2D. Matriz de diseño X
 * @param arreglo_y Estructura 1D. Lista de la componente vertical y_{i} de los puntos.
 * @param n int Grado del polinomio
 * @return Estructura 1D. Lista de los coeficientes del polinomio.
 * @note El orden de los coeficientes del polinomio es el mismo que el de los renglones de la matriz de diseño.
 */
Array1d *minimosCuadradosPolinomio(Array2d *matriz, Array1d *arreglo_y, int n);

/**
 * @brief Función que lee un archivo binario de puntos 2D (x_{i}, y_{i})
 * 
 * El archivo binario debe contener:
 * 
 * 1) Un entero: Cantidad m de puntos.
 * 
 * 2) Un entero: Número de variables. (Debe ser 2).
 * 
 * 3) 2*m doubles: Secuencia de m puntos (x_{i}, y_{i}). (Componente horizontal seguido de componente vertical).
 * 
 * @param cfile string Ruta archivo .bin
 * @return Estructura para representar puntos en 2D
 * @note No olvidar incluir la extensión .bin
 */
Puntos2d *readPuntos2D(const char *cfile);

/**
 * @brief Wrap del método de mínimos cuadrados para ajustar un polinomio a un conjunto de puntos en el plano.
 * 
 * Se obtienen los coeficientes de un polinomio c_{n}x^(n) + c_{n-1}x^(n-1) + ... + c_{1}x + c_{0} en este mismo orden de aparición.
 * 
 * 1) Lee de un archivo binario el conjunto de puntos (x_{i}, y_{i}) a ajustar.
 * 
 * 2) Creación de la matriz X de diseño. El i-ésimo renglón de X es [ x_{i}^n  x_{i}^(n-1)  ...  x_{i}^2  x_{i}  1 ] 
 * 
 * 3) Se resuelve mínimos cuadrados con Cholesky para obtener el arreglo c de coeficientes.
 * 
 * 4) Se escribe un archivo binario con los coeficientes: Un entero (número de coeficientes) seguido de los coeficientes ordenados de mayor a menor grado [ c_{n}  c_{n-1}  ...  c_{1} c_{0} ] (doubles)
 * 
 * Se imprime información relevante como los coeficientes obtenidos y el error cuadrático medio (RMSE).
 * 
 * RMSE = ||Xc-y||/sqrt(m) donde ||.|| es la norma 2 y m el número de puntos (x, y)
 * 
 * @param nombre_archivo string Ruta archivo .bin
 * @param grado_polinomio int Grado n del polinomio
 * @return Estructura 1D. Lista de los (n+1) coeficientes del polinomio
 * @note No olvidar incluir la extensión .bin
 * @note Ver función readPuntos2D para tener información sobre el archivo binario de lectura.
 */
Array1d *metodoMinimosCuadradosPolinomio(const char * nombre_archivo, int grado_polinomio);

#endif