#ifndef _SCREEN_COLOR_H
#define _SCREEN_COLOR_H 1

#include <stdint.h>

typedef struct hsl hsl;

hsl color_rgbtohsl(uint32_t);
uint32_t color_hsltorgb(hsl);
uint32_t color_rgbatorgb(uint32_t, uint64_t);

#endif