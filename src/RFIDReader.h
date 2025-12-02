#ifndef RFID_READER_H
#define RFID_READER_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "config.h"

class RFIDReader {
public:
    RFIDReader();
    void init();
    // CHANGED: Returns true if card found, puts ID into 'buffer'
    bool readCard(char* buffer, byte maxLen); 
    
private:
    SoftwareSerial rfidSerial;
};

#endif