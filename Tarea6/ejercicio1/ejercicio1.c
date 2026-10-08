#include <stdio.h>
#include <stdlib.h>
#include "polinomio.h"

/* 
gcc .\ejercicio1.c .\polinomio.c ..\arrays\array1D.c ..\arrays\array2D.c ..\cholesky\cholesky.c -lm -o .\ejercicio1 & .\ejercicio1
*/

int main(void){

    // Nombre del archivo .bin de los datos por ajustar
    const char *archivo_de_punto2d_bin = "../datosTarea06/puntos2D.bin";

    // Aquí se guardan los coeficientes del polinomio ajustado
    Array1d *coeficientes_polinomio = NULL;

    // Para polinomios de grado 2, 4, y 6
    for(int grado=2;grado<=6;grado+=2){
        coeficientes_polinomio = ejercicio1(archivo_de_punto2d_bin, grado);
        if(!coeficientes_polinomio){
            return 1;
        }
        freeArray1d(coeficientes_polinomio);
    }
    return 0;
}