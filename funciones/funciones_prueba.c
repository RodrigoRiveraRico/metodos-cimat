
#include <math.h>
#include "funciones_prueba.h"

double f1(double x){
    return sin(x)*exp(0.5*x) - 0.5*x*x + 6.0;
}

double df1(double x){
    return exp(0.5*x) * (0.5*sin(x) + cos(x)) - x;
}

double f2(double x){
    return x * (x * (x * (x * (x * (4*x*x - 65) + 94) + 63) - 122) + 132) - 216;
}

double df2(double x){
    return x * (x * (x * (x * (28*x*x - 325) + 376) + 189) - 244) + 132;
}

double f3(double x){
    return atan(x);
}

double df3(double x){
    return 1/(x*x + 1);
}