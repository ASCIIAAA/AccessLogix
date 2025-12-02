#include "RFIDReader.h"

RFIDReader::RFIDReader() : rfidSerial(PIN_RFID_RX, PIN_RFID_TX) {}

void RFIDReader::init() {
    rfidSerial.begin(9600);
}

bool RFIDReader::readCard(char* buffer, byte maxLen) {
    if (rfidSerial.available()) {
        if (rfidSerial.read() == 0x02) { // Start Byte
            char tempBuf[13];
            int readBytes = rfidSerial.readBytes(tempBuf, 12); // Read 12 (10 ID + 2 Checksum)
            
            if (readBytes == 12) {
                tempBuf[10] = '\0'; // Null terminate after ID
                
                // Copy safely to the main buffer
                strncpy(buffer, tempBuf, maxLen);
                
                // Clear the rest of the serial buffer
                while(rfidSerial.available()) rfidSerial.read();
                
                return true; 
            }
        }
    }
    return false;
}