#include "gauss_seidel.h"
#include <math.h>

Array1d *AxTridiagonal(Array2d *A, Array1d *x){

    size_t n = A->rows; // Matriz cuadrada
    Array1d *v = array1d_alloc(n);
    if(!v){
        return NULL;
    }

    // Comoponente i=0
    v->data[0] = A->data[0][0] * x->data[0] + A->data[0][1] * x->data[1];

    // Componentes 1 <= i < n-1
    for(int i=1;i<n-1;i++){
        v->data[i] = A->data[i][i-1] * x->data[i-1] + A->data[i][i] * x->data[i] + A->data[i][i+1] * x->data[i+1];
    }

    // Componente i=n-1
    v->data[n-1] = A->data[n-1][n-2] * x->data[n-2] + A->data[n-1][n-1] * x->data[n-1];

    return v;
}

output *solveTridiagonal(Array2d *A, Array1d *b, size_t n, Array1d *xo, int N, double tol){

    output *resultado = malloc(sizeof *resultado);
    if(!resultado){
        return NULL;
    }

    Array1d *x = xo;
    if(!x){
        free(resultado);
        return NULL;
    }

    Array1d *residual = array1d_alloc(n);
    if(!residual){
        free(resultado);
        return NULL;
    }
    Array1d *v = AxTridiagonal(A,x);
    if(!v){
        freeArray1d(residual); free(resultado);
        return NULL;
    }
    for(size_t i=0;i<n;i++){
        residual->data[i] = v->data[i] - b->data[i];
    }

    if(fabs(errResidual(residual,n))<tol){
            resultado->r = residual;
            resultado->res = EXITO;
            resultado->t = 0;
            resultado->x = x;

            freeArray1d(v);
            return resultado;
    }

    for(int t=1;t<=N;t++){
        
        // Para i=0
        x->data[0] = (b->data[0] - A->data[0][1] * x->data[1]) / A->data[0][0];

        // Para 1 <= i < n-1
        for(size_t i=1;i<n-1;i++){
            x->data[i] = (b->data[i] - A->data[i][i-1] * x->data[i-1] - A->data[i][i+1] * x->data[i+1]) / A->data[i][i];
        }

        // Para i=n-1
        x->data[n-1] = (b->data[n-1] - A->data[n-1][n-2] * x->data[n-2]) / A->data[n-1][n-1];

        // Cálculo de residual
        freeArray1d(v);
        v = AxTridiagonal(A,x);
        for(size_t i=0;i<n;i++){
           residual->data[i] = v->data[i] - b->data[i];
        }

        if(fabs(errResidual(residual,n))<tol){
            resultado->r = residual;
            resultado->res = EXITO;
            resultado->t = t;
            resultado->x = x;

            freeArray1d(v);
            return resultado;
        }

    }
    resultado->r = residual;
    resultado->res = FALLO;
    resultado->t = N;
    resultado->x = x;

    freeArray1d(v);
    return resultado;
}

double errResidual(Array1d *r, size_t n){
    double suma=0;
    for(size_t i=0;i<n;i++){
        suma += r->data[i] * r->data[i];
    }
    return sqrt(suma);
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