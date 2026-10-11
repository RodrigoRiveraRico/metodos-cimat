#ifndef CIRCULO_H
#define CIRCULO_H

#include "../arrays/array2D.h"
#include "../arrays/array1D.h"

/**
 * @brief Función que calcula el promedio de un conjunto de datos
 * 
 * @param arreglo Estructura 1D. Conjunto de datos
 * @param m int Tamaño del conjunto
 * @return double Promedio
 */
double promedio(Array1d *arreglo, int m);

/**
 * @brief Función que recibe la pareja (x, y) en dos listas separadas y devuelve una lista con las componentes x e y consecutivamente.
 *
 * Entrada: arreglo_x = [x1 x2 ... xm]
 * 
 * Entrada: arreglo_y = [y1 y2 ... ym]
 * 
 * Salida: [x1 y1 x2 y2 ... xm ym]
 * 
 * @param arreglo_x Estructura 1D. Componentes x
 * @param arrelo_y Estructura 1D. Componentes y
 * @param m int Cantidad de puntos (x, y) (tamaño de cada lista)
 * @return Estructura 1D. Componente x y componente y consecutivamente
 * @note El usuario debe garantizar que ambos arreglos tienen el mismo tamaño m.
 */
Array1d *mergePuntos2DTo1D(Array1d *arreglo_x, Array1d *arreglo_y, int m);

/**
 * @brief Función que crea la matriz de diseño para el ajuste de un círculo a un conjunto de puntos 2D por mínimos cuadrados
 * 
 * La matriz de diseño X es de tamaño 2*m x 3
 * 
 * Renglón i-ésimo (par) e i-ésimo + 1 (impar):
 * 
 * [ 1 0 cos(θ_{i}) ]
 * 
 * [ 0 1 sin(θ_{i}) ]
 * 
 * El ángulo θ_{i} se calcula usando la función atan2( y_{i} - y^{barra} , x_{i} - x^{barra} )
 * 
 * @param arreglo_x Estructura 1D. Componentes x del conjunto de puntos (x, y)
 * @param arreglo_y Estructura 1D. Componentes y del conjunto de puntos (x, y)
 * @param m int Cantidad de puntos (x, y) (tamaño de cada lista)
 * @param x_barra double Promedio del conjunto arreglo_x
 * @param y_barra double Promedio del conjunto arreglo_y
 * @return Estructura 2D. La matriz X de diseño
 * @note Es responsabilidad del usario garantizar que arreglo_x y arreglo_y tengan la misma cantidad m de elementos.
 */
Array2d *crearMatrizDisenoCirculo(Array1d *arreglo_x, Array1d *arreglo_y, int m, double x_barra, double y_barra);

/**
 * @brief Ajuste de un círculo por mínimos cuadrados usando Cholesky como método de solución
 * 
 * Buscamos los coeficientes c1, c2, c3 de la parametrización 
 * x = c1 + c3 cos(θ), 
 * y = c2 + c3 sin(θ)
 * 
 * Sea X de tamaño 2*m x 3 la matriz de diseño,
 * c el vector de los 3 coeficientes que parametrizan una circunferencia,
 * z el vector de los m puntos (x, y)
 * 
 * El vector z es de tamaño 2*m pues está formado por las m-parejas (x_{i}, y_{i}) dispuestos de forma consecutiva [ x1 y1 x2 y2 ... xm ym ]
 * 
 * El sistema a resolver es X^T X c = X^T z
 * 
 * Con Cholesky factorizamos X^T X = L L^T
 * 
 * Resolvemos el sistemas L L^T = X^T z
 * 
 * Si la matriz de diseño X es tal que
 * renglón i-ésimo (par) e i-ésimo + 1 (impar):
 * 
 * [ 1 0 cos(θ_{i}) ]
 * 
 * [ 0 1 sin(θ_{i}) ]
 * 
 * entonces, el arreglo de coeficientes es [ c1 c2 c3 ] donde: 
 * c1 es la componente x del centro,
 * c2 es la componente y del centro,
 * c3 es el radio.
 * 
 * @param X Estructura 2D. Matriz de diseño de tamaño 2*m x 3
 * @param z Estructura 1D. Lista de tamaño 2*m con las parejas (x, y) de forma consecutiva
 * @return Estructura 1D. Lista con los coeficientes del ajuste.
 * @note El orden de los coeficientes está en función de la matriz de diseño.
 */
Array1d *minimosCuadradosCirculo(Array2d *X, Array1d *z);

/**
 * @brief Wrap del método de mínimos cuadrados que ajusta una circunferencia a un conjunto de puntos 2D
 * 
 * Se obtienen los coeficientes c1, c2, c3 de la parametrización 
 * x = c1 + c3 cos(θ),
 * y = c2 + c3 sin(θ)
 * 
 * 1) Lee de un archivo binario el conjunto de puntos (x_{i}, y_{i}) a ajustar.
 * 
 * 2) Ciclo de iteraciones: la condición de paro es ||(x^{barra} - c1 , y^{brra} - c2)|| < tol 
 * (La diferencia entre el ajuste obtenido en la iteración actual con la iteración anterior es menor a una tolerancia)
 * 
 * 2.1) Creación de la matriz X de diseño.
 * 
 * 2.2) Se resuelve mínimos cuadrados con Cholesky para obtener el arreglo de coeficientes.
 *
 *  Se imprime información relevante como los coeficientes obtenidos y el error cuadrático medio (RMSE).
 * 
 * RMSE = ||Xc-z||/sqrt(m) donde ||.|| es la norma 2 y m el número de puntos (x, y)
 * 
 * La función define variables estáticas para mantener los valores c1, c2, c3 del ajuste obtenido.
 * Esto con la finalidad de que para un segundo conjunto de datos se utilicen en la primera iteración el ajuste anterior obtenido.
 * 
 * En la primera llamada de la función la estimación del centro (primera iteración) es el centroide de los datos.
 * 
 * En la llamadas subsecuentes, la estimación del centro (primera iteración) es el ajuste obtenido en la llamada anterior.
 * 
 * @param nombre_archivo string Ruta archivo .bin
 * @param N int Número máximo de iteraciones
 * @param tol double Tolerancia
 * @return Estructura 1D. Lista con los coeficientes del ajuste.
 * @note No olvidar incluir la extensión .bin
 */
Array1d *metodoMinimosCuadradosCirculo(const char *nombre_archivo, int N, double tol);

#endif