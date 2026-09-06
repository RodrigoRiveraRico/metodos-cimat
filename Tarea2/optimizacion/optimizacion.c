#include <math.h>
#include "optimizacion.h"

int dorada(double *resultados, double (*f)(double), double l, double u, double tol, int N){

    double x_k, x_1, x_3, b;
    double x_l = l, x_u = u;
    double rho = (sqrt(5.0)-1.0)/2.0;
    int res;
    
    for(int k=1;k<=N;k++){
        if(x_u - x_l < tol){
            x_k = f(x_l) < f(x_u) ? x_l : x_u;
            res = 0; // Se cumple tol
            
            resultados[0] = x_k;
            resultados[1] = x_l;
            resultados[2] = x_u;
            resultados[3] = k-1;
            resultados[4] = res;

            return res;
        }

        b = rho*(x_u - x_l);
        x_1 = x_u - b;
        x_3 = x_l + b;

        if(f(x_1) < f(x_3)){
            x_u = x_3;
            x_k = x_1;
        }
        else{
            x_l = x_1;
            x_k = x_3;
        }
    }
    res = 1;    // No se cumple tol

    resultados[0] = x_k;
    resultados[1] = x_l;
    resultados[2] = x_u;
    resultados[3] = N;
    resultados[4] = res;

    return res;
}

int newton(double *resultados, double (*df)(double), double (*ddf)(double), double x, double tol, int N){

    double x_k = x;
    double raiz;
    double df_k = df(x_k);
    double ddf_k;

    int res;

    for(int k=1;k<=N;k++){

        // Si hay divergencia
        if(isnan(x_k)){
            res = 1;    // No se cumple tol

            resultados[0] = x_k;
            resultados[1] = df_k;
            resultados[2] = k-1;
            resultados[3] = res;

            return res;
        }

        if(fabs(df_k) < tol){
            raiz = x_k;
            res = 0;    // Se cumple tol

            resultados[0] = raiz;
            resultados[1] = df_k;
            resultados[2] = k-1;
            resultados[3] = res;

            return res;
        }

        ddf_k = ddf(x_k);
        x_k = x_k - df_k/ddf_k;
        df_k = df(x_k);
    }
    res = 1; // No se cumple tol

    resultados[0] = x_k;
    resultados[1] = df_k;
    resultados[2] = N;
    resultados[3] = res;

    return res;
}
