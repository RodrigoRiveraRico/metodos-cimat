#include "array2D.h"


// Reserva memoria para un arreglo 2D de tamaño nr x nc
Array2d *array2d_alloc(int nr, int nc) {
    // Reservamos memoria
    Array2d* array = (Array2d *) malloc(sizeof(Array2d));
    if(!array) {
        return NULL;
    }
    array->data = (double **) malloc( (nr)*sizeof(double *));
    if(array->data==NULL) {
        free(array);
        return(NULL);
    }
    array->data[0] = (double *) calloc(nr*nc, sizeof(double));
    if(array->data[0]==NULL) {
        free(array->data);
        free(array);
        return(NULL);
    }
    array->rows = nr;
    array->cols = nc;
    array->ndim = 2;
    for(int i=1; i<nr; ++i) 
        array->data[i] = array->data[i-1] + nc;
    return(array);
}

// Lectura de las entradas de un arreglos 2D almacenadas en un archivo binario.
// Devuelve NULL si no se pudo abrir el archivo.
Array2d *readArray2d(const char *cfile) {
    Array2d *array;
    uint64_t nr, nc;
    FILE    *f1 = fopen(cfile, "rb");
    
    if(!f1)  return NULL;
    fread(&nr, sizeof(uint64_t), 1, f1);
    fread(&nc, sizeof(uint64_t), 1, f1);
    array = array2d_alloc(nr, nc);
    if(!array) return NULL;

    fread(array->data[0], sizeof(double), nr*nc, f1);
    fclose(f1);
    return(array);
}

// Almacena las entradas del arreglo 2D en un archivo binario.
// Devuelve 0 en caso de exito y 1 si no.
int writeArray2d(Array2d *array, const char *cfullname) {
    FILE       *f1 = fopen(cfullname, "wb");
    
    if(!f1) return(1);
    int  nr = array->rows,  nc=array->cols;
    fwrite(&nr, sizeof(int), 1, f1);
    fwrite(&nc, sizeof(int), 1, f1);
    fwrite(array->data[0], sizeof(double), nr*nc, f1);
    fclose(f1);
    return(0);
}


// Libera la memoria del arreglo bidimensional
void freeArray2d(Array2d *array) {
    free(array->data[0]);
    free(array->data);
    free(array);
}

// Imprime la fila i-esima del arreglo 
void printRow(double **data, int i, int nc, const char *format, int nview) {
    printf("[");
    if((2*nview+1)<nc) {
        for(int j=0; j<nview; ++j) 
            printf(format, data[i][j]);
        printf(" ... ");
        for(int j=nc-nview; j<nc; ++j) 
            printf(format, data[i][j]);
    }
    else {
        for(int j=0; j<nc; ++j) 
            printf(format, data[i][j]);
    }
    printf("]\n");
}

// Imprime en la consola las entradas del arraglo 2D usando el formato indicado 
// en la cadena format.
// nshow indica la cantidad de filas que se imprimen del inicio y del final
// del arreglo, así como la cantidad de valores de valores que se imprimen
// al inicio y final de cada fila.
// Esto para evitar imprimir todo el arreglo cuando es muy grande.
void printArray2d(Array2d *array, const char *format, int nview) {
    int i, j, nr=array->rows, nc=array->cols; 

    if((2*nview+1)<nr ) {
        for(i=0; i<nview; ++i) 
            printRow(array->data, i, nc, format, nview);
        printf("... \n");
        for(i=nr-nview; i<nr; ++i) 
            printRow(array->data, i, nc, format, nview);
    }
    else {
        for(i=0; i<nr; ++i) 
            printRow(array->data, i, nc, format, nview);
    }

}



