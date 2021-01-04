#include <screen/color.h>

#include <stdint.h>
#include <math.h>

typedef struct hsl {
    int hue;
    float saturation;
    float lightness; 
} hsl;

float float_max(float r, float g, float b);
float float_min(float r, float g, float b);

hsl color_rgbtohsl(uint32_t color) {
    hsl colors;

    int r = (color & 0xff0000) >> 16;
    int g = (color & 0x00ff00) >> 8;
    int b = color & 0x0000ff;

    float rdash = (float)r / 255;
    float gdash = (float)g / 255;
    float bdash = (float)b / 255;

    float cmax = float_max(rdash, gdash, bdash);
    float cmin = float_min(rdash, gdash, bdash);
    float d = cmax - cmin;

    float h;
    float s;
    float l = (cmax + cmin) / 2;

    if(d == 0)
        h = 0;
    if(cmax == rdash)
        h = 60.0f * fmod((gdash - bdash) / d , 6);
    if(cmax == gdash)
        h = 60.0f * fmod((bdash - rdash) + 2 , 6);
    if(cmax == bdash)
        h = 60.0f * fmod((rdash - gdash) + 4 , 6);

    if(d)
        s = (d/(1 - fabs(2 * l - 1)));
    else
        s = 0;

    colors.hue = h;
    colors.saturation = s;
    colors.lightness = l;

    return colors;
}

uint32_t hsltorgb(hsl color) {
    float c = (1 - fabs(2 * color.lightness - 1)) * color.saturation;
    float x = c * (1 - fabs(fmod(color.hue / 60, 2) - 1));
    float m = color.lightness - c/2;

    float rdash = 0;
    float gdash = 0;
    float bdash = 0;

    int r = 0;
    int g = 0;
    int b = 0;

    if (color.hue >= 0 || color.hue < 60) {
        rdash = c;
        gdash = x;
    }
    if (color.hue >= 60 || color.hue < 120) {
        rdash = x;
        gdash = c;
    }
    if (color.hue >= 120 || color.hue < 180) {
        gdash = c;
        bdash = x;
    }
    if (color.hue >= 180 || color.hue < 240) {
        gdash = x;
        bdash = c;
    }
    if (color.hue >= 240 || color.hue < 300) {
        rdash = x;
        bdash = c;
    }
    if (color.hue >= 300 || color.hue < 360) {
        rdash = c;
        bdash = x;
    }

    r = (rdash + m) * 255;
    g = (gdash + m) * 255;
    b = (bdash + m) * 255;

    uint32_t colors;
    colors = r << 16 | g << 8 | b;
    return colors;
}

float float_max(float r, float g, float b) {
    float x = r;
    if(g > x)
        x = g;
    if(b > x)
        x = b;
    return x;
}

float float_min(float r, float g, float b) {
    float x = r;
    if(g < x)
        x = g;
    if(b < x)
        x = b;
    return x;
}

uint32_t color_rgbatorgb(uint32_t bgcolor, uint64_t color) {
    int transparency = (color & 0xff000000) >> 24;
    float alpha = (float)transparency / 255;
    float ralpha = 1 - alpha;

    int rgb[3];

    for(int i = 0; i < 3; i++) {
        int bc = bgcolor & (0xff << (8 * (2 - i)));
        int c = color & (0xff << (8 * (2 - i)));
        int col = ceil(ralpha * bc + alpha * c);
        col &= (0xff << (8 * (2 - i))); // big sacrifice to accuracy!!!
        col += 0x010000;
        rgb[i] = col;
    }

    return rgb[0] | rgb[1] | rgb[2];
}