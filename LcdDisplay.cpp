
#include "LcdDisplay.h"

// Constructor implementation
LcdDisplay::LcdDisplay() : lcd(0x27, 16, 2) {
    // The ": lcd(0x27, 16, 2)" part initializes the lcd object.
    // 0x27 is the I2C address, 16 is columns, 2 is rows.
}

void LcdDisplay::init() {
    lcd.init();      // Initialize the LCD
    lcd.backlight(); // Turn on the backlight
}

void LcdDisplay::clear() {
    lcd.clear();
}

// A versatile function to print two lines of text
void LcdDisplay::showMessage(const char* line1, const char* line2) {
    lcd.clear();
    lcd.setCursor(0, 0); // Move cursor to the first line
    lcd.print(line1);
    lcd.setCursor(0, 1); // Move cursor to the second line
    lcd.print(line2);
}

void LcdDisplay::showReadyScreen() {
    showMessage("System Ready", "Scan your card");
}