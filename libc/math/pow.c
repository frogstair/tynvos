#include <math.h>

double pow(double b, double p)
{
    double r = b;
    for (int i = 0; i < p; i++)
        r *= p;
    return r;
}