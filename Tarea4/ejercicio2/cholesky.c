#include <math.h>
#include <stdio.h>
#include "cholesky.h"

Array2d *cholesky(Array2d *A, size_t n){

    Array2d *L = array2d_alloc(n,n);
    if(!L){return NULL;}

    double suma_diagonal, suma_cruzada;

    for(int j=0;j<n;j++){

        suma_diagonal = 0;
        for(int k=0;k<j;k++){
            suma_diagonal += L->data[j][k] * L->data[j][k];
        }
        suma_diagonal = A->data[j][j] - suma_diagonal;
        if(suma_diagonal<=0){
            printf("\nMatriz no es definida positiva.");
            freeArray2d(L);
            return NULL;
        }
        
        L->data[j][j] = sqrt(suma_diagonal);

        for(int i=j+1;i<n;i++){

            suma_cruzada = 0;
            for(int k=0;k<j;k++){
                suma_cruzada += L->data[i][k] * L->data[j][k];
            }
            suma_cruzada = A->data[i][j] - suma_cruzada;
            L->data[i][j] = suma_cruzada / L->data[j][j];
        }
    }
    return L;    
}

Array1d *solveLLT(Array2d *L, Array1d *b, size_t n){

    Array2d *LT = transpose_cuadrada(L, n);
    if(!LT) {return NULL;}

    double tol = epsilon();
    Array1d *y = forwardSubstitution(L, b, tol);
    if(!y){
        freeArray2d(LT);
        return NULL;
    }
    Array1d *x = backwardSubstitution(LT,y,tol);
    
    freeArray1d(y);
    freeArray2d(LT);
    if(!x){
        return NULL;
    }

    return x;
}

Array2d *transpose_cuadrada(Array2d *matriz, size_t n){
    
    Array2d *transpuesta = array2d_alloc(n, n);
    if(!transpuesta){return NULL;}

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            transpuesta->data[j][i] = matriz->data[i][j];
        }
    }

    return transpuesta;
}

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

double frobenius(Array2d *A, Array2d *B){

    if(A->cols!=B->cols || A->rows!=B->rows){
        return -1;
    }

    size_t n = A->rows;
    size_t m = A->cols;

    double coef;
    double suma=0;
    for(size_t i=0;i<n;i++){
        for(size_t j=0;j<m;j++){
            coef = A->data[i][j] - B->data[i][j];
            suma += coef * coef;
        }
    }
    return sqrt(suma);

}

double errorResidual(Array2d *A, Array1d *x, Array1d *b){

    // Vector residual = Ax-b
    double r_i; /**< Entrada i-ésima del vector residual */
    double err_residual = 0;

    for(size_t i=0;i<A->rows;i++){
        r_i = 0;
        for(size_t j=0;j<A->cols;j++){
            r_i += A->data[i][j] * x->data[j];
        }
        r_i -= b->data[i];
        err_residual += r_i*r_i;
    }

    err_residual = sqrt(err_residual);

    return err_residual;
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