#include "potencia.h"
#include <math.h>


output *potencia(Array2d *A, size_t n, Array1d *vo, int N, double tol){

    output *resultado = malloc(sizeof *resultado);
    if(!resultado){
        return NULL;
    }

    // Definimos lambda
    double lambda;

    // Definimos y
    Array1d *y = NULL;

    // Definimos v
    Array1d *v = vo;

    // y = Av
    y = Av(A,v,n);
    if(!y){
        free(resultado);
        return NULL;
    }
    
    for(int k=1;k<=N;k++){

        // Normalización de y
        if(normalizacion(y,n)==EXITO){
            for(size_t i=0;i<n;i++){
                v->data[i] = y->data[i]; // v = y / ||y||
            }
        }
        // Si falla la normalización por división entre cero
        else{
            freeArray1d(y); free(resultado);
            return NULL;
        }

        freeArray1d(y); // Dado que que la función Av devuelve otro espacio de memoria, hay que liberar antes de "cachar" el nuevo espacio de memoria.
                        // Podemos liberar `y` antes de saber si `Av` tiene éxito,
                        // pues si no tiene éxito detenemos el método y no necesitamos conservar lo que valía `y` en la iteración actual.
        y = Av(A,v,n);  // y = Av
        if(!y){
            free(resultado);
            return NULL;
        }

        lambda = productoPunto(v,y,n); // lambda = < v , y >


        // Condición de paro
        Array1d *diff = NULL; // y - lambda v
        diff = diferencia(y,v,n,lambda);
        if(!diff){
            freeArray1d(y); free(resultado);
            return NULL;
        }
        if(norma2(diff,n)<tol){ // || y - lambda v || < tol
            freeArray1d(diff);
            freeArray1d(y);
            resultado->k = k;
            resultado->lambda = lambda;
            resultado->res = EXITO;
            resultado->v = v;
            return resultado;
        }
        freeArray1d(diff);

    }
    freeArray1d(y);
    resultado->k = N;
    resultado->lambda = lambda;
    resultado->res = FALLO;
    resultado->v = v;
    return resultado;
}

Array1d *Av(Array2d *A, Array1d *v, size_t n){

    Array1d *resultado = array1d_alloc(n);    // Ya está inicializado en ceros
    if(!resultado){
        return NULL;
    }

    // y_{i} = suma_{j} A_{i,j} * v_{j}
    for(size_t i=0;i<n;i++){
        for(size_t j=0;j<n;j++){
            resultado->data[i] += A->data[i][j] * v->data[j];
        }
    }
    return resultado;
}

Array1d *diferencia(Array1d *y, Array1d *v, size_t n, double lamnda){
    Array1d *diff = array1d_alloc(n);
    if(!diff){
        return NULL;
    }
    // x_{i} = y_{i} - lambda * v_{i}
    for(size_t i=0; i<n;i++){
        diff->data[i] = y->data[i] - lamnda * v->data[i];
    }
    return diff;
}

double productoPunto(Array1d *x, Array1d *y, size_t n){
    double suma=0;
    // suma_{i} x_{i} * y_{i}
    for(size_t i=0;i<n;i++){
        suma += x->data[i] * y->data[i];
    }
    return suma;
}

double norma2(Array1d *v, size_t n){
    double suma=0;
    // sqrt( suma_{i} x_{i} * y_{i} )
    for(size_t i=0;i<n;i++){
        suma += v->data[i] * v->data[i];
    }
    return sqrt(suma);
}

int normalizacion(Array1d *v, size_t n){
    double norma = norma2(v,n);

    // Cuidar división entre cero
    if(norma==0.0){return FALLO;}

    // v_{i} = v_{i} / ||v||
    for (size_t i = 0; i < n; i++){
        v->data[i] = v->data[i] / norma;
    }
    return EXITO;
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