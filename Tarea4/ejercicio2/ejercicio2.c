#include <stdio.h>
#include <math.h>
#include "cholesky.h"

#define SALUDO printf("\nHola :D\n")

#define OK 1
#define ERROR_INPUT 2
#define ERROR_READ 4
#define ERROR_METHOD 8
#define ERROR_MEMORY 16

int main(int argc, char **argv){

    SALUDO;
    Array1d *b=NULL, *x=NULL;
    Array2d *A=NULL, *L=NULL;

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
    L = cholesky(A,n);
    if(!L){
        freeArray1d(b);
        freeArray2d(A);
        return ERROR_METHOD;
    }
    Array2d *LT = transpose_cuadrada(L,n);
    if(!LT){
        freeArray1d(b);
        freeArray2d(A);
        freeArray2d(L);
        return ERROR_MEMORY;
    }
    // En C guardamos el producto LL^T
    Array2d *C = array2d_alloc(n,n);    // Ya está inicializada en ceros
    if(!C){
        freeArray1d(b);
        freeArray2d(A);
        freeArray2d(L);
        freeArray2d(LT);
        return ERROR_MEMORY;
    }
    // Producto LL^T
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            for (size_t k = 0; k < n; k++) {
                C->data[i][j] += L->data[i][k] * LT->data[k][j];
            }
        }
    }
    printf("\n|| A - LL^T || = %.10e\n",frobenius(A,C));
    freeArray2d(LT);
    freeArray2d(C);

    printf("\n... Solving LL^T x = b ...\n");
    x = solveLLT(L, b, n);
    if(!x){
        freeArray1d(b);
        freeArray2d(A);
        freeArray2d(L);
        return ERROR_METHOD;
    }
    printf("\nSolucio'n x = ");
    printArray1d(x, "% 6.2f ", 3);
    printf("\n|| Ax - b || = %.10e\n", errorResidual(A,x,b));

    freeArray1d(x);
    freeArray1d(b);
    freeArray2d(A);
    freeArray2d(L);

    return OK;
}