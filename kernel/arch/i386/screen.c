#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <math.h>

#include <kernel/mutiboot.h>

#include <screen/screen.h>
#include <screen/tyn.h>
#include <screen/color.h>

int screen_buffer_location(int, int);

const char CHAR_WIDTH = 5;
const char CHAR_HEIGHT = 8;
const char LINE_SPACE = 1;
const char LETTER_SPACE = 1;

char screen_font_scale = 1;

char char_width;
char char_height;
char line_space;
char letter_space;

const char SCREEN_MARGIN_V = 10;
const char SCREEN_MARGIN_H = 10;

size_t screen_row = SCREEN_MARGIN_V;
size_t screen_column = SCREEN_MARGIN_H;

uint64_t screen_color = 0xffffffff;

size_t SCREEN_WIDTH, SCREEN_HEIGHT;

size_t PIXEL_BUFFER_LENGTH;

static uint8_t *PIXEL_BUFFER = (uint8_t *)0x0;

struct color_info
{
    uint8_t red_field;
    uint8_t red_mask;
    uint8_t green_field;
    uint8_t green_mask;
    uint8_t blue_field;
    uint8_t blue_mask;
};

struct framebuffer
{
    uint64_t ADDR;
    uint32_t PITCH;
    uint32_t WIDTH;
    uint32_t HEIGHT;
    uint8_t BPP;
    uint8_t TYPE;
    struct color_info COLORS;
} * FRAME_BUFFER;

void screen_initialize()
{
    FRAME_BUFFER = (struct framebuffer *)(MULTIBOOT_INFO_ADDRESS + 88);
    PIXEL_BUFFER = (uint32_t *)FRAME_BUFFER->ADDR;

    SCREEN_WIDTH = FRAME_BUFFER->WIDTH;
    SCREEN_HEIGHT = FRAME_BUFFER->HEIGHT;
    PIXEL_BUFFER_LENGTH = SCREEN_HEIGHT * SCREEN_WIDTH * FRAME_BUFFER->BPP / 8;

    screen_clear();

    char_width = CHAR_WIDTH * screen_font_scale;
    char_height = CHAR_HEIGHT * screen_font_scale;
    line_space = LINE_SPACE * screen_font_scale;
    letter_space = LETTER_SPACE * screen_font_scale;
}

void screen_putpixel(int x, int y, uint64_t colora)
{
    uint32_t *pixel = (uint32_t *)(PIXEL_BUFFER + screen_buffer_location(x, y));

    uint8_t transparency = (colora & 0xff000000) >> 24;
    if (transparency == 255)
        *pixel = (uint32_t)colora;
    else
    {
        uint32_t color = color_rgbatorgb(*pixel, colora);
        *pixel = color;
    }
}

int screen_buffer_location(int x, int y)
{
    return y * FRAME_BUFFER->PITCH + x * (FRAME_BUFFER->BPP / 8);
}

void screen_putentryat(unsigned char c, uint64_t color, int xpos, int ypos)
{
    if (c < 128)
    {
        char *letter = tyn_ascii[c];
        for (int ychar = 0; ychar < char_height / screen_font_scale; ychar++)
        {
            for (int ys = 0; ys < screen_font_scale; ys++)
            {
                int drawnh = 0;
                for (int xchar = 0; xchar < char_width + letter_space; xchar++)
                {
                    for (int xs = 0; xs < screen_font_scale; xs++)
                    {
                        char bit = letter[ychar] & 1 << xchar;
                        if (bit)
                            screen_putpixel(xpos, ypos, color);
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

void screen_putchar(char c)
{
    unsigned char uc = c;

    switch (uc)
    {
    case 9:
        for (int i = 0; i < 4; i++)
        {
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

    if (screen_column + char_width + letter_space > SCREEN_WIDTH)
    {
        screen_column = SCREEN_MARGIN_H;
        screen_advancecharrow();
    }
}

void screen_advancecharrow()
{
    screen_row += line_space + char_height;
    if (screen_row > SCREEN_HEIGHT)
    {
        screen_row = SCREEN_MARGIN_V + line_space + char_height;
    }
}

void screen_advancerow()
{

    screen_row++;
}

void screen_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        screen_putchar(data[i]);
    }
}

void screen_writestring(const char *data)
{
    screen_write(data, strlen(data));
}

void screen_setcolor(uint64_t color)
{
    screen_color = color;
}

int screen_snaptorow(int y)
{
    int offset = y % (CHAR_HEIGHT + LINE_SPACE);
    return y + (CHAR_HEIGHT + LINE_SPACE - offset);
}

int screen_snaptocolumn(int x)
{
    int offset = x % (CHAR_WIDTH + LETTER_SPACE);
    return x + (CHAR_WIDTH + LETTER_SPACE - offset);
}

void screen_drawbmp(uint64_t *buffer, size_t width, size_t height, int scale, bool skip)
{
    screen_advancerow();
    int xp = screen_column;
    int yp = screen_row;
    for (size_t y = 0; y < height; y++)
    {
        for (int ys = 0; ys < scale; ys++)
        {
            for (size_t x = 0; x < width; x++)
            {
                for (int xs = 0; xs < scale; xs++)
                {
                    int location = x + y * width;
                    screen_putpixel(xp, yp, buffer[location]);
                    xp++;
                }
            }
            xp = SCREEN_MARGIN_H;
            yp++;
        }
    }
    if (skip)
    {
        screen_row = screen_snaptorow(yp);
        screen_column = screen_snaptocolumn(xp);
        screen_advancecharrow();
    }
}

void screen_drawline(int x0, int y0, int x1, int y1, uint64_t color)
{

    bool steep = abs(y1 - y0) > abs(x1 - x0);

    if (steep)
    {
        x0 = x0 + y0;
        y0 = x0 - y0;
        x0 = x0 - y0;

        x1 = x1 + y1;
        y1 = x1 - y1;
        x1 = x1 - y1;
    }

    if (x0 > x1)
    {
        x0 = x0 + x1;
        x1 = x0 - x1;
        x0 = x0 - x1;

        y0 = y0 + y1;
        y1 = y0 - y1;
        y0 = y0 - y1;
    }

    int deltaErr = abs(y1 - y0);
    int ystep = y0 > y1 ? -1 : 1;
    int deltax = x1 - x0;

    float error = deltax >> 1;
    int y = y0;

    for (int x = x0; x <= x1; x++)
    {
        if (steep)
            screen_putpixel(y, x, color);
        else
            screen_putpixel(x, y, color);
        error -= deltaErr;
        if (error < 0)
        {
            y += ystep;
            error += deltax;
        }
    }
}

void screen_drawrect(int x, int y, int w, int h, uint64_t color, bool fill)
{
    screen_drawline(x, y, x + w, y, color);
    screen_drawline(x, y + h, x + w, y + h, color);

    screen_drawline(x, y, x, y + h, color);
    screen_drawline(x + w, y, x + w, y + h, color);

    if (fill)
    {
        for (int x1 = 1; x1 < w; x1++)
        {
            for (int y1 = 1; y1 < h; y1++)
            {
                screen_putpixel(x1 + x, y1 + y, color);
            }
        }
    }
}

void screen_clear()
{
    memset(PIXEL_BUFFER, 0x000000, PIXEL_BUFFER_LENGTH);
}