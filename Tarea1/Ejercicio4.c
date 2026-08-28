#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "../raices/raices.h"
#include "../funciones/funciones_prueba.h"
#include "../epsilon/epsilon.h"

int main(void){
    double x;
    double puntos[3];
    int N = 100;
    double *result;
    
    result = malloc(4 * sizeof(double));
    
    x = 5;
    newton_r(result, f1, df1, x, N, sqrt(epsilon()));
    printf("\nf(x)    = sin(x)exp(0.5x) - 0.5x^2 + 6");
    printf("\nx_0     = %.2f",x);
    printf("\nf(x_0)  = %.2f",f1(x));
    printf("\nk       = %d",(int)result[2]);
    printf("\nx_k     = %.10e",result[0]);
    printf("\nf_k     = %.10e",result[1]);
    printf("\nres     = %d\n", (int)result[3]);

    
    puntos[0] = -1;
    puntos[1] = 1;
    puntos[2] = 3;
    for(int i=0;i<3;i++){
        x = puntos[i];
    
        newton_r(result, f2, df2, x, N, sqrt(epsilon()));
        printf("\nf(x)    = 4x^7 - 65x^5 + 94x^4 + 63x^3 - 122x^2 + 132x - 216");
        printf("\nx_0     = %.2f",x);
        printf("\nf(x_0)  = %.2f",f2(x));
        printf("\nk       = %d",(int)result[2]);
        printf("\nx_k     = %.10e",result[0]);
        printf("\nf_k     = %.10e",result[1]);
        printf("\nres     = %d\n", (int)result[3]);
    }

    puntos[0] = 0.5;
    puntos[1] = 2.0; // Para este punto la evaluación de la derivada (que es casi 0) ocasiona que x_k brinque cada vez más lejos.
    for(int i=0;i<2;i++){
        x = puntos[i];
    
        newton_r(result, f3, df3, x, N, sqrt(epsilon()));
        printf("\nf(x)    = arctan(x)");
        printf("\nx_0     = %.2f",x);
        printf("\nf(x_0)  = %.2f",f3(x));
        printf("\nk       = %d",(int)result[2]);
        printf("\nx_k     = %.10e",result[0]);
        printf("\nf_k     = %.10e",result[1]);
        printf("\nres     = %d\n", (int)result[3]);
    }
    
    free(result);

    return 0;
}