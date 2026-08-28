#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "../raices/raices.h"
#include "../funciones/funciones_prueba.h"
#include "../epsilon/epsilon.h"

int main(void){
    double a, b;
    int N;
    double *result;
    
    result = malloc(6 * sizeof(double));

    a = -4;
    b = 8;
    N = 30;
    biseccion(result, f1, a, b, N, sqrt(epsilon()));
    printf("\nf(x)    = sin(x)exp(0.5x) - 0.5x^2 + 6");
    printf("\na       = %.2f", a);
    printf("\nb       = %.2f", b);
    printf("\nf(a)    = %.2f", f1(a));
    printf("\nf(b)    = %.2f", f1(b));
    printf("\nk       = %d", (int)result[2]);
    printf("\nx_k     = %.10f", result[0]);
    printf("\nf(x_k)  = %.10e", result[1]);
    printf("\nres     = %d\n", (int)result[5]);

    a = -1;
    b = 3;
    N = 30;
    biseccion(result, f2, a, b, N, sqrt(epsilon()));
    printf("\n\nf(x)  = 4x^7 - 65x^5 + 94x^4 + 63x^3 - 122x^2 + 132x - 216");
    printf("\na       = %.2f", a);
    printf("\nb       = %.2f", b);
    printf("\nf(a)    = %.2f", f2(a));
    printf("\nf(b)    = %.2f", f2(b));
    printf("\nk       = %d", (int)result[2]);
    printf("\nx_k     = %.10f", result[0]);
    printf("\nf(x_k)  = %.10e", result[1]);
    printf("\nres     = %d\n", (int)result[5]);

    a = -3;
    b = 3;
    N = 30;
    biseccion(result, f3, a, b, N, sqrt(epsilon()));
    printf("\n\nf(x)  = arctan(x)");
    printf("\na       = %.2f", a);
    printf("\nb       = %.2f", b);
    printf("\nf(a)    = %.2f", f3(a));
    printf("\nf(b)    = %.2f", f3(b));
    printf("\nk       = %d", (int)result[2]);
    printf("\nx_k     = %.10f", result[0]);
    printf("\nf(x_k)  = %.10e", result[1]);
    printf("\nres     = %d\n", (int)result[5]);

    free(result);

    return 0;
}