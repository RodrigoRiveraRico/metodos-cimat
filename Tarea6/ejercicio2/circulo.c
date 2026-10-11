#include "circulo.h"
#include "../cholesky/cholesky.h"
#include "../ejercicio1/polinomio.h"

#include <math.h>

double promedio(Array1d *arreglo, int m){
    double sum = 0;
    for(int i=0;i<m;i++){
        sum += arreglo->data[i];
    }
    return sum / (double)m;
}

Array1d *mergePuntos2DTo1D(Array1d *arreglo_x, Array1d *arreglo_y, int m){
    
    // Los puntos x_i, y_i aparecen consecutivamente
    Array1d *z = array1d_alloc(2*m);
    if(!z){
        printf("!!! Fallo en asignación de memoria en la funcion 'mergePuntos2DTo1D'\n");
        return NULL;
    }
    for(int i=0;i<m;i++){
        z->data[2*i]   = arreglo_x->data[i];
        z->data[2*i+1] = arreglo_y->data[i]; 
    }
    return z;
}

Array2d *crearMatrizDisenoCirculo(Array1d *arreglo_x, Array1d *arreglo_y, int m, double x_barra, double y_barra){

    // Matriz de diseño de tamaño 2*m x 3
    Array2d *X = array2d_alloc(2*m, 3);     // Matriz inicializada en ceros
    if(!X){
        printf("!!! Error en la asignación de memoria para matriz de diseno.\n");
    }

    // Llenamos de 1's donde corresponde
    for(int i=0;i<2*m;i++){    // Renglones
        int j = i % 2;         // Columna 
        X->data[i][j] = 1;
    }

    // Llenamos de cos(θ_{k}) y sin(θ_{k})
    for(int i=0;i<m;i++){     // Renglones
        double theta = atan2(arreglo_y->data[i] - y_barra, 
                             arreglo_x->data[i] - x_barra);

        X->data[2*i][2]   = cos(theta);
        X->data[2*i+1][2] = sin(theta);
    }
    return X;
}

Array1d *minimosCuadradosCirculo(Array2d *X, Array1d *z){

    // Sistema a resolver: X^T X c = X^T z
    // Procedemso con Cholesky: X^T X = L L^T
    // X^T X es una matriz simétrica de 3 x 3

    // Calculamos X^T X
    Array2d *XTX = transpuestaPorMatriz(X);
    if(!XTX){
        // La función ya tiene su mensaje de error
        return NULL;
    }

    Array2d *L = cholesky(XTX, 3);
    freeArray2d(XTX);
    if(!L){
        // El método imprime sus mensajes de error
        return NULL;
    }

    // Si el método de Cholesky funciona: La matriz X^T X es definida positiva.
    // Por lo tanto, del sistema X^T X c = X^T z, la solución c es un mínimo.
    // Sistema a resolver L L^T c = X^T z

    // Calculamos X^T z
    Array1d *b = matrizTranspuestaPorVector(X, z);
    if(!b){
        // La función imprime su mensaje de error
        freeArray2d(L);
        return NULL;
    }

    // Arreglo con los 3 coeficientes del ajuste circular
    // Resolvemos L L^T c = b
    Array1d *coeficientes = solveLLT(L, b, 3);
    freeArray1d(b);
    freeArray2d(L);
    if(!coeficientes){
        // La función imprime su mensaje de error.
        return NULL;
    }
    return coeficientes;
}

Array1d *metodoMinimosCuadradosCirculo(const char *nombre_archivo, int N, double tol){

    // Implementación para la tarea que busca utilizar el ajuste a puntos en una circunferencia en el ajuste a puntos en un arco de circunferencia
    // c1 componente x del centro
    // c2 componente y del centro
    // c3 radio
    // static inicializa en cero
    static double c1, c2, c3;
    static int flag;    
    // printf("\n### c1 = %f c2 = %f c3 = %f", c1, c2, c3);   // Visualización valor coeficientes cada que se entra a la función.
    
    printf("\n>>> Ajuste de un circulo por minimos cuadrados.\n    Archivo %s\n    Numero de iteraciones: %d\n    Tolerancia %.3e <<<\n\n", nombre_archivo, N, tol);

    // Guardamos los puntos del archivo en dos arreglos identificados por Puntos2D
    printf(">>> Reading datos ...\n");
    Puntos2d *datos = readPuntos2D(nombre_archivo);
    if(!datos){
        return NULL;
    }
    Array1d *z = mergePuntos2DTo1D(datos->arreglo_x, datos->arreglo_y, datos->m);
    if(!z){
        freeArray1d(datos->arreglo_x);
        freeArray1d(datos->arreglo_y);
        free(datos);
        return NULL;
    }

    // Aquí guardamos los coeficientes solución.
    Array1d *coeficientes = NULL;

    // Ciclo de ajuste círculo
    int iter = 0;
    while(++iter <= N){ // Preincremento: Primero incrementamos, luego comparamos
        
        // Aproximación del centro del círculo
        double x_barra, y_barra;
        if(iter == 1){
            if(flag == 0){
                // Ini de centro del círculo: Centroide de los datos
                x_barra = promedio(datos->arreglo_x, datos->m);
                y_barra = promedio(datos->arreglo_y, datos->m);
                flag = 1;
            }
            else{
                // Ini de centro del círculo: Coeficientes solución del primer archivo .bin / primera llamada a la función
                x_barra = c1;
                y_barra = c2;
            }
        }
        else{
            // Siguientes iteraciones se utiliza la aproximación obtenida
            x_barra = coeficientes->data[0];    // c1
            y_barra = coeficientes->data[1];    // c2
            freeArray1d(coeficientes);          // Hay que liberar la memoria reservada de la iteración anterior
            coeficientes = NULL;
        }
        
        printf(">>> Iteracion %d\n", iter);

        // Se crea la matriz de diseño 
        printf("\t>>> Creando matriz de diseno ...\n");
        Array2d *X = crearMatrizDisenoCirculo(datos->arreglo_x, datos->arreglo_y, datos->m, x_barra, y_barra);
        if(!X){
            freeArray1d(datos->arreglo_x);
            freeArray1d(datos->arreglo_y);
            free(datos);
            freeArray1d(z);
            return NULL;
        }

        // Se resuelve el problema de mínimos cuadrados (Cholesky)
        printf("\t>>> Resolviendo minimos cuadrados (Cholesky) ...\n");
        coeficientes = minimosCuadradosCirculo(X, z);
        if(!coeficientes){
            freeArray1d(datos->arreglo_x);
            freeArray1d(datos->arreglo_y);
            free(datos);
            freeArray1d(z);
            freeArray2d(X);
            return NULL;
        }

        // Imprimir el arreglo c
        printf("\t>>> Coeficientes del ajuste: ");
        printArray1d(coeficientes," %.4e ", 3);

        // Imprimir el error cuadrático medio (RMSE)
        printf("\t>>> RMSE: %.6e\n", errorResidual(X, coeficientes, z) / sqrt(datos->m));
        freeArray2d(X);

        // Condición de paro
        double aproximacion_al_centro = sqrt(
            (x_barra - coeficientes->data[0]) * (x_barra - coeficientes->data[0]) +
            (y_barra - coeficientes->data[1]) * (y_barra - coeficientes->data[1]));
        if(aproximacion_al_centro < tol){
            printf(">>> Se cumple condicion de paro.\n");
            freeArray1d(datos->arreglo_x);
            freeArray1d(datos->arreglo_y);
            free(datos);
            freeArray1d(z);

            c1 = coeficientes->data[0];
            c2 = coeficientes->data[1];
            c3 = coeficientes->data[2];
            
            return coeficientes;
        }
    }
    // Se terminaron las iteraciones antes de cumplirse la condición de tolerancia
    printf(">>> Se alcanza el maximo de iteraciones.\n");
    freeArray1d(datos->arreglo_x);
    freeArray1d(datos->arreglo_y);
    free(datos);
    freeArray1d(z);

    c1 = coeficientes->data[0];
    c2 = coeficientes->data[1];
    c3 = coeficientes->data[2];

    return coeficientes;
}