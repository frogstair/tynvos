#ifndef _KERNEL_SCREEN_H
#define _KERNEL_SCREEN_H

#include <stddef.h>
#include <stdint.h>

int screen_initialize(void);

void putpixel(int, int, uint32_t);

extern size_t SCREEN_WIDTH;
extern size_t SCREEN_HEIGHT;

#endif