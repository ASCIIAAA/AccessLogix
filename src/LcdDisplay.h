#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

class LcdDisplay {
public:
    LcdDisplay();
    void init();
    void clear();
    // Changed String to const char* to save RAM
    void showMessage(const char* line1, const char* line2);
    void showMessage(String line1, String line2); // Overload for String objects

private:
    LiquidCrystal_I2C lcd; 
};

#endif