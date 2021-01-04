#include <math.h>

int ceil(double num) {
    int inum = (int)num;
    return num == (float)inum ? inum : inum + 1;
}