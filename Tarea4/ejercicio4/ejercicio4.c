#include "potencia.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SALUDO printf("\nHola :D\n")

#define OK 1
#define ERROR_INPUT 2
#define ERROR_READ 4
#define ERROR_METHOD 8
#define ERROR_MEMORY 16

int main(int argc, char **argv){

    SALUDO;
    Array2d *A=NULL;
    output *resultado = NULL;

    /* Valores predeterminados */
    int N = 100;                            // Número de iteraciones
    double tol = pow(epsilon(),1.0/3.0);    // Tolerancia

    /*
     * Argumentos:
     *
     * argv[1] -> archivo de matriz A
     * argv[2] -> N   (opcional)
     * argv[3] -> tol (opcional)
     */

    if(argc < 2 || argc > 4){
        printf("\nUso:");
        printf("\n%s archivo_A [N] [tol]\n", argv[0]);
        return ERROR_INPUT;
    }

    /* N */
    if(argc >= 3){

        char *endptr;

        N = (int)strtol(argv[2], &endptr, 10);

        if(*endptr != '\0' || N <= 0){
            printf("\nError: N debe ser un entero positivo.\n");
            return ERROR_INPUT;
        }
    }

    /* tol */
    if(argc == 4){

        char *endptr;

        tol = strtod(argv[3], &endptr);

        if(*endptr != '\0' || tol <= 0){
            printf("\nError: tol debe ser un numero positivo.\n");
            return ERROR_INPUT;
        }
    }

    printf("\nParametros:");
    printf("\nN   = %d", N);
    printf("\ntol = %.3e\n", tol);
    
    printf("\n... Reading matriz A ...\n");
    A = readArray2d(argv[1]);
    if(!A){
        return ERROR_READ;
    }

    size_t n = A->rows; // Tamaño de la matriz nxn
    printf("\nLa matriz tiene <%zu> filas y columnas.\n",n);

    // Creación vector inicial de 1's
    printf("\n... Creando vector inicial de 1's ...\n");
    Array1d *v = array1d_alloc(n);  // Inicializado en ceros
    if(!v){
        freeArray2d(A);
        return ERROR_MEMORY;
    }
    for(size_t i=0;i<n;i++){
        v->data[i]=1.0; // v con entradas igual a 1
    }

    // Método de la potencia
    // OJO La función potencia modifica in-palce al vector v
    // resultado->v y v apuntarán al mismo espacio de memoria.
    printf("\n... Me'todo de la potencia ...\n");
    resultado = potencia(A, n, v, N, tol);
    if(!resultado){
        freeArray1d(v);
        freeArray2d(A);
        return ERROR_METHOD;
    }

    if(resultado->res==EXITO){
        printf("\nEl me'todo SI converge.\n");
    }
    else{
        printf("\nEl me'todo NO converge.\n");
    }

    // Solución
    printf("\nEigenvector = ");
    printArray1d(resultado->v, "% 6.2f ", 3);  // resultado->v y v apuntan al mismo espacio de memoria

    // || Av - lambda v ||
    // Obtenemos el resultado de Av
    Array1d *res_Av = Av(A,resultado->v,n); // resultado->v y v apuntan al mismo espacio de memoria
    if(!res_Av){
        freeArray2d(A);
        freeArray1d(v); // resultado->v y v apuntan al mismo espacio de memoria
        free(resultado);
        return ERROR_MEMORY;
    }
    // Obtenemos el resultado de la diferencia Av - lambda v
    double lambda = resultado->lambda;
    Array1d *diff = diferencia(res_Av,resultado->v,n,lambda); // resultado->v y v apuntan al mismo espacio de memoria
    if(!diff){
        freeArray2d(A);
        freeArray1d(v); // resultado->v y v apuntan al mismo espacio de memoria
        freeArray1d(res_Av);
        free(resultado);
        return ERROR_MEMORY;
    }
    // Imprimir resultado || Av - lambda v ||
    printf("\n|| Av - lambda v ||= %.10e\n",norma2(diff,n));
    freeArray1d(res_Av);
    freeArray1d(diff);

    printf("\nNu'mero de iteraciones = <%d>\n",resultado->k);

    freeArray2d(A);
    freeArray1d(v); // resultado->v y v apuntan al mismo espacio de memoria
    free(resultado);

}
