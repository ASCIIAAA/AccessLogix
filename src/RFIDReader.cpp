// src/RFIDReader.cpp

#include "RFIDReader.h"
#include "Arduino.h" // Needed for Serial.print functions
#include <Arduino.h>

// src/RFIDReader.cpp

// Constructor: Initializes the MFRC522 object with the defined pins
// NOTE: RFID_SS_PIN and RFID_RST_PIN are available because Config.h is included in RFIDReader.h
RFIDReader::RFIDReader() : rfid(RFID_SS_PIN, RFID_RST_PIN) { 
    // Note: SPI and PCD_Init must be called in init()
}
// ... rest of the file

// Initialization function
void RFIDReader::init() {
    SPI.begin();
    rfid.PCD_Init();
    Serial.println("RFID Reader Initialized.");
}

// Function to read the card
bool RFIDReader::readCard() {
    // Check for new card and read its serial (UID)
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
        return false; // No new card or failed to read
    }

    // A card was read successfully. Copy the UID to the internal buffer.
    for (byte i = 0; i < rfid.uid.size; i++) {
        currentUID[i] = rfid.uid.uidByte[i];
    }
    currentUID[rfid.uid.size] = '\0'; // Null-terminate if treated as string (optional for byte array)

    // Halt the PICC (card) to deselect it, ready for the next reading
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    return true; // Successfully read card
}

// Returns the pointer to the last read UID
byte* RFIDReader::getUID() {
    return currentUID;
}

// Returns the size of the last read UID (needed for print and comparison)
byte RFIDReader::getUIDSize() {
    return rfid.uid.size;
}

// Debug utility: prints UID
void RFIDReader::printUID(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        if (buffer[i] < 0x10) {
            Serial.print("0");
        }
        Serial.print(buffer[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

// Debug utility: prints firmware version
void RFIDReader::dumpVersionToSerial() {
    rfid.PCD_DumpVersionToSerial();
}