#include <stdio.h>
#include <stdlib.h>

/////////////////////////////////////////////////////////

typedef struct {
    double **data;
    int n;
}tabla;


/////////////////////////////////////////////////////////

tabla *crearTabla(int n);
void llenarTabla(tabla *tabla, double *arreglo_x, double *arreglo_y, int n);
double evalPol(tabla *tabla, double *arreglo_x, int n, double x);
void freeTabla(tabla *tabla);
void printfTabla(tabla *tabla);


/////////////////////////////////////////////////////////

int main(void){

    /********************************************************/

    int    n         = 4;
    double nodos[]   = {1.0, 3.0, 4.0, 8.0};    // Puntos en x
    double valores[] = {2.0, 3.0, 2.0, 10.0};   // Puntos en y

    /********************************************************/

    tabla *diferencias = crearTabla(n);
    if (!diferencias) {
        printf("Error al asignar memoria.\n");
        return 1;
    }
    llenarTabla(diferencias, nodos, valores, n);
    printfTabla(diferencias);

    double p = 10.0;
    double eval = evalPol(diferencias, nodos, n, p);
    printf("%f ", eval);


    freeTabla(diferencias);
    return 0;
}

tabla *crearTabla(int n){
    tabla  *new_tabla = malloc(sizeof *new_tabla);
    if(!new_tabla){
        return NULL;
    }
    new_tabla->n = n;
    new_tabla->data = malloc(n * sizeof *new_tabla->data);
    if(!new_tabla->data){
        free(new_tabla);
        return NULL;
    }

    for(int i=0;i<n;i++){
        // Reservamos espacio de forma triangular: fila i tiene (i+1) columnas
        new_tabla->data[i] = calloc((i+1), sizeof(double));
        if(!new_tabla->data[i]){
            for(int j=0;j<i;j++){
                free(new_tabla->data[j]);
            }
            free(new_tabla->data);
            free(new_tabla);
            return NULL;
        }
    }
    return new_tabla;
}

void llenarTabla(tabla *tabla, double *arreglo_x, double *arreglo_y, int n){

    // 1. Colocar las condiciones iniciales (columna 0)
    for (int i = 0; i < n; i++)
    {
        tabla->data[i][0] = arreglo_y[i];
    }

    // 2. Calcular las diferencias divididas (columnas > 0)
    // j representa la columna (orden de la diferencia)
    // i representa la fila
    for (int j = 1; j < n; j++)
    {
        for (int i = j; i < n; i++)
        {
            double numerador = tabla->data[i][j - 1] - tabla->data[i - 1][j - 1];
            double denominador = arreglo_x[i] - arreglo_x[i - j];
            tabla->data[i][j] = numerador / denominador;
        }
    }
}

double evalPol(tabla *tabla, double *arreglo_x, int n, double x){

    // Ciclo de la suma
    double suma = tabla->data[0][0];    // F[0,0]
    for(int i=1;i<n;i++){
        // Ciclo del producto
        double producto = 1;
        for(int j=0;j<i;j++){
            producto *= (x - arreglo_x[j]);
        }
        producto *= tabla->data[i][i];
        suma += producto;
        printf("%f ",suma); // Al final se está haciendo cero, ¿por qué?
    }
}

void freeTabla(tabla *tabla){
    if(!tabla) return;
    for(int i=0; i<tabla->n; i++){
        free(tabla->data[i]);
    }
    free(tabla->data);
    free(tabla);
}

void printfTabla(tabla *tabla){
    printf("--- Tabla de Diferencias Divididas ---\n");
    for(int i=0; i<tabla->n; i++){
        for(int j=0; j<=i; j++){
            printf("%10.4f ", tabla->data[i][j]);
        }
        printf("\n");
    }
}