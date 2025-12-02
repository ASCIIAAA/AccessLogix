#include "RFIDReader.h"

RFIDReader::RFIDReader() : rfidSerial(PIN_RFID_RX, PIN_RFID_TX) {}

void RFIDReader::init() {
    rfidSerial.begin(9600);
    rfidSerial.setTimeout(100); 
}

bool RFIDReader::readCard(char* buffer, byte maxLen) {
    if (rfidSerial.available()) {
        char tempBuf[15]; 
        
        int count = rfidSerial.readBytes(tempBuf, 12); 
        
        if (count >= 10) {
            strncpy(buffer, tempBuf, 10);
            buffer[10] = '\0'; 
            
            while(rfidSerial.available()) rfidSerial.read();
            
            return true;
        }
    }
    return false;
}