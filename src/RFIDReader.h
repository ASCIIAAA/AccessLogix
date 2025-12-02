#ifndef RFID_READER_H
#define RFID_READER_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "config.h"

class RFIDReader {
public:
    RFIDReader();
    void init();
    String readCard(); // Returns the 10-char UID string if present
    
private:
    SoftwareSerial rfidSerial;
};

#endif