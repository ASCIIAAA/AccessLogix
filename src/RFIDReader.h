// include/RFIDReader.h

#ifndef RFID_READER_H
#define RFID_READER_H

// These two includes are required here
#include <SPI.h>
#include <MFRC522.h>

class RFIDReader {
public:
    RFIDReader(); // Constructor
    void init();
    bool readCard();
    void haltCard(); 
    void dumpVersionToSerial();
    void printUID(byte *buffer, byte bufferSize);
    byte* getUID();
    byte getUIDSize();

private:
    MFRC522 mfrc522; // The MFRC522 object is a member of the class
};

#endif // RFID_READER_H