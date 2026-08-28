#include <stdio.h>
#include "../epsilon/epsilon.h"

int main(void){

    double eps;

    eps = epsilon();

    printf("\nEpsilon de la maquina: %.20e",eps);

    printf("\nResultado tabla:");
    tabla(eps);

    return 0;
}