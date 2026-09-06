
#include <math.h>
#include "funciones_prueba.h"

double f1(double x){
    return 0.2*x*log(x) + (x - 2.3)*(x - 2.3);
}

double df1(double x){
    return 2.0*x + 0.2 * log(x) - 4.4;
}

double ddf1(double x){
    return 0.2/x + 2.0;
}

double f2(double x){
    return -(1.5 - 3.0*x)*sin(18.0*x);
}

double df2(double x){
    return 3*sin(18.0*x) + (54.0*x - 27.0)*cos(18.0*x);
}

double ddf2(double x){
    return (486.0 - 972.0*x)*sin(18.0*x) + 108.0*cos(18.0*x);
}

double f3(double x){
    return 0.5*(x-1)*(x-1) + 3*sin(x);
}

double df3(double x){
    return x + 3.0*cos(x) - 1.0;
}

double ddf3(double x){
    return 1.0 - 3.0*sin(x);
}

double f4(double x){
    return x + 2*fabs(sin(x));
}