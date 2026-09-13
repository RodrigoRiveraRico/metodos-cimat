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

    Array1d *b=NULL, *x=NULL;
    Array2d *L=NULL;

    double tol = pow(epsilon(),2.0/3.0);

    if(argc<3) {
        printf("Hay que proporcionar dos para'metros:");
        printf("1. El nombre del archivo binario de un arreglo 1D");
        printf("2. El nombre del archivo binario de un arreglo 2D");
        return ERROR_INPUT;
    } 

    b = readArray1d(argv[1]);
    if(!b){return ERROR_READ_BIN;}
    
    L = readArray2d(argv[2]);
    if(!L){
        freeArray1d(b);
        return ERROR_READ_BIN;
    }
   
    /*Tamaño de b*/
    printf("\nEl tamano del vector b es <%zu>",b->n);
    /*Elementos de b*/
    // printf("\n");
    // printArray1d(b, "% 6.2f  ", 3);

    /*Tamaño de L*/
    printf("\nLa matriz L tiene <%zu> filas y <%zu> columnas", L->rows, L->cols);
    /*Elementos de L*/
    // printf("\n");
    // printArray2d(L, "% 6.2f  ", 3);

    /*Obtenemos la solución*/
    x = forwardSubstitution(L, b, tol);
    if(!x){
        printf("\n%s","El sistema no tiene solucio'n u'nica.");
        freeArray1d(b);
        freeArray2d(L);    
        return ERROR_METHOD;
    }

    /*Elementos de x*/
    printf("\nLa solucio'n del sistema (mostramos los primeros y u'ltimos 3 elementos) es:\n");
    printArray1d(x, "% 6.2f  ", 3);

    /*Error residual*/
    // Vector residual: Lx-b
    double r_i; /**< Entrada i-ésima del vector residual */
    double err_residual = 0;

    for(size_t i=0;i<L->rows;i++){
        r_i = 0;
        for(size_t j=0;j<L->cols;j++){
            r_i += L->data[i][j] * x->data[j];
        }
        r_i -= b->data[i];
        err_residual += r_i*r_i;
    }
    err_residual = sqrt(err_residual);

    printf("\nError residual ||Lx-b|| = %e",err_residual);

    freeArray2d(L); freeArray1d(b); freeArray1d(x);

    return OK;
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