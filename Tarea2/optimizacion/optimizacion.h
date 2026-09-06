#ifndef OPTIMIZACION_H
#define OPTIMIZACION_H

/** 
 * @brief Método de la Sección Dorada
 * 
 * @param p Apuntador a arreglo de (5 * sizeof(double))
 * @param f Función real
 * @param l Límite inferior del intervalo de búsqueda
 * @param u Límite superior del intervalo de búsqueda
 * @param tol Tolerancia
 * @param N Número de iteraciones
 * @return Array:
 * [0] La última aproximación.
 * [1] Límite inferior del último intervalo de búsqueda.
 * [2] Límite superior del último intervalo de búsqueda.
 * [3] El número de iteraciones.
 * [4] Variable que indica si se cumplió la condición de paro del algoritmo:
 * res = 0 si |f(x_k)| < tol o res = 1 en caso contrario. 
 */
int dorada(double *p, double (*f)(double), double l, double u, double tol, int N);

/** 
 * @brief Método de Newton
 * 
 * @param p Apuntador a arreglo de (4 * sizeof(double))
 * @param df Primera derivada de la función
 * @param ddf Segunda derivada de la función
 * @param x Un punto en el dominio de la función
 * @param tol Tolerancia
 * @param N Número máximo de iteraciones
 * @return Array:
 * [0] La última aproximación.
 * [1] La derivada evaluada en el la última aproximación.
 * [2] El número de iteraciones.
 * [3] Variable que indica si se cumplió la condición de paro del algoritmo:
 * res = 0 si |f(x_k)| < tol o res = 1 en caso contrario. 
 */
int newton(double *p, double (*df)(double), double (*ddf)(double), double x, double tol, int N);


#endif