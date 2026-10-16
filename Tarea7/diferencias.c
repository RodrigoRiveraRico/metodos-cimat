
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/////////////////////////////////////////////////////////

typedef struct {
    double **data;
    int n;
}tabla;


/////////////////////////////////////////////////////////

double **crearTabla(int n);
void llenarTabla(double **data, double *arreglo_x, double *arreglo_y, int n);
double llenarTablaRec(double **data, double *arreglo_x, double *arreglo_y, int n, int m);
/** 
 * @brief Función que hace la evaluación en el polinomio de interpolación del método de diferencias divididas. 
 * 
 * La evaluación se hace con el método de Horner.
 * @param tabla Tabla con las diferencias dividas dispuestas de forma triangular inferior.
 * @param arreglo_x
 * @param n int
 * @param x double
 * @return double La evaluación P(x)
 */
double evalPol(double **data, double *arreglo_x, int n, double x);
void freeTabla(double **data, int n);
void printfTabla(double **data, int n);
double errorAbs(double real, double aprox);
void tablaResultados(double x, double (*f)(double), double eval_pol, double error, double cota);

/////////////////////////////////////////////////////////

int main(void){

    /********************************************************/
/*   
    int    n         = 4;
    double nodos[]   = {1.0, 3.0, 4.0, 8.0};    // Puntos en x
    double valores[] = {2.0, 3.0, 2.0, 10.0};   // Puntos en y

    int    n         = 5;
    double nodos[]   = {1.0, 1.3, 1.6, 1.9, 2.2};    // Puntos en x
    double valores[] = {0.7651977, 0.6200860, 0.4554022, 0.2818186, 0.1103623};   // Puntos en y
*/
       
    int    n         = 5;
    double nodos[]   = {0.0, 1.0/4.0, 1.0/2.0, 3.0/4.0, 1.0};    // Puntos en x
    double valores[] = {exp(0.0), exp(1.0/4.0), exp(1.0/2.0), exp(3.0/4.0), exp(1.0)};   // Puntos en y

    /********************************************************/

    tabla diferencias;
    diferencias.n = n;
    diferencias.data = crearTabla(n);

    if (!diferencias.data) {
        printf("Error al asignar memoria.\n");
        return 1;
    }

    /*
    llenarTabla(diferencias.data, nodos, valores, n);
    printfTabla(diferencias.data, n);
    */
    llenarTablaRec(diferencias.data, nodos, valores, n-1, n-1);
    printfTabla(diferencias.data, n);

    // double p_x = 1.5;
    double puntos[] = {0.10, 0.35, 0.60, 0.90};
    for(int i=0;i<4;i++){
        double eval = evalPol(diferencias.data, nodos, n, puntos[i]);
        double error_abs = errorAbs(exp(puntos[i]), eval);

        tablaResultados(puntos[i], exp, eval, error_abs, -1);
    }
    
    freeTabla(diferencias.data, diferencias.n);
    return 0;
}

/***********************************************************
/     Funciones para el método de diferencias divididas    /
***********************************************************/

double **crearTabla(int n){
    double **data = malloc(n * sizeof *data);
    if(!data){
        return NULL;
    }

    for(int i=0;i<n;i++){
        // Reservamos espacio de forma triangular: fila i tiene (i+1) columnas
        data[i] = calloc((i+1), sizeof(double));
        if(!data[i]){
            for(int j=0;j<i;j++){
                free(data[j]);
            }
            free(data);
            return NULL;
        }
    }
    return data;
}

void llenarTabla(double **data, double *arreglo_x, double *arreglo_y, int n){

    // 1. Colocar las condiciones iniciales (columna 0)
    for (int i = 0; i < n; i++)
    {
        data[i][0] = arreglo_y[i];
    }

    // 2. Calcular las diferencias divididas (columnas > 0)
    // j representa la columna (orden de la diferencia)
    // i representa la fila
    for (int j = 1; j < n; j++)
    {
        for (int i = j; i < n; i++)
        {
            double numerador = data[i][j - 1] - data[i - 1][j - 1];
            double denominador = arreglo_x[i] - arreglo_x[i - j];
            data[i][j] = numerador / denominador;   // Division entre cero?
        }
    }
}

double llenarTablaRec(double **data, double *arreglo_x, double *arreglo_y, int n, int m){

    // Caso base
    // Primera columna
    if(m==0){
        data[n][m] = arreglo_y[n];
        return arreglo_y[n];
    }

    // Recursividad
    double izq = llenarTablaRec(data, arreglo_x, arreglo_y, n, m-1);
    double der = llenarTablaRec(data, arreglo_x, arreglo_y, n-1, m-1);
    double divisor = arreglo_x[n] - arreglo_x[n-m];

    double coef = (izq - der) / divisor;
    
    data[n][m] = coef;

    return coef;
}

double evalPol(double **data, double *arreglo_x, int n, double x){
    double eval = data[n-1][n-1];
    for(int i = n-2; i >= 0; i--){
        eval = eval * (x - arreglo_x[i]) + data[i][i];
    }
    return eval;
}   

void freeTabla(double **data, int n){
    if(!data) return;
    for(int i=0; i<n; i++){
        free(data[i]);
    }
    free(data);
}

void printfTabla(double **data, int n){
    printf("--- Tabla de Diferencias Divididas ---\n");
    for(int i=0; i<n; i++){
        for(int j=0; j<=i; j++){
            printf("%14.4e ", data[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

double errorAbs(double real, double aprox){
    return fabs(real-aprox);
}


/***********************************************************
/            Funciones para probar el método               /
***********************************************************/
void tablaResultados(double x, double (*f)(double), double eval_pol, double error, double cota){
    static int flag = 0;
    // Encabezado
    if(flag==0){
        printf("\n--------------------------------\n");
        printf(" x | f(x) | P(x) |  |f(x) - P(x)|  |  cota \n");
        flag = 1;
    }
    // Cuerpo
    printf("%.4e   %.4e   %.4e   %.4e   %.4e \n", x, f(x), eval_pol, error, cota);
}