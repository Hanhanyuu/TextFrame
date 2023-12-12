#include "stdint.h"
#ifndef __LCD_DRIVER_H
#define __LCD_DRIVER_H
void LCD_DisplayChar(
    uint16_t Xpos,
    uint16_t Ypos,
    uint8_t Ascii,
    uint16_t Backcolor,
    uint16_t Textcolor
);

void PadColor(
    uint16_t Xpos,
    uint16_t Ypos,
    uint16_t Height,
    uint16_t Width,
    uint16_t Color
);
#endif