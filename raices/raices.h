#ifndef RAICES_H
#define RAICES_H

/** 
 * @brief Método de bisección
 * 
 * @param p Apuntador a arreglo de (6 * sizeof(double))
 * @param func Función f: R -> R continua
 * @param a Valor del intervalo [a,b]
 * @param b Valor del intervalo [a,b]. Los extremos a y b son tal que f(a)f(b) < 0
 * @param N Número máximo de iteraciones
 * @param tol Tolerancia
 * @return Array: 
 * [0] La última aproximación x_k ∈ [a_k, b_k] de una raíz de f. 
 * [1] El valor f_k = f(x_k).
 * [2] El número k de iteraciones realizadas.
 * [3] [4] los extremos a_k, b_k del último intervalo generado.
 * [5] Una variable 'res' que indica si se cumplió la condición de paro del algoritmo: 
 * res = 1 si |f(x_k)| < tol o res = 0 en caso contrario.
 */
int biseccion(double *p, double (*func)(double), double a, double b, int N, double tol);

/**
 * @brief Método de Newton-Raphson
 * 
 * @param p Apuntador a arreglo de (4 * sizeof(double))
 * @param func Función f: R -> R continua
 * @param dfunc Derivada de f
 * @param x Un punto en el dominio de f
 * @param N Número máximo de iteraciones
 * @param tol Tolerancia
 * @return Array: 
 * [0] El último punto x_k que genera el algoritmo.
 * [1] El valor f_k = f(x_k).
 * [2] El número k de iteraciones realizadas.
 * [3] Una variable 'res' que indica si se cumplió la condición de paro del algoritmo: 
 * res = 1 si |f(x_k)| < tol o res = 0 en caso contrario.
 */
int newton_r(double *p, double (*func)(double), double (*dfunc)(double), double x, int N, double tol);

#endif