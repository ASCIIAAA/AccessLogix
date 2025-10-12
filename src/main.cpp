// src/main.cpp

// CRITICAL: Must be included in any .cpp file that uses Arduino functions (setup, loop, Serial, pinMode, etc.)
#include <Arduino.h> 
#include "RFIDReader.h" 

// --- System Globals (Replaced from AccessLogix.ino) ---

// Create an instance of the RFIDReader class
RFIDReader rfidReader;

// Access Control Variables for testing
// Define your actual working UID here (4-byte example: 79 8C D5 05)
const byte ALLOWED_UID[] = {0x79, 0x8C, 0xD5, 0x05}; 
const byte UID_SIZE = 4; // Assuming 4-byte MIFARE Classic

// Define LED and Buzzer pins
#define GREEN_LED 6
#define RED_LED 7
#define BUZZER  8

// Helper function to check the scanned UID against the allowed list
// NOTE: This function can eventually be moved into a UserDB or SystemController class
bool checkAccess(byte *scannedUID, byte scannedSize) {
    if (scannedSize != UID_SIZE) {
        return false;
    }
    for (byte i = 0; i < UID_SIZE; i++) {
        if (scannedUID[i] != ALLOWED_UID[i]) {
            return false;
        }
    }
    return true;
}

// --- Arduino Required Functions ---

void setup() {
    // 1. Setup Serial
    Serial.begin(115200); 
    while (!Serial);

    // 2. Initialize the RFID Reader
    rfidReader.init();
    
    // CRITICAL TROUBLESHOOTING CHECK
    rfidReader.dumpVersionToSerial(); 

    // 3. Initialize GPIO for LEDs and Buzzer
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);

    Serial.println("System Ready. Scan your allowed card...");
}


void loop() {
    // 1. Check if a new card is presented and read successfully
    if (rfidReader.readCard()) {
        byte* uid = rfidReader.getUID();
        byte size = rfidReader.getUIDSize();

        Serial.print("Scanned UID: ");
        rfidReader.printUID(uid, size); // Use the class utility for clean printing

        // 2. Perform the access check
        if (checkAccess(uid, size)) {
            Serial.println("Access Granted! 🟢");
            digitalWrite(GREEN_LED, HIGH);
            digitalWrite(RED_LED, LOW);
            tone(BUZZER, 1000, 200);
        } else {
            Serial.println("Access Denied! 🔴");
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(RED_LED, HIGH);
            tone(BUZZER, 500, 500);
        }

        // Wait briefly and reset indicators
        delay(2000); 
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(RED_LED, LOW);
        noTone(BUZZER);
    }
}