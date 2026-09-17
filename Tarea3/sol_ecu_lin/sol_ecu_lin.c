#include "../arrays/array1D.h"
#include "../arrays/array2D.h"
#include "sol_ecu_lin.h"
#include <math.h>
#include <stdlib.h>

Array1d *forwardSubstitution(Array2d *L, Array1d *b, double tol){

    Array1d *x = NULL;

    size_t n = b->n;

    x = array1d_alloc(n);
    if(!x){
        printf("\n%s","No hay memoria suficiente para guardar la solucio'n del sistema.");
        return NULL;}

    // Solución al sistema Lx = b
    double suma;
    for(size_t i=0;i<n;i++){

        if(fabs(L->data[i][i]) < tol){
            printf("\n%s","Divisio'n entre cero.");
            freeArray1d(x);
            return NULL;
        }

        suma = 0;
        for(size_t j=0;j<i;j++){
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
    for(size_t i=n;i-- > 0; ){

        if(fabs(U->data[i][i]) < tol){
            printf("\n%s","Divisio'n entre cero.");
            freeArray1d(x);
            return NULL;
        }

        suma = 0;
        for(size_t j=i+1;j<n;j++){
            suma += (U->data[i][j]) * (x->data[j]);
        }
        
        x->data[i] = ((b->data[i]) - suma)/(U->data[i][i]);
    }

    return x;
}

outLU *LU(Array2d *A, double tol){

    Array2d *L=NULL,*U=NULL;
    Array1d *p=NULL;

    /* Validar A */
    if(!A){ 
        printf("\n%s","Matriz no válida.");
        return NULL;
    }
    if (A->rows != A->cols) {
        printf("\n%s","No es matriz cuadrada.");
        return NULL;
    }
    size_t n = A->rows; //Matriz cuadrada de tamaño nxn


    /* Memoria para salida de la función */
    outLU *out = malloc(sizeof *out);
    if(!out){
        printf("\n%s","No hay memoria suficiente para guardar la salida de la funcio'n");
        return NULL;
    }
    out->L=NULL;
    out->U=NULL;
    out->p=NULL;

    /* Inicializamos L
     * Matriz identidad de tamaño nxn 
     */
    L = array2d_alloc(n,n);
    if(!L){
        printf("\n%s","No hay memoria suficiente para crear la matriz L.");
        free(out);
        return NULL;
    }
    for(size_t i=0;i<n;i++){
        L->data[i][i] = 1.0;
    }

    /* Inicializamos U
     * Copia de la matriz A
     */
    U = array2d_alloc(n,n);
    if(!U){
        printf("\n%s", "No hay memoria suficiente para crear la matriz U");
        freeArray2d(L); free(out);
        return NULL;
    }
    for(size_t i=0;i<n;i++){
        for(size_t j=0;j<n;j++){
            U->data[i][j] = A->data[i][j];
        }
    }

    /* Inicializamos p
     * Arreglo 1D donde registramos los intercambios de filas
     */
    p = array1d_alloc(n);
    if(!p){
        printf("\n%s","No hay memoria suficiente para crear el arreglo de permutacio'n p.");
        freeArray2d(L); freeArray2d(U); free(out);
        return NULL;
    }
    for(size_t i=0;i<n;i++){
        p->data[i]=i;
    }

    /* Bucle principal */
    for(size_t k=0;k<n-1;k++){
        /* Buscar pivote */
        int r;
        r = k;
        for (size_t i=k+1;i<n;i++) {
            if (fabs(U->data[i][k]) > fabs(U->data[r][k])) {
                r = i;
            }
        }

        /* Comprobar pivote */
        if(fabs(U->data[r][k]) < tol){
            out->L = L;
            out->U = U;
            out->p = p;
            out->res = SINGULAR;
            return out;
        }

        /* Intercambiar filas de U 
         * Registrar intercambio en p
         */
        if(r != k){
            for(size_t j=0;j<n;j++){
                double temp = U->data[r][j];
                U->data[r][j] = U->data[k][j];
                U->data[k][j] = temp;
            }
            int temp = p->data[r];
            p->data[r] = p->data[k];
            p->data[k] = temp;

            /* Intercambiar multiplicadores ya calculados */
            for(size_t j=0;j<k;j++){
                double temp = L->data[r][j];
                L->data[r][j] = L->data[k][j];
                L->data[k][j] = temp;
            }
        }

        /* Eliminación */
        for(size_t i=k+1;i<n;i++){
            L->data[i][k] = U->data[i][k] / U->data[k][k];
        }

        for(size_t i=k+1;i<n;i++){
            for(size_t j=k+1;j<n;j++){
                U->data[i][j] -= L->data[i][k] * U->data[k][j];
            }
        }
        
        for(size_t i=k+1;i<n;i++){
            U->data[i][k]=0;
        }
    }

    /* Comprobar último pivote */
    if (fabs(U->data[n-1][n-1]) < tol) {
        out->L = L;
        out->U = U;
        out->p = p;
        out->res = SINGULAR;
        return out;
    }

    /* Éxito */
    out->L = L;
    out->U = U;
    out->p = p;
    out->res = EXITO;
    return out;
}

Array1d *solveLU(Array2d *L, Array2d *U, Array1d *p, Array1d *b, double tol){

    Array1d *b_hat=NULL,*y=NULL,*x=NULL;
    size_t n = b->n;

    /* Vector de valores independientes reordenado 
     * b_hat es la representación de Pb
     */
    b_hat = array1d_alloc(n);
    if(!b_hat){
        printf("\n%s","Memoria insuficiente.");
        return NULL;
    }
    for(size_t i=0;i<n;i++){
        size_t idx = p->data[i];
        b_hat->data[i] = b->data[idx];
    }

    /* Sustitución hacia adelante para Ly = b_hat 
     * con Ax = y
     */
    y = forwardSubstitution(L,b_hat,tol);
    if(!y){
        printf("\n%s","El sistema no tiene solucio'n u'nica.");
        freeArray1d(b_hat);
        return NULL;
    }

    /* Sustitucion hacia atrás para Ux = y */
    x = backwardSubstitution(U,y,tol);
    if(!x){
        printf("\n%s","El sistema no tiene solucio'n u'nica.");
        freeArray1d(b_hat); freeArray1d(y);
        return NULL;
    }

    freeArray1d(b_hat); freeArray1d(y);
    return x;
}