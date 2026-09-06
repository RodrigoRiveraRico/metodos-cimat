#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "optimizacion/optimizacion.h"
#include "funciones/funciones_prueba.h"
#include "epsilon/epsilon.h"

#define OK 0
#define ERROR_MEM 1 

int initMemory(double **, int);

int main(void){

    double *resultado = NULL;
    int n = 5;

    FILE *pw = fopen("dorada.bin", "wb");
    if(!pw){
        printf("\nError al crear el archivo");
        return 1;
    }

    if(initMemory(&resultado,n) == ERROR_MEM){
        printf("\nNo se pudo asignar memoria.");
        return 1;
    }

    printf("\n*** Seccion dorada ***\n");
    dorada(resultado, f1, 0.5, 4.0, pow(epsilon(),1.0/3.0), 50);
    printf("\nf(x)        = 0.2xln(x) + (x - 2.3)^2");
    printf("\nk           = %d", (int)resultado[3]);
    printf("\nx_k         = %.10e", resultado[0]);
    printf("\nf_k         = %.10e", f1(resultado[0]));
    printf("\nx_u - x_l   = %.10e", resultado[2]-resultado[1]);
    printf("\nres         = %d\n", (int)resultado[4]);
    fwrite(resultado, sizeof(double), n, pw);

    dorada(resultado, f2, 0.0, 1.4, pow(epsilon(),1.0/3.0), 50);
    printf("\nf(x)        = -(1.5 - 3.0 x) sin(18 x)");
    printf("\nk           = %d", (int)resultado[3]);
    printf("\nx_k         = %.10e", resultado[0]);
    printf("\nf_k         = %.10e", f2(resultado[0]));
    printf("\nx_u - x_l   = %.10e", resultado[2]-resultado[1]);
    printf("\nres         = %d\n", (int)resultado[4]);
    fwrite(resultado, sizeof(double), n, pw);

    dorada(resultado, f3, -4.0, 6.0, pow(epsilon(),1.0/3.0), 50);
    printf("\nf(x)        = 0.5(x-1)^2 + 3 sin(x)");
    printf("\nk           = %d", (int)resultado[3]);
    printf("\nx_k         = %.10e", resultado[0]);
    printf("\nf_k         = %.10e", f3(resultado[0]));
    printf("\nx_u - x_l   = %.10e", resultado[2]-resultado[1]);
    printf("\nres         = %d\n", (int)resultado[4]);
    fwrite(resultado, sizeof(double), n, pw);

    dorada(resultado, f4, -4.0, 6, pow(epsilon(),1.0/3.0), 50);
    printf("\nf(x)        = x + 2|sin x|");
    printf("\nk           = %d", (int)resultado[3]);
    printf("\nx_k         = %.10e", resultado[0]);
    printf("\nf_k         = %.10e", f4(resultado[0]));
    printf("\nx_u - x_l   = %.10e", resultado[2]-resultado[1]);
    printf("\nres         = %d\n", (int)resultado[4]);
    fwrite(resultado, sizeof(double), n, pw);
    
    fclose(pw);

    free(resultado);

    return 0;
}

int initMemory(double **pv, int n){

    *pv = malloc(n*sizeof(double));

    if(!pv){
        return ERROR_MEM;
    }

    return OK;
}