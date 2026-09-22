#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gauss_seidel.h"

#define SALUDO printf("\nHola :D\n")

#define OK 1
#define ERROR_INPUT 2
#define ERROR_READ 4
#define ERROR_METHOD 8
#define ERROR_MEMORY 16

int main(int argc, char **argv){

    SALUDO;
    Array1d *b=NULL;
    Array2d *A=NULL;
    output *resultado = NULL;

    /* Valores predeterminados */
    int N = 100;
    double tol = sqrt(epsilon());

    /*
     * Argumentos:
     *
     * argv[1] -> archivo de vector b
     * argv[2] -> archivo de matriz A
     * argv[3] -> N   (opcional)
     * argv[4] -> tol (opcional)
     */

    if(argc < 3 || argc > 5){
        printf("\nUso:");
        printf("\n%s archivo_b archivo_A [N] [tol]\n", argv[0]);
        return ERROR_INPUT;
    }

    /* N */
    if(argc >= 4){

        char *endptr;

        N = (int)strtol(argv[3], &endptr, 10);

        if(*endptr != '\0' || N <= 0){
            printf("\nError: N debe ser un entero positivo.\n");
            return ERROR_INPUT;
        }
    }

    /* tol */
    if(argc == 5){

        char *endptr;

        tol = strtod(argv[4], &endptr);

        if(*endptr != '\0' || tol <= 0){
            printf("\nError: tol debe ser un numero positivo.\n");
            return ERROR_INPUT;
        }
    }

    printf("\nParametros:");
    printf("\nN   = %d", N);
    printf("\ntol = %.3e\n", tol);

    printf("\n... Reading vector b ...\n");
    b = readArray1d(argv[1]);
    if(!b){return ERROR_READ;}
    
    printf("\n... Reading matriz A ...\n");
    A = readArray2d(argv[2]);
    if(!A){
        freeArray1d(b);
        return ERROR_READ;
    }

    size_t n = A->rows; // Tamaño de la matriz
    printf("\nEl tamano de la matriz es <%zu>\n",n);

    Array1d *x = array1d_alloc(n);  // Ya se inicializa con ceros
    if(!x){
        freeArray1d(b); freeArray2d(A);
        return ERROR_MEMORY;
    }

    resultado = solveTridiagonal(A,b,n,x,N,tol);
    if(!resultado){
        freeArray1d(b); freeArray2d(A); freeArray1d(x);
        return ERROR_METHOD;
    }

    if(resultado->res == EXITO){
        printf("\nEl algoritmo converge\n");
    }
    else{
        printf("\nEl algoritmo no converge\n");
    }
    printf("\nSolucio'n x = ");
    printArray1d(x, "% 6.2f ", 3);
    printf("\n|| Ax - b || = %.10e\n",errResidual(resultado->r,n));
    printf("\nNu'mero de iteraciones realizadas: <%d>",resultado->t);


    freeArray1d(b); freeArray2d(A); freeArray1d(x);
    freeArray1d(resultado->r); free(resultado);
    return OK;
}