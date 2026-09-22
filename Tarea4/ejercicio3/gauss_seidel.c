#include "gauss_seidel.h"
#include <math.h>

Array1d *AxTridiagonal(Array2d *A, Array1d *x){

    size_t n = A->rows; // Matriz cuadrada
    Array1d *v = array1d_alloc(n); // Resultado del producto Ax
    if(!v){
        return NULL;
    }
    // Para una matriz A tridiagonal:
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

    Array1d *x = xo;    // Vector solución. Aquí se guardan las soluciones iterativas.
                        // Inicializado con el vector inicial.
    if(!x){
        free(resultado);
        return NULL;
    }

    Array1d *residual = array1d_alloc(n);   // Vector residual = Ax - b
    if(!residual){
        free(resultado);
        return NULL;
    }
    Array1d *v = AxTridiagonal(A,x);    // Ax = v con A tridiagonal
    if(!v){
        freeArray1d(residual); free(resultado);
        return NULL;
    }
    // Actualización r_{i} = v_{i} - b_{i}
    for(size_t i=0;i<n;i++){
        residual->data[i] = v->data[i] - b->data[i];
    }

    // Condición de paro antes de ejecutar el método (cero iteraciones)
    if(fabs(norma2(residual,n))<tol){
            resultado->r = residual;
            resultado->res = EXITO;
            resultado->t = 0;
            resultado->x = x;

            freeArray1d(v);
            return resultado;
    }

    // Iteraciones del método
    for(int t=1;t<=N;t++){
        
        // Para una matriz A tridiagonal:
        // Para i=0
        x->data[0] = (b->data[0] - A->data[0][1] * x->data[1]) / A->data[0][0];

        // Para 1 <= i < n-1
        for(size_t i=1;i<n-1;i++){
            x->data[i] = (b->data[i] - A->data[i][i-1] * x->data[i-1] - A->data[i][i+1] * x->data[i+1]) / A->data[i][i];
        }

        // Para i=n-1
        x->data[n-1] = (b->data[n-1] - A->data[n-1][n-2] * x->data[n-2]) / A->data[n-1][n-1];

        // Cálculo de residual
        freeArray1d(v); // Liberación de v antes de guardar nueva dirección de memoria
        v = AxTridiagonal(A,x); // Ax = v con A tridiagonal
        // Actualización vector residual
        for(size_t i=0;i<n;i++){
           residual->data[i] = v->data[i] - b->data[i];
        }

        // Condición de paro (t iteraciones)
        if(fabs(norma2(residual,n))<tol){
            resultado->r = residual;
            resultado->res = EXITO;
            resultado->t = t;
            resultado->x = x;

            freeArray1d(v);
            return resultado;
        }

    }
    // Paro por máximo de iteraciones realizadas
    resultado->r = residual;
    resultado->res = FALLO;
    resultado->t = N;
    resultado->x = x;

    freeArray1d(v);
    return resultado;
}

double norma2(Array1d *r, size_t n){
    // Norma 2 del vector r
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