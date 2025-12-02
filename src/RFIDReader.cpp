#include "RFIDReader.h"

RFIDReader::RFIDReader() : rfidSerial(PIN_RFID_RX, PIN_RFID_TX) {}

void RFIDReader::init() {
    rfidSerial.begin(9600);
    rfidSerial.setTimeout(100); // Make reading faster (don't wait 1s)
}

bool RFIDReader::readCard(char* buffer, byte maxLen) {
    if (rfidSerial.available()) {
        // Create a temporary buffer to catch the data
        char tempBuf[15]; 
        
        // Read up to 12 bytes (ID + Checksum)
        // We know your module sends at least 10 bytes for the ID
        int count = rfidSerial.readBytes(tempBuf, 12); 
        
        if (count >= 10) {
            // We got enough data! 
            // Copy the first 10 characters (the UID) into the main buffer
            strncpy(buffer, tempBuf, 10);
            buffer[10] = '\0'; // Ensure it is null-terminated
            
            // Clear any leftover checksum data from the serial buffer
            while(rfidSerial.available()) rfidSerial.read();
            
            return true;
        }
    }
    return false;
}