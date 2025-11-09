// src/KeypadReader.cpp

#include "KeypadReader.h"

KeypadReader::KeypadReader() :
    keys {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    },
    colPins{0, 8, 7, 6}, 
    rowPins{A0,A1,A2,A3}, 
    keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS)
{}

char KeypadReader::getKey() {
    return keypad.getKey();
}