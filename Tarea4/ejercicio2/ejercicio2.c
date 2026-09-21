#include <stdio.h>
#include <math.h>
#include "cholesky.h"

#define SALUDO printf("\nHola :D\n")

#define OK 1
#define ERROR_INPUT 2
#define ERROR_READ 4
#define ERROR_METHOD 8

int main(int argc, char **argv){

    SALUDO;
    Array1d *b=NULL, *x=NULL;
    Array2d *A=NULL, *chol=NULL;

    if(argc<3) {
        printf("Hay que proporcionar dos para'metros:");
        printf("1. El nombre del archivo binario de un arreglo 1D");
        printf("2. El nombre del archivo binario de un arreglo 2D");
        return ERROR_INPUT;
    } 

    printf("\n... Reading vector b ...\n");
    b = readArray1d(argv[1]);
    if(!b){return ERROR_READ;}
    
    printf("\n... Reading matriz A ...\n");
    A = readArray2d(argv[2]);
    if(!A){
        freeArray1d(b);
        return ERROR_READ;
    }

    printf("\nMatriz de <%zu> filas y <%zu> columnas\n", A->rows, A->cols);

    printf("\n... Solving Cholesky ...\n");
    size_t n = A->rows; // Matriz cuadrada
    chol = cholesky(A,n);
    if(!chol){
        freeArray1d(b);
        freeArray2d(A);
        return ERROR_METHOD;
    }
    printf("\n|| A - LL^T || = %f\n",frobenius(A,chol));

    printf("\n... Solving LL^T x = b ...\n");
    x = solveLLT(chol, b, n);
    if(!x){
        freeArray1d(b);
        freeArray2d(A);
        freeArray2d(chol);
        return ERROR_METHOD;
    }
    printf("\nSolucio'n x = ");
    printArray1d(x, "% 6.2f ", 3);
    printf("\n|| Ax - b || = %.10e\n", errorResidual(A,x,b));

    freeArray1d(x);
    freeArray1d(b);
    freeArray2d(A);
    freeArray2d(chol);

    return OK;
}