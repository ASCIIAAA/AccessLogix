// src/main.cpp

#include <Arduino.h>
#include "RFIDReader.h"
#include "LcdDisplay.h"
#include "RTCClock.h"
#include "KeypadReader.h" 

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
#define GREEN_LED A0
#define RED_LED   A1
#define BUZZER    A2

// --- Access Control ---
const byte ALLOWED_UID[] = {0x79, 0x8C, 0xD5, 0x05};
const byte UID_SIZE = 4;
const String CORRECT_PIN = "1234";
String enteredPin = "";
unsigned long pinEntryStartTime = 0;
const unsigned long PIN_TIMEOUT = 10000; // 10 seconds to enter PIN

// --- 
// 
// THIS IS THE FIXED CODE
// 
// ---
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
// --- END OF FIX ---


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

void setup() {
    Serial.begin(115200);
    // Pin setup
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);

    lcd.init();
    rtcClock.init();
    rfidReader.init();
    
    resetToReadyState();
}

void handleCardScan() {
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
        signalDeny(); // <-- This will now call the fixed function
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
            for (int i = 0; i < enteredPin.length(); i++) {
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