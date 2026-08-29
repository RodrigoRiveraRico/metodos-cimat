
#include <stdio.h>
#include <math.h>
#include "raices.h"

// Que el primer argumento sea un apuntador fue por necesidad de la tarea.
// No hace falta implementar un apuntador para el algoritmo.
int biseccion(double *p, double (*func)(double), double a, double b, int N, double tol){

    double a_k, b_k, x_k;
    int res = 0;

    a_k = a;
    b_k = b;

    // Se revisa si los extremos del intervalo son solución
    if(fabs(func(a_k)) < tol){
        x_k = a_k;
        res = 1;

        p[0] = x_k;
        p[1] = func(x_k);
        p[2] = 0;
        p[3] = a_k;
        p[4] = b_k;
        p[5] = res;
        // printf("\n%f %e %d %f %f %d",x_k,func(x_k),0,a_k,b_k,res);

        return res;
    }
    if(fabs(func(b_k)) < tol){
        x_k = b_k;
        res = 1;

        p[0] = x_k;
        p[1] = func(x_k);
        p[2] = 0;
        p[3] = a_k;
        p[4] = b_k;
        p[5] = res;
        // printf("\n%f %e %d %f %f %d",x_k,func(x_k),0,a_k,b_k,res);

        return res;
    }

    // Método de bisección
    // Si no son raices, se genera la secuencia x_k de puntos
    for(int k=0;k<N+1;k++){
        x_k = (a_k + b_k)/2.0;

        if(fabs(func(x_k)) < tol){
            res = 1;

            p[0] = x_k;
            p[1] = func(x_k);
            p[2] = k;
            p[3] = a_k;
            p[4] = b_k;
            p[5] = res;
            // printf("\n%f %e %d %f %f %d",x_k,func(x_k),k,a_k,b_k,res);

            return res;
        }
        
        if(func(a_k)*func(x_k)<0){
            b_k = x_k;
        }
        else{
            a_k = x_k;
        }
    }
    // No se encontró la raíz dentro de N iteraciones
    p[0] = x_k;
    p[1] = func(x_k);
    p[2] = N;
    p[3] = a_k;
    p[4] = b_k;
    p[5] = res;    
    // printf("\n%f %e %d %f %f %d",x_k,func(x_k),N,a_k,b_k,res);

    return res;
}

int newton_r(double *p, double (*func)(double), double (*dfunc)(double), double x, int N, double tol){

    double x_k = x;
    double raiz;
    double f_k = func(x_k);
    double df_k;

    int res = 0;

    for(int k=1;k<N;k++){

        // Si hay divergencia
        if(isnan(x_k)){
            p[0] = x_k;
            p[1] = f_k;
            p[2] = k;
            p[3] = res;

            return res;
        }

        if(fabs(f_k) < tol){
            raiz = x_k;
            res = 1;

            p[0] = raiz;
            p[1] = f_k;
            p[2] = k;
            p[3] = res;

            return res;
        }

        df_k = dfunc(x_k);
        x_k = x_k - f_k/df_k;
        f_k = func(x_k);
    }
    p[0] = x_k;
    p[1] = f_k;
    p[2] = N;
    p[3] = res;

    return res;
}
