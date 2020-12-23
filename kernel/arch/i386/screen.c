#include <kernel/screen.h>

#include "vga.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

int screen_buffer_location(int, int);

size_t SCREEN_WIDTH, SCREEN_HEIGHT;

static uint64_t* MULTIBOOT_INFO_BLOCK = (uint64_t*) 0x0;
static uint8_t* PIXEL_BUFFER = (uint8_t*) 0x0;

struct color_info {
    uint8_t red_field;
    uint8_t red_mask;
    uint8_t green_field;
    uint8_t green_mask;
    uint8_t blue_field;
    uint8_t blue_mask;
};

struct framebuffer {
    uint64_t ADDR;
    uint32_t PITCH;
    uint32_t WIDTH;
    uint32_t HEIGHT;
    uint8_t  BPP;
    uint8_t  TYPE;
    struct color_info COLORS;
}* FRAME_BUFFER ;

int screen_initialize() {

    asm("mov %%ebx, %0"
        : "=a"(MULTIBOOT_INFO_BLOCK));
    int flags = MULTIBOOT_INFO_BLOCK[0];

    if(!(flags & 0x1000)) {
        return 1;
    }

    FRAME_BUFFER = (struct framebuffer*)(MULTIBOOT_INFO_BLOCK + 11);
    PIXEL_BUFFER = (uint32_t*)FRAME_BUFFER->ADDR;

    SCREEN_WIDTH = FRAME_BUFFER->WIDTH;
    SCREEN_HEIGHT = FRAME_BUFFER->HEIGHT;

    return 0;
}

void putpixel(int x, int y, uint32_t color) {
    uint32_t* pixel = (uint32_t*)(PIXEL_BUFFER + screen_buffer_location(x, y));
    *pixel = color;
}

int screen_buffer_location(int x, int y) {
    return y * FRAME_BUFFER->PITCH + x * (FRAME_BUFFER->BPP/8);
}