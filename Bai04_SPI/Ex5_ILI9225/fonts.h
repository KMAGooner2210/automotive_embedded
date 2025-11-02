#ifndef FONTS_H
#define FONTS_H

#include <stdint.h>

extern const uint8_t Terminal6x8[];
extern const uint8_t Terminal11x16[];
extern const uint8_t Terminal12x16[];
extern const uint8_t Trebuchet_MS16x21[];

typedef struct {
    const uint8_t* data;
    uint8_t width;
    uint8_t height;
    uint8_t offset;
    uint8_t numchars;
} FontDef;

#endif