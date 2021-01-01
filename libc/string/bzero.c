#include <string.h>

void bzero(void* to, size_t count) {
    memset(to, 0, count);
}