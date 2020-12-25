#ifndef _KERNEL_SCREEN_H
#define _KERNEL_SCREEN_H 1

#include <stddef.h>
#include <stdint.h>

void screen_initialize(void);
void screen_putchar(char c);
void screen_write(const char* data, size_t size);
void screen_writestring(const char* data);
void screen_setcolor(uint32_t color);
void screen_advancerow(void);
void screen_advancecharrow(void);
void screen_putpixel(int, int, uint32_t);

void screen_drawbmp(uint32_t* array, size_t width, size_t height, int scale);

extern size_t SCREEN_WIDTH;
extern size_t SCREEN_HEIGHT;
extern char screen_font_scale;

#endif