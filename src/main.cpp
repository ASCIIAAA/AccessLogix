// src/main.cpp

#include <Arduino.h>
#include "RFIDReader.h"
#include "LcdDisplay.h"
#include "RTCClock.h"
#include "KeypadReader.h" 
#include <SPI.h>  
#include <SD.h>
//#include<SdFat.h>
// --- System State Enum ---
enum SystemState {
    WAITING_FOR_CARD,
    WAITING_FOR_PIN,
    ACCESS_GRANTED,
    ACCESS_DENIED
};
SystemState currentState = WAITING_FOR_CARD;

// --- System Globals ---
RFIDReader rfidReader;
LcdDisplay lcd;
RTCClock rtcClock;
KeypadReader keypad; 

// --- Pin Definitions (UPDATED PINS) ---
#define GREEN_LED 2
#define RED_LED   5
#define BUZZER    3
#define SD_CS_PIN 4
#define RFID_CS_PIN 10

// --- Access Control ---
const byte ALLOWED_UID[] = {0x79, 0x8C, 0xD5, 0x05};
const byte UID_SIZE = 4;
const String CORRECT_PIN = "1234";
String enteredPin = "";
unsigned long pinEntryStartTime = 0;
const unsigned long PIN_TIMEOUT = 10000; // 10 seconds to enter PIN

void signalGrant() {
    // Blink green LED 3 times quickly for "Access Granted"
    for (int i = 0; i < 3; i++) {
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(BUZZER, HIGH); 
        delay(75);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(BUZZER, LOW);  
        delay(100);
    }
}

void signalDeny() {
    // Blink red LED and beep buzzer twice for "Access Denied"
    for (int i = 0; i < 2; i++) {
        digitalWrite(RED_LED, HIGH);
        digitalWrite(BUZZER, HIGH); 
        delay(250); 
        digitalWrite(RED_LED, LOW);
        digitalWrite(BUZZER, LOW);  
        delay(300);
    }
}

bool checkCard(byte *scannedUID, byte scannedSize) {
    if (scannedSize != UID_SIZE) return false;
    for (byte i = 0; i < UID_SIZE; i++) {
        if (scannedUID[i] != ALLOWED_UID[i]) return false;
    }
    return true;
}

void resetToReadyState() {
    enteredPin = "";
    currentState = WAITING_FOR_CARD;
    lcd.showMessage("Scan your card", rtcClock.getFormattedTime().c_str());
    Serial.println("\nSystem Ready. Scan your allowed card...");
}

// src/main.cpp

void setup() {
    Serial.begin(115200);
    
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    
    // --- THIS IS THE NEW PART ---
    // Explicitly set up CS pins as OUTPUT
    pinMode(RFID_CS_PIN, OUTPUT);
    pinMode(SD_CS_PIN, OUTPUT);

    // Force BOTH devices to be DESELECTED (HIGH) before starting.
    digitalWrite(RFID_CS_PIN, HIGH);
    digitalWrite(SD_CS_PIN, HIGH); 
    // --- END NEW PART ---
    
    lcd.init();
    rtcClock.init();
    
    // --- MODIFIED SECTION ---
    // Now, with the SD card FORCED high (deselected), initialize the RFID reader.
    Serial.println("Initializing RFID Reader...");
    rfidReader.init();
    Serial.println("RFID Reader Init done.");
    // --- END MODIFIED SECTION ---

    
    Serial.println("Initializing SD card...");
    lcd.showMessage("SD Card", "Initializing...");
    
    // --- MODIFIED SECTION ---
    // Force RFID pin HIGH (deselected) before initializing SD card.
    digitalWrite(RFID_CS_PIN, HIGH);
    
    if (!SD.begin(SD_CS_PIN)) {
        Serial.println("SD Card initialization FAILED!");
        lcd.showMessage("SD Card ERROR", "Check wiring");
        while (1); // Halt
    }
    // Deselect the SD card again, just to be safe.
    digitalWrite(SD_CS_PIN, HIGH); 
    // --- END MODIFIED SECTION ---
    
    Serial.println("SD card initialized.");
    lcd.showMessage("SD Card OK", "");
    delay(1000); 
    currentState = WAITING_FOR_CARD;
    resetToReadyState();
}

void handleCardScan() {
    digitalWrite(SD_CS_PIN, HIGH);
    if (rfidReader.readCard()) {
        byte* uid = rfidReader.getUID();
        byte size = rfidReader.getUIDSize();
        
        Serial.print("Scanned UID: ");
        rfidReader.printUID(uid, size);

        if (checkCard(uid, size)) {
            currentState = WAITING_FOR_PIN;
            pinEntryStartTime = millis();
            enteredPin = "";
           lcd.showMessage("Enter PIN:", "");
        } else {
            Serial.println("Card Denied!");
            lcd.showMessage("Card Denied!", "");
            signalDeny(); // <-- This will now call the fixed function
            delay(2000);
            resetToReadyState();
        }
        rfidReader.haltCard();
    }
}

void handlePinEntry() {
    // Check for PIN entry timeout
    if (millis() - pinEntryStartTime > PIN_TIMEOUT) {
        Serial.println("PIN entry timed out!");
       lcd.showMessage("Timeout!", "");
        signalDeny();
        delay(2000);
        resetToReadyState();
        return;
    }

    char key = keypad.getKey();
    if (key) {
        if (key == '#') { // '#' is the ENTER key
            if (enteredPin == CORRECT_PIN) {
                currentState = ACCESS_GRANTED;
            } else {
                currentState = ACCESS_DENIED;
            }
        } else if (key == '*') { // '*' is the CLEAR key
            enteredPin = "";
           lcd.showMessage("Enter PIN:", "");
        } else {
            enteredPin += key;
            String asterisks = "";
            for (unsigned int i = 0; i < enteredPin.length(); i++) { // Fixed unsigned int warning
                asterisks += "*";
            }
           lcd.showMessage("Enter PIN:", asterisks.c_str());
        }
    }
}

void loop() {
    switch (currentState) {
        case WAITING_FOR_CARD:
            // Every second, update the time on the LCD
            if (millis() % 1000 == 0) {
                lcd.showMessage("Scan your card", rtcClock.getFormattedTime().c_str());
            }
            handleCardScan();
            break;

        case WAITING_FOR_PIN:
            handlePinEntry();
            break;

        case ACCESS_GRANTED:
            Serial.println("Access Granted! 🟢");
           lcd.showMessage("Access Granted!", rtcClock.getFormattedTime().c_str());
            signalGrant(); // <-- This will now call the fixed function
            delay(3000);
            resetToReadyState();
            break;

        case ACCESS_DENIED:
            Serial.println("Access Denied! 🔴");
            lcd.showMessage("Access Denied!", "Wrong PIN");
            signalDeny(); // <-- This will now call the fixed function
            delay(3000);
            resetToReadyState();
            break;
    }
}