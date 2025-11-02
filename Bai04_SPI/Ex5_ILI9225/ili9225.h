#ifndef ILI9225_H
#define ILI9225_H

#include "stm32f10x.h"
#include <stdint.h>
#include <stdbool.h>

// ILI9225 LCD Registers
#define ILI9225_DRIVER_OUTPUT_CTRL      0x01
#define ILI9225_LCD_AC_DRIVING_CTRL     0x02
#define ILI9225_ENTRY_MODE              0x03
#define ILI9225_DISP_CTRL1              0x07
#define ILI9225_BLANK_PERIOD_CTRL1      0x08
#define ILI9225_FRAME_CYCLE_CTRL        0x0B
#define ILI9225_INTERFACE_CTRL          0x0C
#define ILI9225_OSC_CTRL                0x0F
#define ILI9225_POWER_CTRL1             0x10
#define ILI9225_POWER_CTRL2             0x11
#define ILI9225_POWER_CTRL3             0x12
#define ILI9225_POWER_CTRL4             0x13
#define ILI9225_POWER_CTRL5             0x14
#define ILI9225_VCI_RECYCLING           0x15
#define ILI9225_RAM_ADDR_SET1           0x20
#define ILI9225_RAM_ADDR_SET2           0x21
#define ILI9225_GRAM_DATA_REG           0x22
#define ILI9225_GATE_SCAN_CTRL          0x30
#define ILI9225_VERTICAL_SCROLL_CTRL1   0x31
#define ILI9225_VERTICAL_SCROLL_CTRL2   0x32
#define ILI9225_VERTICAL_SCROLL_CTRL3   0x33
#define ILI9225_PARTIAL_DRIVING_POS1    0x34
#define ILI9225_PARTIAL_DRIVING_POS2    0x35
#define ILI9225_HORIZONTAL_WINDOW_ADDR1 0x36
#define ILI9225_HORIZONTAL_WINDOW_ADDR2 0x37
#define ILI9225_VERTICAL_WINDOW_ADDR1   0x38
#define ILI9225_VERTICAL_WINDOW_ADDR2   0x39
#define ILI9225_GAMMA_CTRL1             0x50
#define ILI9225_GAMMA_CTRL2             0x51
#define ILI9225_GAMMA_CTRL3             0x52
#define ILI9225_GAMMA_CTRL4             0x53
#define ILI9225_GAMMA_CTRL5             0x54
#define ILI9225_GAMMA_CTRL6             0x55
#define ILI9225_GAMMA_CTRL7             0x56
#define ILI9225_GAMMA_CTRL8             0x57
#define ILI9225_GAMMA_CTRL9             0x58
#define ILI9225_GAMMA_CTRL10            0x59

#define ILI9225C_INVOFF                 0x20
#define ILI9225C_INVON                  0x21

// Color definitions (RGB565)
#define COLOR_BLACK         0x0000
#define COLOR_WHITE         0xFFFF
#define COLOR_BLUE          0x001F
#define COLOR_GREEN         0x07E0
#define COLOR_RED           0xF800
#define COLOR_NAVY          0x0008
#define COLOR_DARKBLUE      0x0011
#define COLOR_DARKGREEN     0x0320
#define COLOR_DARKCYAN      0x03EF
#define COLOR_CYAN          0x07FF
#define COLOR_TURQUOISE     0x04D0
#define COLOR_INDIGO        0x4810
#define COLOR_DARKRED       0x8000
#define COLOR_OLIVE         0x7BE0
#define COLOR_GRAY          0x8410
#define COLOR_SKYBLUE       0x867D
#define COLOR_BLUEVIOLET    0x895C
#define COLOR_LIGHTGREEN    0x9772
#define COLOR_DARKVIOLET    0x901A
#define COLOR_YELLOWGREEN   0x9E66
#define COLOR_BROWN         0xA145
#define COLOR_DARKGRAY      0xAD55
#define COLOR_SIENNA        0xA285
#define COLOR_LIGHTBLUE     0xA65C
#define COLOR_GREENYELLOW   0xAFE5
#define COLOR_SILVER        0xC618
#define COLOR_LIGHTGREY     0xD69A
#define COLOR_LIGHTCYAN     0xE7FF
#define COLOR_VIOLET        0xEC1D
#define COLOR_AZUR          0xF7FF
#define COLOR_BEIGE         0xF7BB
#define COLOR_MAGENTA       0xF81F
#define COLOR_TOMATO        0xFB08
#define COLOR_GOLD          0xFEA0
#define COLOR_ORANGE        0xFD20
#define COLOR_SNOW          0xFFDF
#define COLOR_YELLOW        0xFFE0

// Screen dimensions
#define ILI9225_LCD_WIDTH   176
#define ILI9225_LCD_HEIGHT  220

// GPIO pin definitions
typedef struct {
    GPIO_TypeDef* rst_port;
    uint16_t rst_pin;
    GPIO_TypeDef* rs_port;
    uint16_t rs_pin;
    GPIO_TypeDef* cs_port;
    uint16_t cs_pin;
    GPIO_TypeDef* led_port;
    uint16_t led_pin;
} ILI9225_GPIO_t;

// Function prototypes
void ILI9225_Init(ILI9225_GPIO_t* gpio);
void ILI9225_WriteCommand(uint8_t command);
void ILI9225_WriteData(uint16_t data);
void ILI9225_WriteRegister(uint16_t reg, uint16_t data);
void ILI9225_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void ILI9225_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void ILI9225_FillScreen(uint16_t color);
void ILI9225_DrawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg_color, const uint8_t* font);
void ILI9225_DrawString(uint16_t x, uint16_t y, const char* str, uint16_t color, uint16_t bg_color, const uint8_t* font);
void ILI9225_SetOrientation(uint8_t orientation);
void ILI9225_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void ILI9225_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void ILI9225_FillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void ILI9225_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void ILI9225_FillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void ILI9225_Clear(void);
void ILI9225_SetBacklight(bool state);

uint16_t ILI9225_GetWidth(void);
uint16_t ILI9225_GetHeight(void);
uint16_t ILI9225_ColorFromRGB(uint8_t r, uint8_t g, uint8_t b);
void ILI9225_Demo(void);
#endif