#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

class LcdDisplay {
public:
    LcdDisplay();
    void init();
    void clear();
    
    void showMessage(const char* line1,const char* line2);
    void showMessage(const __FlashStringHelper* line1,const __FlashStringHelper* line2);
    void setCursor(uint8_t col,uint8_t row);
    void print(const char* message);
    void print(const __FlashStringHelper* message);
    void print(int number);
private:
    LiquidCrystal_I2C lcd; 
};

#endif