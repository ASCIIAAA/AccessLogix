// src/KeypadReader.cpp

#include "KeypadReader.h"

KeypadReader::KeypadReader() :
    keys {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    },
    rowPins{9, 8, 7, 6}, // Corresponds to R1, R2, R3, R4
    colPins{5, 4, 3, 2}, // Corresponds to C1, C2, C3, C4
    keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS)
{}

char KeypadReader::getKey() {
    return keypad.getKey();
}