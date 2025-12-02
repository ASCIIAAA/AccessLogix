#include <Arduino.h>
#include "config.h"
#include "RFIDReader.h"
#include "LcdDisplay.h"
#include "RTCClock.h"
#include "KeypadReader.h"
#include "UserDB.h"
#include "Logger.h"

// Globals
char uidBuffer[13];      
char timeBuffer[20];     
char enteredPass[10]; 
int passIndex = 0;  

// Objects
RFIDReader rfid;
LcdDisplay lcd;
RTCClock rtc;
KeypadReader keypad;
UserDB userDB;
Logger logger;
User* currentUser = nullptr;

enum SystemState { WAITING_FOR_CARD, WAITING_FOR_PASS, ACCESS_GRANTED, ACCESS_DENIED };
SystemState currentState = WAITING_FOR_CARD;

void signal(bool success) {
    if (success) {
        digitalWrite(PIN_LED_GREEN, HIGH);
        digitalWrite(PIN_BUZZER, HIGH);
        delay(100);
        digitalWrite(PIN_BUZZER, LOW);
        delay(100);
        digitalWrite(PIN_BUZZER, HIGH);
        delay(100);
        digitalWrite(PIN_BUZZER, LOW);
        digitalWrite(PIN_LED_GREEN, LOW);
    } else {
        digitalWrite(PIN_LED_RED, HIGH);
        digitalWrite(PIN_BUZZER, HIGH);
        delay(1000);
        digitalWrite(PIN_BUZZER, LOW);
        digitalWrite(PIN_LED_RED, LOW);
    }
}

void resetSystem() {
    currentUser = nullptr;
    memset(enteredPass, 0, sizeof(enteredPass));
    passIndex = 0;
    currentState = WAITING_FOR_CARD;
    
    // UPDATED: Using F() macro
    lcd.showMessage(F("System Ready"), F("Scan Card..."));
    Serial.println(F("\n--- WAITING FOR CARD ---"));
}

void setup() {
    Serial.begin(115200);
    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);

    lcd.init();
    // UPDATED: Using F() macro
    lcd.showMessage(F("Booting..."), F("System Init"));
    
    rtc.init();
    rfid.init();
    userDB.init(); 
    logger.init(); 

    Serial.println(F("System Initialized."));
    resetSystem();
}

void loop() {
    switch (currentState) {
        case WAITING_FOR_CARD: {
            if (rfid.readCard(uidBuffer, sizeof(uidBuffer))) {
                Serial.print(F("Card Scanned: "));
                Serial.println(uidBuffer);
                
                currentUser = userDB.findUser(uidBuffer);

                if (currentUser != nullptr) {
                    if (currentUser->isActive()) {
                        Serial.print(F("User Found: "));
                        Serial.println(currentUser->getName());
                        
                        lcd.clear();
                        // Mix of F() and normal variables works because print handles both
                        lcd.showMessage("User Found:", currentUser->getName());
                        delay(1000);
                        lcd.showMessage(F("Enter Pass:"), F("Press * when done"));
                        
                        currentState = WAITING_FOR_PASS;
                        passIndex = 0;
                        memset(enteredPass, 0, sizeof(enteredPass));
                    } else {
                        Serial.println(F("User Inactive"));
                        lcd.showMessage(F("Access Denied"), F("Card Blocked"));
                        
                        rtc.getFormattedTime(timeBuffer);
                        logger.logAccess(currentUser, "DENIED_BLOCKED", timeBuffer);
                        
                        signal(false);
                        delay(2000);
                        resetSystem();
                    }
                } else {
                    Serial.println(F("Unknown Card"));
                    lcd.showMessage(F("Unknown Card"), F("Access Denied"));
                    
                    rtc.getFormattedTime(timeBuffer);
                    logger.logAccess(nullptr, "DENIED_UNKNOWN", timeBuffer);
                    
                    signal(false);
                    delay(2000);
                    resetSystem();
                }
            }
            break;
        }

        case WAITING_FOR_PASS: {
            char key = keypad.getKey();
            if (key) {
                if (key == '*') {
                    if (currentUser->checkPassword(enteredPass)) {
                        currentState = ACCESS_GRANTED;
                    } else {
                        currentState = ACCESS_DENIED;
                    }
                } else if (key == '#') {
                    passIndex = 0;
                    memset(enteredPass, 0, sizeof(enteredPass));
                    lcd.showMessage(F("Pass Cleared"), F("Enter Again + *"));
                } else {
                    if (passIndex < 9) {
                        enteredPass[passIndex] = key;
                        passIndex++;
                        enteredPass[passIndex] = '\0';
                        
                        lcd.showMessage("Pass:", "Processing...");
                        Serial.print(F("*"));
                    }
                }
            }
            break;
        }

        case ACCESS_GRANTED: {
            Serial.println(F("Access Granted"));
            lcd.showMessage(F("Access Granted"), F("Welcome!"));
            
            rtc.getFormattedTime(timeBuffer);
            logger.logAccess(currentUser, "GRANTED", timeBuffer);
            
            signal(true);
            delay(3000);
            resetSystem();
            break;
        }

        case ACCESS_DENIED: {
            Serial.println(F("Wrong Password"));
            lcd.showMessage(F("Wrong Password"), F("Access Denied"));
            
            rtc.getFormattedTime(timeBuffer);
            logger.logAccess(currentUser, "DENIED_PASSWORD", timeBuffer);
            
            signal(false);
            delay(2000);
            resetSystem();
            break;
        }
    }
}