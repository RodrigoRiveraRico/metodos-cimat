#include <stdio.h>
#include <math.h>
#include "epsilon/epsilon.h"

int main(void){

    double eps;

    eps = epsilon();

    printf("\nEpsilon de la maquina: %.20e",eps);

    printf("\nResultado tabla:\n");

    double a[9], b[9];
    int c[9];

    a[0] = 1.0 + eps;
    a[1] = 1.0 - eps/2.0;
    a[2] = 1.0 + eps/2.0;
    a[3] = 1.0 - eps/4.0;
    a[4] = 1.0 - eps/2.0;
    a[5] = eps/2.0;
    a[6] = pow(eps,5);
    a[7] = eps + pow(eps,2);
    a[8] = eps + pow(eps,3);

    b[0] = 1.0;
    b[1] = 1.0;
    b[2] = 1.0;
    b[3] = 1.0;
    b[4] = 1.0 - eps;
    b[5] = 0.0;
    b[6] = 0.0;
    b[7] = eps;
    b[8] = eps;

    for(int i=0;i<9;i++){
        c[i] = (a[i] == b[i]);
    }


    printf("%8s %28s %28s\n", "a_i", "b_i", "a_i==b_i");

    for (int i = 0; i < 9; i++) {
        printf("%.20e %28.20e %7d\n", a[i], b[i], c[i]);
    }

    // printf("a_i b_i a_i==b_i");
    // for(int i=0;i<9;i++){
    //     printf("\n%e %e %d",a[i],b[i],c[i]);
    // }

    return 0;
}