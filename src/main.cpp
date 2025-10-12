// src/main.cpp

#include <Arduino.h>
#include "RFIDReader.h"
#include "LcdDisplay.h" // <-- Include the new LCD class header

// --- System Globals ---
RFIDReader rfidReader;
LcdDisplay lcd;


const byte ALLOWED_UID[] = {0x79, 0x8C, 0xD5, 0x05};
const byte UID_SIZE = 4;

#define GREEN_LED 6
#define RED_LED 7
#define BUZZER 8

// --- HELPER FUNCTIONS FOR SIGNALS (MODIFIED FOR ACTIVE BUZZER) ---

void signalGrant() {
    // Blink green LED 3 times quickly for "Access Granted"
    for (int i = 0; i < 3; i++) {
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(BUZZER, HIGH); // CHANGED: Turn buzzer ON
        delay(75);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(BUZZER, LOW);  // CHANGED: Turn buzzer OFF
        delay(100);
    }
}

void signalDeny() {
    // Blink red LED and beep buzzer twice for "Access Denied"
    for (int i = 0; i < 2; i++) {
        digitalWrite(RED_LED, HIGH);
        digitalWrite(BUZZER, HIGH); // CHANGED: Turn buzzer ON
        delay(250); // Keep it on for a longer duration
        digitalWrite(RED_LED, LOW);
        digitalWrite(BUZZER, LOW);  // CHANGED: Turn buzzer OFF
        delay(300);
    }
}

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

void setup() {
    Serial.begin(115200);
    while (!Serial);

    lcd.init(); // <-- Initialize the LCD
    lcd.showMessage("AccessLogix", "Initializing...");
    delay(1000);

    rfidReader.init();
    rfidReader.dumpVersionToSerial();

    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);

    Serial.println("System Ready. Scan your allowed card...");
    lcd.showReadyScreen(); // <-- Show the welcome message on the LCD
}

void loop() {
    if (rfidReader.readCard()) {
        byte* uid = rfidReader.getUID();
        byte size = rfidReader.getUIDSize();

        Serial.print("Scanned UID: ");
        rfidReader.printUID(uid, size);

        if (checkAccess(uid, size)) {
            Serial.println("Access Granted! 🟢");
            lcd.showMessage("Access Granted!", ""); // <-- Display on LCD
            signalGrant();
        } else {
            Serial.println("Access Denied! 🔴");
            lcd.showMessage("Access Denied!", ""); // <-- Display on LCD
            signalDeny();
        }
        
        rfidReader.haltCard();

        delay(2000); // Wait 2 seconds to show the message

        // Reset for the next scan
        Serial.println("\nSystem Ready. Scan your allowed card...");
        lcd.showReadyScreen();
    }
}