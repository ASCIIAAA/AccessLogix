#include "KeypadReader.h"

KeypadReader::KeypadReader() :
    keys {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    },
    rowPins{A3, A2, A1, A0}, 
    colPins{9, 8, 7, 6}, 
    keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS)
{}

char KeypadReader::getKey() {
    return keypad.getKey();
}