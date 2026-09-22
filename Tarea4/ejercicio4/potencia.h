#ifndef POTENCIA_H
#define POTENCIA_H

#include "../arrays/array2D.h"
#include "../arrays/array1D.h"

#define EXITO 1
#define FALLO 0

/// @brief Estructura para la salida del método de la potencia
/// @note res es 1 si el método alcanza la tolerancia; 0 si el método alcanza el máximo de iteraciones.
typedef struct{
    Array1d *v;     /*< Vector propio */
    double lambda;  /*< Valor propio */
    int k;          /*< Número de iteraciones realizadas */
    int res;        /*< Variable indicadora */
}output;

/// @brief Método de la potencia
/// @param A Arreglo 2D Matriz cuadrada de nxn
/// @param n size_t Tamaño de la matriz nxn
/// @param vo Arreglo 1D Vector inicial
/// @param N int Número máximo de iteraciones
/// @param tol double Tolerancia
/// @return Estructura que guarda: El vector propio; El valor propio; 
/// Número de iteraciones realizadas; 
/// Variable que indica si el método para por alcanzar el máximo de iteraciones o por alcanzar la tolerancia.
/// @note El método modifica al vector vo recibido in-place.
output *potencia(Array2d *A, size_t n, Array1d *vo, int N, double tol);

/// @brief Multiplicacion matriz A por vector v
/// @param A Estructura 2D Matriz cuadrada de nxn
/// @param v Estructura 1D Vector
/// @param n siez_t Tamaño de la matriz
/// @return Estructura 1D El producto Av
Array1d *Av(Array2d *A, Array1d *v, size_t n);

/// @brief Obtenemos el vector diferencia y - lambda * v
///
/// Ambos vectores del mismo tamaño n
/// @param y Arreglo 1D Vector
/// @param v Arreglo 1D vector
/// @param n size_t Tamaño del vector
/// @param lamnda double Un valor real
/// @return Arreglo 1D Vector
Array1d *diferencia(Array1d *y, Array1d *v, size_t n, double lamnda);

/// @brief Producto punto entre vectores
///
/// Ambos vectores de mismo tamaño n
/// @param x Arreglo 1D Vector
/// @param y Arreglo 1D Vector
/// @param n size_t Tamaño del vector
/// @return double Producto punto
double productoPunto(Array1d *x, Array1d *y, size_t n);

/// @brief Cálculo de la norma 2 de un vector
/// @param v Arreglo 1D Vector
/// @param n size_t Tamaño del vector
/// @return double La norma 2 del vector
double norma2(Array1d *v, size_t n);

/// @brief Normalización vectorial
/// @param v Arreglo 1D Vector
/// @param n size_t Tamaño del vector
/// @return int EXITO si la normalización se logró; FALLO si hay división entre cero
/// @note Normalización in-place.
int normalizacion(Array1d *v, size_t n);

/// Devuelve el épsilon de la máquina
double epsilon(void);



#endif