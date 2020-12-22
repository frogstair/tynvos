#include <kernel/screen.h>

#include "vga.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

void putpixel(int, int, char);
size_t screen_buffer_location(int, int);

static const size_t SCREEN_WIDTH = 720;
static const size_t SCREEN_HEIGHT = 480;
static const size_t SCREEN_DEPTH = 32;
static uint16_t* PIXEL_BUFFER = (uint16_t*) 0xFC000000;
static uint16_t* MULTIBOOT_INFO_BLOCK = (uint16_t*) 0x0;

int screen_initialize() {

    asm("mov %%ebx, %0"
        : "=a"(MULTIBOOT_INFO_BLOCK));
    int flags = MULTIBOOT_INFO_BLOCK[0];

    if(!(flags & 0x1000)) {
        return 1;
    }

    

    return 0;
}

void putpixel(int x, int y, char color) {
    PIXEL_BUFFER[screen_buffer_location(x, y)] = color;
}

size_t screen_buffer_location(int x, int y) {
    int pitch = SCREEN_WIDTH * SCREEN_DEPTH;
    int width = SCREEN_DEPTH;
    //return (size_t)(y * pitch + x * width);
    return (size_t)(y * SCREEN_WIDTH + x);
}