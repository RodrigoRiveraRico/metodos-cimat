#include <stdio.h>
#include <stdlib.h>
#include "circulo.h"

/* 
gcc .\ejercicio2.c .\circulo.c  ..\ejercicio1\polinomio.c ..\arrays\array1D.c ..\arrays\array2D.c ..\cholesky\cholesky.c -lm -o .\ejercicio2 & .\ejercicio2 ..\datosTarea06\circunferencia1.bin ..\datosTarea06\circunferencia2.bin 30 0.01
*/

int main(int argc, char **argv){

    int    N   = 30;         // Número máximo de iteraciones
    double tol = 0.01;       // Tolerancia aproximación del centro de la circunferencia

    /*
     * Argumentos:
     *
     * argv[1] -> archivo de circunferencia
     * argv[2] -> archivo de arco
     * argv[3] -> N   (opcional)
     * argv[4] -> tol (opcional)
     */

     if(argc < 3 || argc > 5){
        printf("\nUso:");
        printf("\n%s archivo_circ archivo_arc [N] [tol]\n", argv[0]);
        return 1;
    }

    /* N */
    if(argc >= 4){

        char *endptr;

        N = (int)strtol(argv[3], &endptr, 10);

        if(*endptr != '\0' || N <= 0){
            printf("\nError: N debe ser un entero positivo.\n");
            return 1;
        }
    }

    /* tol */
    if(argc == 5){

        char *endptr;

        tol = strtod(argv[4], &endptr);

        if(*endptr != '\0' || tol <= 0){
            printf("\nError: tol debe ser un numero positivo.\n");
            return 1;
        }
    }

    //////////////////////////////////////////////////////////
    ///////////////////// CIRCUNFERENCIA /////////////////////
    //////////////////////////////////////////////////////////

    // Nombre del archivo .bin de los datos por ajustar (circunferencia)
    const char *archivo_de_puntos_circulo_bin = argv[1];     // "../datosTarea06/circunferencia1.bin"
    // Nombre del archivo .bin de los coeficientes ajustados (circunferencia)
    const char *archivo_de_salida_circulo_bin = "./datosTarea06/coeficientes_circunferencia1.bin";
    // Hay que tokenizar para construir la ruta del archivo de salida.

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
    printf(">>> Exito en la escritura del archivo binario.\n");
    freeArray1d(coeficientes);
    coeficientes = NULL;

    //////////////////////////////////////////////////////////
    ////////////////////////// ARCO //////////////////////////
    //////////////////////////////////////////////////////////

    // Nombre del archivo .bin de los datos por ajustar (arco)
    const char *archivo_de_puntos_arco_bin = argv[2];    // "../datosTarea06/circunferencia2.bin";
    // Nombre del archivo .bin de los coeficientes ajustados (arco)
    const char *archivo_de_salida_arco_bin = "./datosTarea06/coeficientes_circunferencia2.bin";
    // Hay que tokenizar para construir la ruta del archivo de salida.

    coeficientes = metodoMinimosCuadradosCirculo(archivo_de_puntos_arco_bin, N, tol);
    if(!coeficientes){
        return 1;
    }

    // Grabar datos (arco)
    printf(">>> Writing coeficientes en %s\n",archivo_de_salida_arco_bin);
    if(writeArray1d(coeficientes, archivo_de_salida_arco_bin) == 1){
        printf("!!! Error en la escritura del archivo binario.\n");
        freeArray1d(coeficientes);
        return 1;
    }
    printf(">>> Exito en la escritura del archivo binario.\n");
    freeArray1d(coeficientes);

    return 0;
}