#include "array1D.h"

// Reserva memoria para un arreglo de tamaño n
Array1d *array1d_alloc(size_t n) {
    // Reservamos memoria
    Array1d *array = (Array1d *) malloc(sizeof(Array1d));
    if (!array) { 
        return NULL; 
    }
    
    array->data = (double *) calloc(n, sizeof(double));
    if(array->data==NULL) {
        free(array);
        return NULL;
    }
    array->ndim = 1;
    array->n    = n;

    return(array);
}

// Crea un arreglo 1D a partir de la información en un archivo binario
Array1d *readArray1d(const char *cfullname) {
    uint64_t n;
    FILE    *f1 = fopen(cfullname, "rb");
    
    if(!f1) return(NULL);
    fread(&n, sizeof(uint64_t), 1, f1);

    Array1d *array = array1d_alloc(n);
    if(!array) {
        return NULL;
    }

    fread(array->data, sizeof(double), n, f1);
    fclose(f1);

    return(array);
}

// Escribe las entradas de un arreglo en un archivo binario
int writeArray1d(Array1d *array, const char *cfullname) {
    int     n;
    FILE   *f1 = fopen(cfullname, "wb");
    
    if(!f1) return(1);
    fwrite(&(array->n), sizeof(size_t), 1, f1);
    fwrite(array->data, sizeof(double), array->n, f1);
    fclose(f1);
    return(0);
}


// Libera la memoria del arreglo unidimensional
void freeArray1d(Array1d *array) {
    free(array->data);
    free(array);
}

// Imprime los elementos de un vector usando el formato especificado
// en la cadena format.
// nshow indica la cantidad de valores que se imprimen del inicio y del final
// del arreglo. Esto para evitar imprimir todo el arreglo cuando es muy grande.
void printArray1d(Array1d *array, const char *format, int nshow)  {
    size_t   i;
    printf("[");
    if(nshow==0 || (2*nshow)>=array->n) {
        for(i=0; i<array->n; i++) {
            printf(format, array->data[i]);
        }
    }
    else {
        for(i=0; i<nshow; i++) {
            printf(format, array->data[i]);
        }
        printf(" ... ");
        for(i=array->n-nshow; i<array->n; i++) {
            printf(format, array->data[i]);
        }
    }
    printf("]\n");
}


