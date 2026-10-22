
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 * @brief Función que inicializa un arreglo 2D triangular inferior para almacenar las diferencias divididas
 * 
 * @param n int Número de filas
 * @return double** Arreglo 2D triangular inferior
 */
double **crearTabla(int n);

/**
 * @brief Función que llena un arreglo 2D triangular inferior con las dierencias divididas
 * 
 * @param data double** Arreglo 2D triangular inferior. Aquí es donde se guardaran las diferencias divididas.
 * @param arreglo_x double* Nodos
 * @param arreglo_y double* Coordenada y de los nodos
 * @param n int Número de nodos
 */
void llenarTabla(double **data, double *arreglo_x, double *arreglo_y, int n);

/**
 * @brief Función recursiva que llena un arreglo 2D triangular inferior con las dierencias divididas
 * 
 * @param data double** Arreglo 2D triangular inferior. Aquí es donde se guardaran las diferencias divididas.
 * @param arreglo_x double* Nodos
 * @param arreglo_y double* Coordenada y de los nodos
 * @param n int Número de nodos - 1
 * @param m int Número de nodos - 1
 */
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

/**
 * @brief Función para liberar memoria
 * 
 * @param data double**
 * @param n int Número de filas
 */
void freeTabla(double **data, int n);

/**
 * @brief Función para imprimir la tabla de diferencias divididas
 * 
 * @param data double** Arreglo 2D triangular inferior
 * @param n int Número de columnas
 */
void printfTabla(double **data, int n);

/**
 * @brief Fucnión que calcula el error absoluto
 * 
 * @param real double Valor real
 * @param aprox double Valor aproximado
 * @return double Error absoluto
 */
double errorAbs(double real, double aprox);

/**
 * @brief Calcula una cota superior del error de interpolación
 *
 * Supone que f(x) = exp(x), que los nodos están en [0,1]
 * y que x también pertenece a [0,1].
 *
 * @param arreglo_x Nodos de interpolación
 * @param n Número de nodos
 * @param x Punto de evaluación
 * @return double Cota superior del error absoluto
 */
double cotaError(double *arreglo_x, int n, double x);

/**
 * @brief Función para imprimir una tabla de resultados
 *
 * @param x double Punto a evaluar
 * @param f double (*f)(double) Apuntador a función
 * @param eval_pol double Evaluación en el polinomio
 * @param error double Error absoluto
 * @param cota double Cota teórica
 */
void tablaResultados(double x, double (*f)(double), double eval_pol, double error, double cota);

/**
 * @brief p(x) = x^3 - 2x^2 + 5
 */
double polinomio(double x);

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
    double nodos[]   = {0.0, 1.0/4.0, 1.0/2.0, 3.0/4.0, 1.0};                            // Puntos en x
    double valores[] = {exp(0.0), exp(1.0/4.0), exp(1.0/2.0), exp(3.0/4.0), exp(1.0)};   // Puntos en y
    double puntos[]  = {0.10, 0.35, 0.60, 0.90};                                         // Punto a evaluar

/*    
    int    n         = 4;
    double nodos[]   = {-1.0, 0.0, 1.0, 2.0};    // Puntos en x
    double valores[] = {polinomio(-1.0), polinomio(0.0), polinomio(1.0), polinomio(2.0)};   // Puntos en y
    double puntos[]  = {0.5};
    
*/  
    /********************************************************/

    double **diferencias = crearTabla(n);
    if (!diferencias) {
        printf("Error al asignar memoria.\n");
        return 1;
    }

    /*
    double  **diferencias_iter = crearTabla(n);
    if (!diferencias_iter) {
        printf("Error al asignar memoria.\n");
        return 1;
    }
    printf("Diferencias divididas (iterativo)\n");
    llenarTabla(diferencias_iter, nodos, valores, n);
    printfTabla(diferencias_iter, n);
    freeTabla(diferencias_iter, n);
    */
   
    printf("Diferencias divididas (recursivo)\n");
    llenarTablaRec(diferencias, nodos, valores, n-1, n-1);
    printfTabla(diferencias, n);

    for(int i=0;i<4;i++){
        double eval = evalPol(diferencias, nodos, n, puntos[i]);

        double error_abs = errorAbs(exp(puntos[i]), eval);

        double cota = cotaError(nodos, n, puntos[i]);

        tablaResultados(puntos[i], exp, eval, error_abs, cota);
    }
    
    freeTabla(diferencias, n);
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
            if(denominador == 0){
                printf("!! División entre cero.\n");
                return;
            }
            data[i][j] = numerador / denominador;
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
    double coef;
    if(divisor==0){ // Una mejor implementación sería revisar que no haya nodos iguales antes de aplicar recursividad??
        printf("!! División entre cero\n");
        coef = 0;
    }
    else{
        coef = (izq - der) / divisor;
    }

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

double cotaError(double *arreglo_x, int n, double x){
    double producto = 1.0;
    double factorial = 1.0;

    for (int i = 0; i < n; i++) {
        producto *= (x - arreglo_x[i]);
    }

    for (int i = 2; i <= n; i++) {
        factorial *= i;
    }

    double M = exp(1.0);

    return (M / factorial) * fabs(producto);
}


/***********************************************************
/            Funciones para probar el método               /
***********************************************************/
void tablaResultados(double x, double (*f)(double), double eval_pol, double error, double cota){
    static int flag = 0;
    // Encabezado
    if(flag==0){
        printf("\n--------------------------------\n");
        printf("     x      |     f(x)     |     P(x)     | |f(x)-P(x)| |    cota \n");
        flag = 1;
    }
    // Cuerpo
    printf("%.4e     %.4e     %.4e     %.4e     %.4e \n", x, f(x), eval_pol, error, cota);
}

double polinomio(double x){
    return x*x*x - 2*x*x + 5;
}