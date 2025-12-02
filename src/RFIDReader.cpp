#include "RFIDReader.h"

RFIDReader::RFIDReader() : rfidSerial(PIN_RFID_RX, PIN_RFID_TX) {
}

void RFIDReader::init() {
    rfidSerial.begin(9600);
}

String RFIDReader::readCard() {
    if (rfidSerial.available()) {
        if (rfidSerial.read() == 0x02) { // Start Byte
            char buf[13];
            // Read 12 bytes (10 ID + 2 Checksum)
            int readBytes = rfidSerial.readBytes(buf, 12);
            if (readBytes == 12) {
                buf[10] = '\0'; // Null terminate after the 10th char (ignoring checksum)
                
                // Clear buffer
                while(rfidSerial.available()) rfidSerial.read();
                
                return String(buf);
            }
        }
    }
    return "";
}