#include "arrays/array1D.h"
#include "arrays/array2D.h"
#include "sol_ecu_lin/sol_ecu_lin.h"
#include <math.h>

#define OK 1
#define ERROR_METHOD 2
#define ERROR_READ_BIN 4
#define ERROR_INPUT 8

// Devuelve el épsilon de la máquina
double epsilon(void);

int main(int argc, char **argv){

    Array2d *A=NULL;
    Array1d *b=NULL;
    outLU *resultado=NULL;

    double tol = pow(epsilon(),2.0/3.0);

    if(argc<3) {
        printf("Hay que proporcionar dos para'metros:");
        printf("1. El nombre del archivo binario de un arreglo 1D");
        printf("2. El nombre del archivo binario de un arreglo 2D");
        return ERROR_INPUT;
    } 

    b = readArray1d(argv[1]);
    if(!b){return ERROR_READ_BIN;}
    
    A = readArray2d(argv[2]);
    if(!A){
        freeArray1d(b);
        return ERROR_READ_BIN;
    }

    // Factorización LU
    resultado = LU(A,tol);
    if(!resultado){
        freeArray1d(b); freeArray2d(A);
        return ERROR_METHOD;}

    freeArray2d(A); freeArray1d(b);
    freeArray2d(resultado->L); freeArray2d(resultado->U); freeArray1d(resultado->p); free(resultado);
}

double epsilon(void){
    
    double eps = 0.5;
    double unit = 1.0;
    double val = unit + eps;

    while(val > unit){
        eps/=2;
        val = unit + eps;
    }
    eps = 2*eps;

    return eps;
}