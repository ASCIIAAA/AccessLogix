#pragma once
#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <LiquidCrystal_I2C.h>

class LcdDisplay {
public:
    LcdDisplay(); // Constructor
    void init();
    void clear();
    void showMessage(const char* line1, const char* line2);
    void showReadyScreen();

private:
    // Initialize the LCD object with its I2C address, columns, and rows.
    // 0x27 is the most common address. If it doesn't work, yours might be 0x3F.
    LiquidCrystal_I2C lcd; 
};

#endif // LCD_DISPLAY_H