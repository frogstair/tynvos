#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/mutiboot.h>

#include <screen/screen.h>
#include <screen/tyn.h>

int screen_buffer_location(int, int);

const char CHAR_WIDTH = 5;
const char CHAR_HEIGHT = 8;
const char LINE_SPACE = 1;
const char LETTER_SPACE = 1;

char screen_font_scale = 2;

char char_width;
char char_height;
char line_space;
char letter_space;

const char SCREEN_MARGIN_V = 10;
const char SCREEN_MARGIN_H = 10;

size_t screen_row = SCREEN_MARGIN_V;
size_t screen_column = SCREEN_MARGIN_H;

uint32_t screen_color = 0xffffff;

size_t SCREEN_WIDTH, SCREEN_HEIGHT;

size_t PIXEL_BUFFER_LENGTH;

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
}* FRAME_BUFFER;

void screen_initialize() {
    FRAME_BUFFER = (struct framebuffer*)(MULTIBOOT_INFO_ADDRESS + 88);
    PIXEL_BUFFER = (uint32_t*)FRAME_BUFFER->ADDR;

    SCREEN_WIDTH = FRAME_BUFFER->WIDTH;
    SCREEN_HEIGHT = FRAME_BUFFER->HEIGHT;
    PIXEL_BUFFER_LENGTH = SCREEN_HEIGHT * SCREEN_WIDTH * FRAME_BUFFER->BPP/8;

    char_width = CHAR_WIDTH * screen_font_scale;
    char_height = CHAR_HEIGHT * screen_font_scale;
    line_space = LINE_SPACE * screen_font_scale;
    letter_space = LETTER_SPACE * screen_font_scale;
}

void screen_putpixel(int x, int y, uint32_t color) {
    uint32_t* pixel = (uint32_t*)(PIXEL_BUFFER + screen_buffer_location(x, y));
    *pixel = color;
}

int screen_buffer_location(int x, int y) {
    return y * FRAME_BUFFER->PITCH + x * (FRAME_BUFFER->BPP/8);
}

void screen_putentryat(unsigned char c, uint32_t color, int xpos, int ypos) {
    if(c < 128) {
        char* letter = tyn_ascii[c];
        for(int ychar = 0; ychar < char_height/screen_font_scale; ychar++) {
            for(int ys = 0; ys < screen_font_scale; ys++) {
                int drawnh = 0;
                for(int xchar = 0; xchar < char_width + letter_space; xchar++) {
                    for(int xs = 0; xs < screen_font_scale; xs++) {
                        char bit = letter[ychar] & 1 << xchar;
                        if(bit) screen_putpixel(xpos, ypos, color);
                        xpos++;
                        drawnh++;
                    }
                }
                xpos -= drawnh;
                ypos++;
            }
        }
    }
}

void screen_putchar(char c) {
    unsigned char uc = c;

    switch(uc) {
        case 9:
            for(int i = 0; i < 4; i++) {
                screen_putentryat(' ', screen_color, screen_column, screen_row);
                screen_column += char_width + letter_space;
            }
            break;
        case 10:
            screen_column = SCREEN_MARGIN_H;
            screen_advancecharrow();
            break;
        default:
            screen_putentryat(uc, screen_color, screen_column, screen_row);
            screen_column += char_width + letter_space;
            break;
    }

    if(screen_column + char_width + letter_space > SCREEN_WIDTH) {
        screen_column = SCREEN_MARGIN_H;
        screen_advancecharrow();
    }

}

void screen_advancecharrow() {
    screen_row+=line_space + char_height;
    if(screen_row > SCREEN_HEIGHT) {
        screen_row = SCREEN_MARGIN_V + line_space + char_height;
    }
}

void screen_advancerow() {
                            
    screen_row++;
}

void screen_write(const char* data, size_t size) {
    for(size_t i = 0; i < size; i++) {
        screen_putchar(data[i]);
    }
}

void screen_writestring(const char* data) {
    screen_write(data, strlen(data));
}

void screen_setcolor(uint32_t color) {
    screen_color = color;
}

void screen_drawbmp(uint32_t* buffer, size_t width, size_t height, int scale) {
    screen_advancerow();
    for(size_t y = 0; y < width; y++) {
        for(int ys = 0; ys < scale; ys++) {
            for(size_t x = 0; x < height; x++) {
                for(int xs = 0; xs < scale; xs++) {
                    int location = x + y * width;
                    screen_putpixel(screen_column, screen_row, buffer[location]);
                    screen_column++;
                }
            }
            screen_column = SCREEN_MARGIN_H;
            screen_advancerow();
        }
    }
}