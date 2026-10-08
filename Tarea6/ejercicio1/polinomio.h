#ifndef POLINOMIO_H
#define POLINOMIO_H

#include "../arrays/array2D.h"
#include "../arrays/array1D.h"

typedef struct {
    size_t m;
    Array1d *arreglo_x;
    Array1d *arreglo_y;
}Puntos2d;

Array2d *crearMatrizDiseno(Array1d *arreglo_x, int m, int n);

Array2d *transpuestaPorMatriz(Array2d *matriz);

Array1d *matrizTranspuestaPorVector(Array2d *matriz, Array1d *vector);

Array1d *minimosCuadradosPolinomio(Array2d *matriz, Array1d *arreglo_y, int m, int n);

Puntos2d *readPuntos2D(const char *cfile);

Array1d *metodoMinimosCuadradosPolinomio(const char * nombre_archivo, int grado_polinomio);

#endif