#include <stdio.h>
#include <stdlib.h>
#include "polinomio.h"

/* 
gcc .\ejercicio1.c .\polinomio.c ..\arrays\array1D.c ..\arrays\array2D.c ..\cholesky\cholesky.c -lm -o .\ejercicio1 & .\ejercicio1 ..\datosTarea06\puntos2D.bin
*/
int main(int argc, char **argv){

    if(argc<2){
        printf("Hay que proporcionar como parámetro el archivo binario que contiene los puntos a ajustar.");
        return 1;
    }

    // Nombre del archivo .bin de los datos por ajustar
    const char *archivo_de_punto2d_bin = argv[1];

    // Aquí se guardan los coeficientes del polinomio ajustado
    Array1d *coeficientes_polinomio = NULL;

    // Para polinomios de grado 2, 4, y 6
    for(int grado=2;grado<=6;grado+=2){
        coeficientes_polinomio = metodoMinimosCuadradosPolinomio(archivo_de_punto2d_bin, grado);
        if(!coeficientes_polinomio){
            return 1;
        }
        freeArray1d(coeficientes_polinomio);
    }
    return 0;
}