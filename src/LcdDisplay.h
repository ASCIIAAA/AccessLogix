#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

class LcdDisplay {
public:
    LcdDisplay();
    void init();
    void clear();
    
    // Normal char* support
    void showMessage(const char* line1, const char* line2);
    
    // NEW: Support for F() macro strings to save RAM
    void showMessage(const __FlashStringHelper* line1, const __FlashStringHelper* line2);

private:
    LiquidCrystal_I2C lcd; 
};

#endif