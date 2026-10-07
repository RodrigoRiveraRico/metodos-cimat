#include "polinomio.h"
#include "../cholesky/cholesky.h"


Array2d *crearMatrizDiseno(Array1d *arreglo_x, int m, int n){

    // Asignación memoria para matriz de m x n.
    // Se inicializa en ceros.
    Array2d *matriz = array2d_alloc(m,n+1);
    if(!matriz){
        printf(">>> No se pudo asignar memoria para la matriz de diseno\n");
        return NULL;
    }

    // Llenado de la matriz
    for(int i=0;i<m;i++){

        // Última columna de cada fila siempre es 1
        matriz->data[i][n] = 1.0;
        
        // Llenado desde la penúltima columna hasta la primera.
        // Cada asignación se hace con el valor de la asignación anterior.
        // X_{i} ^ n = X_{i} ^ (n-1) * X_{i}
        for(int j=n-1;j>=0;j--){
            matriz->data[i][j] = matriz->data[i][j+1] * arreglo_x->data[i];
        }
    }
    return matriz;
}

Array2d *transpuestaPorMatriz(Array2d *matriz){

    // Si A es de m x n, entonces A^T A es de n x n
    size_t m = matriz->rows;
    size_t n = matriz->cols;

    // Aquí va el resultado A^T A
    Array2d * matriz_simetrica = array2d_alloc(n,n);
    if(!matriz_simetrica){
        printf(">>> Fallo en asignación de memoria. Funcion 'transpuestaPorMatriz'\n");
        return NULL;
    }

    // Llenar diagonal
    for(int j=0;j<n;j++){       // Columna
        double suma = 0.0;
        for(int i=0;i<m;i++){   // Renglón
            suma += matriz->data[i][j] * matriz->data[i][j];
        }   
        matriz_simetrica->data[j][j] = suma;
    }

    // Calculamos lo que hay debajo de la diagonal y aprovechamos que la matriz resultante es simétrica.
    for(int j=0;j<n-1;j++){         // Columna
        for(int i=j+1;i<n;i++){     // Renglón
            double suma = 0.0;
            for(int k=0;k<m;k++){   // Multiplicación
                suma += matriz->data[k][j] * matriz->data[k][i];
            }
            matriz_simetrica->data[i][j] = suma;
            matriz_simetrica->data[j][i] = suma;
        }
    }
    return matriz_simetrica;
}

Array1d *matrizTranspuestaPorVector(Array2d *matriz, Array1d *vector){

    // Si A una matriz es de m x n, entonces su transpuesta A^T es de n x m
    size_t m = matriz->rows;
    size_t n = matriz->cols;

    // Calculamos el producto A^T x v
    // Si el vector v es de m x 1, el resultado será otro vector de tamaño n x 1
    Array1d *resultado = array1d_alloc(n);
    if(!resultado){
        printf(">>> Fallo en asignación de memoria. Funcion 'matrizTranspuestaPorVector'\n");
        return NULL;
    }

    for(int j=0;j<n;j++){       // Columna
        double suma = 0.0;
        for(int i=0;i<m;i++){   // Renglón
            suma += matriz->data[i][j] * vector->data[i];
        }
        resultado->data[j] = suma;
    }
    return resultado;
}

// No se está utilizando el parámetro m dentro de la función 
Array1d *minimosCuadradosPolinomio(Array2d *matriz, Array1d *arreglo_y, int m, int n){

    // Sistema a resolver: X^T X c = X^T y 
    // Procedemos con Cholesky: X^T X = L L^T
    // X^T X es una matriz simétrica de (n+1) x (n+1)

    // Calculamos X^T X
    Array2d *XTX = transpuestaPorMatriz(matriz);
    if(!XTX){
        // La función ya tiene su mensaje de error
        return NULL;
    }

    Array2d *L = cholesky(XTX, n+1);
    if(!L){
        // El método imprime sus mensajes de error
        freeArray2d(XTX);
        return NULL;
    }

    // Si el método de cholesky funciona: La matriz X^T X es definida positiva. 
    // Por lo tanto, del sistema X^T X c = X^T y, la solución c es un mínimo.
    // Sistema a resolver: L L^T c = X^T y

    // Calculamos X^T y
    Array1d *b = matrizTranspuestaPorVector(matriz, arreglo_y);
    if(!b){
        // La función imprime su mensaje de error
        freeArray2d(L);
        freeArray2d(XTX);
        return NULL;
    }

    // Arreglo con los n+1 coeficientes del polinomio de grado n
    // Resolvemos L L^T c = b
    Array1d *coeficientes = solveLLT(L, b, n+1);
    if(!coeficientes){
        // La función imprime su mensaje de error.
        freeArray1d(b);
        freeArray2d(L);
        freeArray2d(XTX);
        return NULL;
    }

    freeArray1d(b);
    freeArray2d(L);
    freeArray2d(XTX);
    return coeficientes;
}

Puntos2d *readPuntos2D(const char *cfile){
    Puntos2d *datos = NULL;
    Array1d  *arreglo_x = NULL;
    Array1d  *arreglo_y = NULL;
    uint64_t nr, nc;
    FILE     *f1 = fopen(cfile, "rb");

    if(!f1){
        printf(">>> Error al abrir el archivo.\n");
        return NULL;
    }

    fread(&nr, sizeof(uint64_t), 1, f1);    // Renglones. Cantidad de puntos 2D
    fread(&nc, sizeof(uint64_t), 1, f1);    // Columnas. 2 (x,y)

    // Puntos x_i
    arreglo_x = array1d_alloc(nr);
    if(!arreglo_x){
        printf(">>> Fallo asignación de memoria para almacenar los puntos del archivo.\n");
        fclose(f1);
        return NULL;
    }

    // Puntos y_i
    arreglo_y = array1d_alloc(nr);
    if(!arreglo_y){
        printf(">>> Fallo asignación de memoria para almacenar los puntos del archivo.\n");
        fclose(f1);
        freeArray1d(arreglo_x);
        return NULL;
    }
    
    // Leemos del archivo
    // Los elementos impares son x_i
    // Los elementos pares son y_i
    for(int i=0;i<nr;i++){
        fread(&arreglo_x->data[i], sizeof(double), 1, f1);
        fread(&arreglo_y->data[i], sizeof(double), 1, f1);
    }
    fclose(f1);

    // En una estructura devolvemos los puntos leidos
    datos = malloc(sizeof *datos);
    if(!datos){
        freeArray1d(arreglo_x);
        freeArray1d(arreglo_y);
        printf(">>> Fallo asignación de memoria para almacenar los puntos del archivo.\n");
        return NULL;
    }

    datos->m = nr;
    datos->arreglo_x = arreglo_x;
    datos->arreglo_y = arreglo_y;
    return datos;
}

Array1d *ejercicio1(const char * nombre_archivo, int grado_polinomio){

    // Guardamos los puntos del archivo en dos arreglos identificados por Puntos2D
    Puntos2d *datos = readPuntos2D(nombre_archivo);
    if(!datos){
        return NULL;
    }

    // Se crea la matriz de diseño X
    Array2d *X = crearMatrizDiseno(datos->arreglo_x,datos->m,grado_polinomio);
    if(!X){
        freeArray1d(datos->arreglo_x);
        freeArray1d(datos->arreglo_y);
        free(datos);
        return NULL;
    }

    // Se resuelve el problema de mínimos cuadrados
    Array1d *coeficientes_polinomio = minimosCuadradosPolinomio(X, datos->arreglo_y, datos->m, grado_polinomio);
    if(!coeficientes_polinomio){
        freeArray1d(datos->arreglo_x);
        freeArray1d(datos->arreglo_y);
        free(datos);
        freeArray2d(X);
        return NULL;
    }

    // Imprimir grado del polinomio
    // Imprimir el arreglo c
    // Imprimir el error cuadrático medio

    // Grabar los valores de c


    freeArray1d(datos->arreglo_x);
    freeArray1d(datos->arreglo_y);
    free(datos);
    freeArray2d(X);
    return coeficientes_polinomio;
}