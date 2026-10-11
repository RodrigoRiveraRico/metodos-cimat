#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "circulo.h"

/* 
gcc .\ejercicio2.c .\circulo.c  ..\ejercicio1\polinomio.c ..\arrays\array1D.c ..\arrays\array2D.c ..\cholesky\cholesky.c -lm -o .\ejercicio2 & .\ejercicio2 ..\datosTarea06\circunferencia1.bin ..\datosTarea06\circunferencia2.bin 30 0.01
*/

/**
 * @brief Función que crea la ruta de los archivos de salida
 * 
 * La función tiene como objetivo crear la ruta del archivo de salida de modo que el archivo de salida viva en la misma carpeta del archivo de entrada.
 * 
 * La función concatena la cadena "coeficientes_" al inicio del nombre del archivo de salida.
 * 
 * @param ruta_archivo_entrada string Ruta archivo entrada
 * @param ruta_archivo_salida string Dirección de memoria dónde guardar la ruta del archivo de salida
 * @return int
 * @retval 0 en caso de éxito
 * @retval 1 en caso de fallo
 */
int crearRutaArchivoSalidaConcat(const char *ruta_archivo_entrada, char *ruta_archivo_salida);

int main(int argc, char **argv){

    int    N   = 30;         // Número máximo de iteraciones
    double tol = 0.01;       // Tolerancia aproximación del centro de la circunferencia

    /*
     * Argumentos:
     *
     * argv[1] -> archivo de circunferencia
     * argv[2] -> archivo de arco
     * argv[3] -> N   (opcional)
     * argv[4] -> tol (opcional)
     */

     if(argc < 3 || argc > 5){
        printf("\nUso:");
        printf("\n%s archivo_circ archivo_arc [N] [tol]\n", argv[0]);
        return 1;
    }

    /* N */
    if(argc >= 4){

        char *endptr;

        N = (int)strtol(argv[3], &endptr, 10);

        if(*endptr != '\0' || N <= 0){
            printf("\nError: N debe ser un entero positivo.\n");
            return 1;
        }
    }

    /* tol */
    if(argc == 5){

        char *endptr;

        tol = strtod(argv[4], &endptr);

        if(*endptr != '\0' || tol <= 0){
            printf("\nError: tol debe ser un numero positivo.\n");
            return 1;
        }
    }

    //////////////////////////////////////////////////////////
    ///////////////////// CIRCUNFERENCIA /////////////////////
    //////////////////////////////////////////////////////////

    // Nombre del archivo .bin de los datos por ajustar (circunferencia)
    const char *archivo_de_puntos_circulo_bin = argv[1];    // "../datosTarea06/circunferencia1.bin"
    // Nombre del archivo .bin de los coeficientes ajustados (circunferencia)
    char archivo_de_salida_circulo_bin[256];
    crearRutaArchivoSalidaConcat(archivo_de_puntos_circulo_bin, archivo_de_salida_circulo_bin);   // "../datosTarea06/coeficientes_circunferencia1.bin";

    Array1d *coeficientes = metodoMinimosCuadradosCirculo(archivo_de_puntos_circulo_bin, N, tol);
    if(!coeficientes){
        return 1;
    }

    // Grabar datos (circunferencia)
    printf(">>> Writing coeficientes en %s\n",archivo_de_salida_circulo_bin);
    if(writeArray1d(coeficientes, archivo_de_salida_circulo_bin) == 1){
        printf("!!! Error en la escritura del archivo binario.");
        freeArray1d(coeficientes);
        return 1;
    }
    printf(">>> Exito en la escritura del archivo binario.\n");
    freeArray1d(coeficientes);
    coeficientes = NULL;

    //////////////////////////////////////////////////////////
    ////////////////////////// ARCO //////////////////////////
    //////////////////////////////////////////////////////////

    // Nombre del archivo .bin de los datos por ajustar (arco)
    const char *archivo_de_puntos_arco_bin = argv[2];   // "../datosTarea06/circunferencia2.bin";
    // Nombre del archivo .bin de los coeficientes ajustados (arco)
    char archivo_de_salida_arco_bin[256];
    crearRutaArchivoSalidaConcat(archivo_de_puntos_arco_bin, archivo_de_salida_arco_bin);     // "../datosTarea06/coeficientes_circunferencia2.bin";

    coeficientes = metodoMinimosCuadradosCirculo(archivo_de_puntos_arco_bin, N, tol);
    if(!coeficientes){
        return 1;
    }

    // Grabar datos (arco)
    printf(">>> Writing coeficientes en %s\n",archivo_de_salida_arco_bin);
    if(writeArray1d(coeficientes, archivo_de_salida_arco_bin) == 1){
        printf("!!! Error en la escritura del archivo binario.\n");
        freeArray1d(coeficientes);
        return 1;
    }
    printf(">>> Exito en la escritura del archivo binario.\n");
    freeArray1d(coeficientes);

    return 0;
}

int crearRutaArchivoSalidaConcat(const char *ruta_archivo_entrada, char *ruta_archivo_salida) {
    const char *origen = ruta_archivo_entrada;
    char *destino = ruta_archivo_salida;

    // 1. Buscamos la última diagonal normal '/'
    const char *ultima_diagonal = strrchr(origen, '/');
    
    // 2. Si no la encuentra, buscamos la diagonal invertida '\' por si acaso es Windows
    if (ultima_diagonal == NULL) {
        ultima_diagonal = strrchr(origen, '\\');
    }

    if (ultima_diagonal != NULL) {
        // --- CASO CON RUTA ---
        // Calcular cuántos caracteres componen la ruta de la carpeta (incluyendo la diagonal)
        int longitud_ruta = ultima_diagonal - origen + 1;

        // Copiamos la parte de la carpeta
        strncpy(destino, origen, longitud_ruta);
        destino[longitud_ruta] = '\0'; 

        // Concatenamos el prefijo y el nombre del archivo
        strcat(destino, "coeficientes_");
        strcat(destino, ultima_diagonal + 1);
    } else {
        // --- CASO SIN RUTA (El archivo se pasa directamente, ej: "circunferencia1.bin") ---
        // Simplemente unimos el prefijo con el nombre directamente
        strcpy(destino, "coeficientes_");
        strcat(destino, origen);
    }

    // Mostrar resultados para verificar
    // printf("Original:  %s\n", origen);
    // printf("Resultado: %s\n", destino);

    return 0; // Éxito en cualquier escenario
}