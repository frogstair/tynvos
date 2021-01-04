#include <math.h>

double fmod(double a, double b) {
    return a - (int)(a/b) * b;
}