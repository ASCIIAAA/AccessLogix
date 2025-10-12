// src/RFIDReader.h

#ifndef RFIDREADER_H
#define RFIDREADER_H

#include <SPI.h>
#include <MFRC522.h>
#include "Config.h" // For pin definitions and UID size

class RFIDReader {
private:
    MFRC522 rfid;
    byte currentUID[10]; // Buffer to hold the currently read UID (max 10 bytes)

public:
    // Constructor
    RFIDReader();

    // Initializes the SPI bus and the MFRC522 module
    void init();

    // Checks for a new card and reads its UID if present
    // Returns true if a new, valid UID was read and stored in currentUID
    bool readCard();

    // Returns the pointer to the last read UID
    byte* getUID();

    // Utility function to print the UID to Serial Monitor for debugging
    void printUID(byte *buffer, byte bufferSize);

    // Utility function to get the current UID size
    byte getUIDSize();
    
    // Utility for troubleshooting (like the one we used before)
    void dumpVersionToSerial();
};

#endif // RFIDREADER_H