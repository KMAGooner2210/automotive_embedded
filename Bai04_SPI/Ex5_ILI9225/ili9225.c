#include "ili9225.h"
#include "fonts.h"
#include <string.h>

static ILI9225_GPIO_t ili9225_gpio;
static uint16_t _maxX, _maxY;
static uint8_t _orientation;

// Private function prototypes
static void ILI9225_Reset(void);
static void ILI9225_Delay(uint32_t delay);
static void ILI9225_GPIO_Write(GPIO_TypeDef* port, uint16_t pin, uint8_t state);
static void ILI9225_Swap(uint16_t *a, uint16_t *b);
static void ILI9225_OrientCoordinates(uint16_t *x, uint16_t *y);

// Simple delay function
static void ILI9225_Delay(uint32_t delay) {
    for(volatile uint32_t i = 0; i < delay * 1000; i++);
}

// GPIO write function
static void ILI9225_GPIO_Write(GPIO_TypeDef* port, uint16_t pin, uint8_t state) {
    if(state) {
        port->BSRR = pin;
    } else {
        port->BRR = pin;
    }
}

// Swap function
static void ILI9225_Swap(uint16_t *a, uint16_t *b) {
    uint16_t temp = *a;
    *a = *b;
    *b = temp;
}

// Reset function
static void ILI9225_Reset(void) {
    ILI9225_GPIO_Write(ili9225_gpio.rst_port, ili9225_gpio.rst_pin, 1);
    ILI9225_Delay(1);
    ILI9225_GPIO_Write(ili9225_gpio.rst_port, ili9225_gpio.rst_pin, 0);
    ILI9225_Delay(10);
    ILI9225_GPIO_Write(ili9225_gpio.rst_port, ili9225_gpio.rst_pin, 1);
    ILI9225_Delay(50);
}

void ILI9225_Init(ILI9225_GPIO_t* gpio) {
    memcpy(&ili9225_gpio, gpio, sizeof(ILI9225_GPIO_t));
    
    // Initialize GPIO pins
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // Enable clocks
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
    
    // RST pin
    GPIO_InitStructure.GPIO_Pin = gpio->rst_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(gpio->rst_port, &GPIO_InitStructure);
    
    // RS pin
    GPIO_InitStructure.GPIO_Pin = gpio->rs_pin;
    GPIO_Init(gpio->rs_port, &GPIO_InitStructure);
    
    // CS pin  
    GPIO_InitStructure.GPIO_Pin = gpio->cs_pin;
    GPIO_Init(gpio->cs_port, &GPIO_InitStructure);
    
    // LED pin (backlight)
    if(gpio->led_pin != 0) {
        GPIO_InitStructure.GPIO_Pin = gpio->led_pin;
        GPIO_Init(gpio->led_port, &GPIO_InitStructure);
        ILI9225_SetBacklight(true);
    }
    
    // Initialize SPI2
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    
    // Configure SPI pins: PB13(SCK), PB15(MOSI)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);
    
    SPI_Cmd(SPI2, ENABLE);
    
    // Reset sequence
    ILI9225_Reset();
    
    // Initialization sequence from original Arduino code
    ILI9225_WriteRegister(ILI9225_POWER_CTRL1, 0x0000);
    ILI9225_WriteRegister(ILI9225_POWER_CTRL2, 0x0000);
    ILI9225_WriteRegister(ILI9225_POWER_CTRL3, 0x0000);
    ILI9225_WriteRegister(ILI9225_POWER_CTRL4, 0x0000);
    ILI9225_WriteRegister(ILI9225_POWER_CTRL5, 0x0000);
    ILI9225_Delay(40);

    // Power-on sequence
    ILI9225_WriteRegister(ILI9225_POWER_CTRL2, 0x0018);
    ILI9225_WriteRegister(ILI9225_POWER_CTRL3, 0x6121);
    ILI9225_WriteRegister(ILI9225_POWER_CTRL4, 0x006F);
    ILI9225_WriteRegister(ILI9225_POWER_CTRL5, 0x495F);
    ILI9225_WriteRegister(ILI9225_POWER_CTRL1, 0x0800);
    ILI9225_Delay(10);
    ILI9225_WriteRegister(ILI9225_POWER_CTRL2, 0x103B);
    ILI9225_Delay(50);

    ILI9225_WriteRegister(ILI9225_DRIVER_OUTPUT_CTRL, 0x011C);
    ILI9225_WriteRegister(ILI9225_LCD_AC_DRIVING_CTRL, 0x0100);
    ILI9225_WriteRegister(ILI9225_ENTRY_MODE, 0x1030);
    ILI9225_WriteRegister(ILI9225_DISP_CTRL1, 0x0000);
    ILI9225_WriteRegister(ILI9225_BLANK_PERIOD_CTRL1, 0x0808);
    ILI9225_WriteRegister(ILI9225_FRAME_CYCLE_CTRL, 0x1100);
    ILI9225_WriteRegister(ILI9225_INTERFACE_CTRL, 0x0000);
    ILI9225_WriteRegister(ILI9225_OSC_CTRL, 0x0D01);
    ILI9225_WriteRegister(ILI9225_VCI_RECYCLING, 0x0020);
    ILI9225_WriteRegister(ILI9225_RAM_ADDR_SET1, 0x0000);
    ILI9225_WriteRegister(ILI9225_RAM_ADDR_SET2, 0x0000);

    /* Set GRAM area */
    ILI9225_WriteRegister(ILI9225_GATE_SCAN_CTRL, 0x0000);
    ILI9225_WriteRegister(ILI9225_VERTICAL_SCROLL_CTRL1, 0x00DB);
    ILI9225_WriteRegister(ILI9225_VERTICAL_SCROLL_CTRL2, 0x0000);
    ILI9225_WriteRegister(ILI9225_VERTICAL_SCROLL_CTRL3, 0x0000);
    ILI9225_WriteRegister(ILI9225_PARTIAL_DRIVING_POS1, 0x00DB);
    ILI9225_WriteRegister(ILI9225_PARTIAL_DRIVING_POS2, 0x0000);
    ILI9225_WriteRegister(ILI9225_HORIZONTAL_WINDOW_ADDR1, 0x00AF);
    ILI9225_WriteRegister(ILI9225_HORIZONTAL_WINDOW_ADDR2, 0x0000);
    ILI9225_WriteRegister(ILI9225_VERTICAL_WINDOW_ADDR1, 0x00DB);
    ILI9225_WriteRegister(ILI9225_VERTICAL_WINDOW_ADDR2, 0x0000);

    /* Set GAMMA curve */
    ILI9225_WriteRegister(ILI9225_GAMMA_CTRL1, 0x0000);
    ILI9225_WriteRegister(ILI9225_GAMMA_CTRL2, 0x0808);
    ILI9225_WriteRegister(ILI9225_GAMMA_CTRL3, 0x080A);
    ILI9225_WriteRegister(ILI9225_GAMMA_CTRL4, 0x000A);
    ILI9225_WriteRegister(ILI9225_GAMMA_CTRL5, 0x0A08);
    ILI9225_WriteRegister(ILI9225_GAMMA_CTRL6, 0x0808);
    ILI9225_WriteRegister(ILI9225_GAMMA_CTRL7, 0x0000);
    ILI9225_WriteRegister(ILI9225_GAMMA_CTRL8, 0x0A00);
    ILI9225_WriteRegister(ILI9225_GAMMA_CTRL9, 0x0710);
    ILI9225_WriteRegister(ILI9225_GAMMA_CTRL10, 0x0710);

    ILI9225_WriteRegister(ILI9225_DISP_CTRL1, 0x0012);
    ILI9225_Delay(50);
    ILI9225_WriteRegister(ILI9225_DISP_CTRL1, 0x1017);

    ILI9225_SetOrientation(0);
    ILI9225_Clear();
}

void ILI9225_WriteCommand(uint8_t command) {
    ILI9225_GPIO_Write(ili9225_gpio.rs_port, ili9225_gpio.rs_pin, 0); // Command mode
    ILI9225_GPIO_Write(ili9225_gpio.cs_port, ili9225_gpio.cs_pin, 0); // CS low
    
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI2, command);
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    SPI_I2S_ReceiveData(SPI2);
    
    ILI9225_GPIO_Write(ili9225_gpio.cs_port, ili9225_gpio.cs_pin, 1); // CS high
}

void ILI9225_WriteData(uint16_t data) {
    ILI9225_GPIO_Write(ili9225_gpio.rs_port, ili9225_gpio.rs_pin, 1); // Data mode
    ILI9225_GPIO_Write(ili9225_gpio.cs_port, ili9225_gpio.cs_pin, 0); // CS low
    
    uint8_t high_byte = data >> 8;
    uint8_t low_byte = data & 0xFF;
    
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI2, high_byte);
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    SPI_I2S_ReceiveData(SPI2);
    
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI2, low_byte);
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    SPI_I2S_ReceiveData(SPI2);
    
    ILI9225_GPIO_Write(ili9225_gpio.cs_port, ili9225_gpio.cs_pin, 1); // CS high
}

void ILI9225_WriteRegister(uint16_t reg, uint16_t data) {
    ILI9225_WriteCommand(reg >> 8);
    ILI9225_WriteCommand(reg & 0xFF);
    ILI9225_WriteData(data);
}

static void ILI9225_OrientCoordinates(uint16_t *x, uint16_t *y) {
    uint16_t temp;
    switch (_orientation) {
        case 0:
            break;
        case 1:
            *y = _maxY - *y - 1;
            temp = *x;
            *x = *y;
            *y = temp;
            break;
        case 2:
            *x = _maxX - *x - 1;
            *y = _maxY - *y - 1;
            break;
        case 3:
            *x = _maxX - *x - 1;
            temp = *x;
            *x = *y;
            *y = temp;
            break;
    }
}

void ILI9225_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    uint16_t x0_temp = x0, y0_temp = y0, x1_temp = x1, y1_temp = y1;
    
    ILI9225_OrientCoordinates(&x0_temp, &y0_temp);
    ILI9225_OrientCoordinates(&x1_temp, &y1_temp);

    if (x1_temp < x0_temp) ILI9225_Swap(&x0_temp, &x1_temp);
    if (y1_temp < y0_temp) ILI9225_Swap(&y0_temp, &y1_temp);

    ILI9225_WriteRegister(ILI9225_HORIZONTAL_WINDOW_ADDR1, x1_temp);
    ILI9225_WriteRegister(ILI9225_HORIZONTAL_WINDOW_ADDR2, x0_temp);
    ILI9225_WriteRegister(ILI9225_VERTICAL_WINDOW_ADDR1, y1_temp);
    ILI9225_WriteRegister(ILI9225_VERTICAL_WINDOW_ADDR2, y0_temp);
    ILI9225_WriteRegister(ILI9225_RAM_ADDR_SET1, x0_temp);
    ILI9225_WriteRegister(ILI9225_RAM_ADDR_SET2, y0_temp);
    ILI9225_WriteCommand(0x00);
    ILI9225_WriteCommand(0x22);
}

void ILI9225_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if((x >= _maxX) || (y >= _maxY)) return;

    ILI9225_SetWindow(x, y, x+1, y+1);
    ILI9225_WriteData(color);
}

void ILI9225_FillScreen(uint16_t color) {
    ILI9225_FillRectangle(0, 0, _maxX-1, _maxY-1, color);
}

void ILI9225_SetOrientation(uint8_t orientation) {
    _orientation = orientation % 4;
    switch (_orientation) {
        case 0:
            _maxX = ILI9225_LCD_WIDTH;
            _maxY = ILI9225_LCD_HEIGHT;
            break;
        case 1:
            _maxX = ILI9225_LCD_HEIGHT;
            _maxY = ILI9225_LCD_WIDTH;
            break;
        case 2:
            _maxX = ILI9225_LCD_WIDTH;
            _maxY = ILI9225_LCD_HEIGHT;
            break;
        case 3:
            _maxX = ILI9225_LCD_HEIGHT;
            _maxY = ILI9225_LCD_WIDTH;
            break;
    }
}

void ILI9225_Clear(void) {
    ILI9225_FillScreen(COLOR_BLACK);
}

void ILI9225_SetBacklight(bool state) {
    if(ili9225_gpio.led_pin != 0) {
        ILI9225_GPIO_Write(ili9225_gpio.led_port, ili9225_gpio.led_pin, state);
    }
}

void ILI9225_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    // Bresenham's line algorithm
    int16_t steep = abs((int16_t)(y2 - y1)) > abs((int16_t)(x2 - x1));
    int16_t dx, dy;

    if (steep) {
        ILI9225_Swap(&x1, &y1);
        ILI9225_Swap(&x2, &y2);
    }

    if (x1 > x2) {
        ILI9225_Swap(&x1, &x2);
        ILI9225_Swap(&y1, &y2);
    }

    dx = x2 - x1;
    dy = abs((int16_t)(y2 - y1));

    int16_t err = dx / 2;
    int16_t ystep;

    if (y1 < y2) ystep = 1;
    else ystep = -1;

    for (; x1 <= x2; x1++) {
        if (steep) ILI9225_DrawPixel(y1, x1, color);
        else ILI9225_DrawPixel(x1, y1, color);

        err -= dy;
        if (err < 0) {
            y1 += ystep;
            err += dx;
        }
    }
}

void ILI9225_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    ILI9225_DrawLine(x1, y1, x2, y1, color);
    ILI9225_DrawLine(x1, y1, x1, y2, color);
    ILI9225_DrawLine(x1, y2, x2, y2, color);
    ILI9225_DrawLine(x2, y1, x2, y2, color);
}

void ILI9225_FillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    ILI9225_SetWindow(x1, y1, x2, y2);

    for(uint16_t t = (y2 - y1 + 1) * (x2 - x1 + 1); t > 0; t--) {
        ILI9225_WriteData(color);
    }
}

void ILI9225_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    ILI9225_DrawPixel(x0, y0 + r, color);
    ILI9225_DrawPixel(x0, y0 - r, color);
    ILI9225_DrawPixel(x0 + r, y0, color);
    ILI9225_DrawPixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ILI9225_DrawPixel(x0 + x, y0 + y, color);
        ILI9225_DrawPixel(x0 - x, y0 + y, color);
        ILI9225_DrawPixel(x0 + x, y0 - y, color);
        ILI9225_DrawPixel(x0 - x, y0 - y, color);
        ILI9225_DrawPixel(x0 + y, y0 + x, color);
        ILI9225_DrawPixel(x0 - y, y0 + x, color);
        ILI9225_DrawPixel(x0 + y, y0 - x, color);
        ILI9225_DrawPixel(x0 - y, y0 - x, color);
    }
}

void ILI9225_FillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
    ILI9225_DrawLine(x0 - r, y0, x0 + r, y0, color);
    
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ILI9225_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
        ILI9225_DrawLine(x0 - x, y0 - y, x0 + x, y0 - y, color);
        ILI9225_DrawLine(x0 - y, y0 + x, x0 + y, y0 + x, color);
        ILI9225_DrawLine(x0 - y, y0 - x, x0 + y, y0 - x, color);
    }
}

void ILI9225_DrawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg_color, const uint8_t* font) {
    uint8_t font_width = font[0];
    uint8_t font_height = font[1];
    uint8_t font_offset = font[2];
    uint8_t font_numchars = font[3];
    uint8_t nbrows = (font_height + 7) / 8;
    
    if (c < font_offset || c >= (font_offset + font_numchars)) return;
    
    uint16_t char_offset = (c - font_offset) * (font_width * nbrows + 1) + 4;
    uint8_t char_width = font[char_offset];
    char_offset++;
    
    for (uint8_t i = 0; i <= char_width; i++) {
        uint8_t h = 0;
        for (uint8_t j = 0; j < nbrows; j++) {
            uint8_t char_data;
            if (i == char_width) char_data = 0x00;
            else char_data = font[char_offset++];
            
            for (uint8_t k = 0; k < 8; k++) {
                if (h >= font_height) break;
                if (char_data & (1 << k)) {
                    ILI9225_DrawPixel(x + i, y + (j * 8) + k, color);
                } else if (bg_color != color) {
                    ILI9225_DrawPixel(x + i, y + (j * 8) + k, bg_color);
                }
                h++;
            }
        }
    }
}

void ILI9225_DrawString(uint16_t x, uint16_t y, const char* str, uint16_t color, uint16_t bg_color, const uint8_t* font) {
    uint16_t current_x = x;
    uint8_t font_width = font[0];
    
    while (*str) {
        ILI9225_DrawChar(current_x, y, *str, color, bg_color, font);
        current_x += font_width + 1;
        str++;
    }
}

uint16_t ILI9225_GetWidth(void) {
    return _maxX;
}

uint16_t ILI9225_GetHeight(void) {
    return _maxY;
}

uint16_t ILI9225_ColorFromRGB(uint8_t r, uint8_t g, uint8_t b) {
    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}