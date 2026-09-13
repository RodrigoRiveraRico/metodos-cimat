#include "../arrays/array1D.h"
#include "../arrays/array2D.h"
#include "sol_ecu_lin.h"
#include <math.h>

Array1d *forwardSubstitution(Array2d *L, Array1d *b, double tol){

    Array1d *x = NULL;

    size_t n = b->n;

    x = array1d_alloc(n);
    if(!x){
        printf("\n%s","No hay memoria suficiente para guardar la solucio'n del sistema.");
        return NULL;}

    // Solución al sistema Lx = b
    double suma;
    for(int i=0;i<n;i++){

        if(fabs(L->data[i][i]) < tol){
            printf("\n%s","Divisio'n entre cero.");
            freeArray1d(x);
            return NULL;
        }

        suma = 0;
        for(int j=0;j<i;j++){
            suma += (L->data[i][j]) * (x->data[j]);
        }
        
        x->data[i] = ((b->data[i]) - suma)/(L->data[i][i]);
    }

    return x;
}

Array1d *backwardSubstitution(Array2d *U, Array1d *b, double tol){

    Array1d *x = NULL;

    size_t n = b->n;

    x = array1d_alloc(n);
    if(!x){
        printf("\n%s","No hay memoria suficiente para guardar la solucio'n del sistema.");
        return NULL;}

    // Solución al sistema Ux = b
    double suma;
    for(int i=n-1;i>=0;i--){

        if(fabs(U->data[i][i]) < tol){
            printf("\n%s","Divisio'n entre cero.");
            freeArray1d(x);
            return NULL;
        }

        suma = 0;
        for(int j=i+1;j<n;j++){
            suma += (U->data[i][j]) * (x->data[j]);
        }
        
        x->data[i] = ((b->data[i]) - suma)/(U->data[i][i]);
    }

    return x;
}