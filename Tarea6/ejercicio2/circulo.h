#ifndef CIRCULO_H
#define CIRCULO_H

#include "../arrays/array2D.h"
#include "../arrays/array1D.h"

double centroide(Array1d *arreglo, int m);

Array1d *mergePuntos2DTo1D(Array1d *arreglo_x, Array1d *arreglo_y, int m);

Array2d *crearMatrizDisenoCirculo(Array1d *arreglo_x, Array1d *arreglo_y, int m, double x_barra, double y_barra);

Array1d *minimosCuadradosCirculo(Array2d *X, Array1d *z, int m);

Array1d *metodoMinimosCuadradosCirculo(const char *nombre_archivo, int N, double tol);

#endif