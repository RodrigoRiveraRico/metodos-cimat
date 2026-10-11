#include <stdio.h>
#include <stdlib.h>
#include "circulo.h"

/* 
gcc .\ejercicio2.c .\circulo.c  ..\ejercicio1\polinomio.c ..\arrays\array1D.c ..\arrays\array2D.c ..\cholesky\cholesky.c -lm -o .\ejercicio2 & .\ejercicio2
*/

int main(void){
    int    N   = 30;         // Número máximo de iteraciones
    double tol = 0.01;       // Tolerancia aproximación del centro de la circunferencia

    //////////////////////////////////////////////////////////
    ///////////////////// CIRCUNFERENCIA /////////////////////
    //////////////////////////////////////////////////////////

    // Nombre del archivo .bin de los datos por ajustar (circunferencia)
    const char *archivo_de_puntos_circulo_bin = "../datosTarea06/circunferencia1.bin";
    // Nombre del archivo .bin de los coeficientes ajustados (circunferencia)
    const char *archivo_de_salida_circulo_bin = "../datosTarea06/coeficientes_circunferencia1.bin";

    Array1d *coeficientes = metodoMinimosCuadradosCirculo(archivo_de_puntos_circulo_bin, N, tol);
    if(!coeficientes){
        return 1;
    }

    // Grabar datos (circunferencia)
    printf(">>> Writing coeficientes en %s\n",archivo_de_salida_circulo_bin);
    if(writeArray1d(coeficientes, archivo_de_salida_circulo_bin) == 1){
        printf("!!! Error en la escritura del archivo binario.");
        freeArray1d(coeficientes);
        return 1;
    }
    printf(">>> Exito en la escritura del archivo binario.");
    freeArray1d(coeficientes);
    coeficientes = NULL;

    //////////////////////////////////////////////////////////
    ////////////////////////// ARCO //////////////////////////
    //////////////////////////////////////////////////////////

    // Nombre del archivo .bin de los datos por ajustar (arco)
    const char *archivo_de_puntos_arco_bin = "../datosTarea06/circunferencia2.bin";
    // Nombre del archivo .bin de los coeficientes ajustados (arco)
    const char *archivo_de_salida_arco_bin = "../datosTarea06/coeficientes_circunferencia2.bin";

    coeficientes = metodoMinimosCuadradosCirculo(archivo_de_puntos_arco_bin, N, tol);
    if(!coeficientes){
        return 1;
    }

    // Grabar datos (arco)
    printf(">>> Writing coeficientes en %s\n",archivo_de_salida_arco_bin);
    if(writeArray1d(coeficientes, archivo_de_salida_arco_bin) == 1){
        printf("!!! Error en la escritura del archivo binario.");
        freeArray1d(coeficientes);
        return 1;
    }
    printf(">>> Exito en la escritura del archivo binario.");
    freeArray1d(coeficientes);
    
    return 0;
}