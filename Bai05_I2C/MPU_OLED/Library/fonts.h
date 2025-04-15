#ifndef _FONTS_H
#define _FONTS_H

#include "stm32f10x.h"                  // Device header
#include "string.h"

typedef struct{
	uint8_t FontWidth;
	uint8_t FontHeight;
	const uint16_t *data;
} FontDef_t;

typedef struct{
	uint8_t Length;
	uint8_t Height;
} FONTS_SIZE_t;

extern FontDef_t Font_7x10;
char* FONTS_GetStringSize(char *str,FONTS_SIZE_t* SizeStruct,FontDef_t* Font);

#endif
