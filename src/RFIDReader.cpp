// src/RFIDReader.cpp

#include "RFIDReader.h"
#include <Arduino.h>

// Define the pins here, as they are specific to the hardware this class controls
#define SS_PIN 10
#define RST_PIN 9

// Constructor: Initializes the MFRC522 object with the correct pins
RFIDReader::RFIDReader() : mfrc522(SS_PIN, RST_PIN) {
    // The ": mfrc522(SS_PIN, RST_PIN)" part is the C++ "initializer list".
    // It's the proper way to initialize member objects like mfrc522.
}

// init(): This is where the communication starts.
void RFIDReader::init() {
    SPI.begin();   
    delay(50);       // <<<--- CRITICAL: Start the SPI bus
    mfrc522.PCD_Init();   // <<<--- CRITICAL: Initialize the reader AFTER SPI is running
}

// Dumps the firmware version to serial for debugging
void RFIDReader::dumpVersionToSerial() {
    mfrc522.PCD_DumpVersionToSerial();
}

// Checks for a new card and reads its UID
bool RFIDReader::readCard() {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        // A card has been read successfully
        return true;
    }
    return false;
}

void RFIDReader::haltCard() {
    mfrc522.PICC_HaltA();      // Halt the card
    mfrc522.PCD_StopCrypto1(); // Stop encryption (good practice)
}

// Returns a pointer to the UID stored in the library
byte* RFIDReader::getUID() {
    return mfrc522.uid.uidByte;
}

// Returns the size of the UID
byte RFIDReader::getUIDSize() {
    return mfrc522.uid.size;
}

// Helper function to print the UID nicely
void RFIDReader::printUID(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
    Serial.println();
}