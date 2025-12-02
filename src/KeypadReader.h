#ifndef KEYPAD_READER_H
#define KEYPAD_READER_H

#include <Keypad.h>

class KeypadReader {
public:
    KeypadReader();
    char getKey();

private:
    const static byte ROWS = 4;
    const static byte COLS = 4;
    char keys[ROWS][COLS];
    byte rowPins[ROWS];
    byte colPins[COLS];
    Keypad keypad;
};

#endif