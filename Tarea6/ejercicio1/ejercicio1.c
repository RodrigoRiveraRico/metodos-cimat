#include <stdio.h>
#include <stdlib.h>
#include "polinomio.h"

int main(void){

    Array1d *respuesta = ejercicio1("../datosTarea06/puntos2D.bin", 6);
    if(!respuesta){
        return 1;
    }

    printArray1d(respuesta, "%f ", 10);


    freeArray1d(respuesta);
    return 0;
}

