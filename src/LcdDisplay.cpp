#include "LcdDisplay.h"

LcdDisplay::LcdDisplay() : lcd(0x27, 16, 2) {}

void LcdDisplay::init() {
    lcd.init();
    lcd.backlight();
}

void LcdDisplay::clear() {
    lcd.clear();
}

void LcdDisplay::showMessage(const char* line1, const char* line2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
}

// NEW Implementation for Flash Strings
void LcdDisplay::showMessage(const __FlashStringHelper* line1, const __FlashStringHelper* line2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
}