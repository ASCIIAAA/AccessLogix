#include <Arduino.h>
#include "config.h"
#include "RFIDReader.h"
#include "LcdDisplay.h"
#include "RTCClock.h"
#include "KeypadReader.h"
#include "UserDB.h"
#include "Logger.h"

enum SystemState {
    WAITING_FOR_CARD,
    WAITING_FOR_PASS,
    ACCESS_GRANTED,
    ACCESS_DENIED
};

SystemState currentState = WAITING_FOR_CARD;
RFIDReader rfid;
LcdDisplay lcd;
RTCClock rtc;
KeypadReader keypad;
UserDB userDB;
Logger logger;

User* currentUser = nullptr;
char enteredPass[10]; // Replaced String with Char Array
int passIndex = 0;    // Tracks current position in password

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
    // Reset password buffer
    memset(enteredPass, 0, sizeof(enteredPass));
    passIndex = 0;
    
    currentState = WAITING_FOR_CARD;
    lcd.showMessage("System Ready", "Scan Card...");
    Serial.println(F("\n--- WAITING FOR CARD ---"));
}

void setup() {
    Serial.begin(115200);
    delay(2000); 
    
    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);

    lcd.init();
    lcd.showMessage("Booting...", "System Init");
    
    rtc.init();
    rfid.init();
    userDB.init(); 
    logger.init(); 

    Serial.println(F("System Initialized."));
    delay(1000);
    resetSystem();
}

void loop() {
    switch (currentState) {
        case WAITING_FOR_CARD: {
            String uid = rfid.readCard();
            if (uid.length() > 0) {
                Serial.print(F("Card Scanned: "));
                Serial.println(uid);
                currentUser = userDB.findUser(uid);

                if (currentUser != nullptr) {
                    if (currentUser->isActive()) {
                        Serial.print(F("User Found: "));
                        Serial.println(currentUser->getName());
                        
                        lcd.clear();
                        lcd.showMessage("User Found:", currentUser->getName());
                        delay(1000);
                        lcd.showMessage("Enter Pass:", "Press * when done");
                        
                        currentState = WAITING_FOR_PASS;
                        // Reset password logic
                        passIndex = 0;
                        memset(enteredPass, 0, sizeof(enteredPass));
                    } else {
                        Serial.println(F("User Inactive/Blocked"));
                        lcd.showMessage("Access Denied", "Card Blocked");
                        logger.logAccess(currentUser, "DENIED_BLOCKED", rtc.getFormattedTime());
                        signal(false);
                        delay(2000);
                        resetSystem();
                    }
                } else {
                    Serial.println(F("Unknown Card"));
                    lcd.showMessage("Unknown Card", "Access Denied");
                    logger.logAccess(nullptr, "DENIED_UNKNOWN", rtc.getFormattedTime());
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
                    Serial.println(F("Verifying Password..."));
                    // Pass the char array to checkPassword
                    if (currentUser->checkPassword(enteredPass)) {
                        currentState = ACCESS_GRANTED;
                    } else {
                        currentState = ACCESS_DENIED;
                    }
                } else if (key == '#') {
                    // Clear Password
                    passIndex = 0;
                    memset(enteredPass, 0, sizeof(enteredPass));
                    lcd.showMessage("Pass Cleared", "Enter Again + *");
                } else {
                    // Check buffer overflow
                    if (passIndex < 9) {
                        enteredPass[passIndex] = key;
                        passIndex++;
                        enteredPass[passIndex] = '\0'; // Ensure null termination
                        
                        Serial.print(F("*"));
                        
                        // Manually build mask string
                        char mask[10];
                        for(int i=0; i<passIndex; i++) mask[i] = '*';
                        mask[passIndex] = '\0';
                        
                        lcd.showMessage("Pass:", mask);
                    }
                }
            }
            break;
        }

        case ACCESS_GRANTED: {
            Serial.println(F("Access Granted"));
            lcd.showMessage("Access Granted", "Welcome!");
            logger.logAccess(currentUser, "GRANTED", rtc.getFormattedTime());
            signal(true);
            delay(3000);
            resetSystem();
            break;
        }

        case ACCESS_DENIED: {
            Serial.println(F("Wrong Password"));
            lcd.showMessage("Wrong Password", "Access Denied");
            logger.logAccess(currentUser, "DENIED_PASSWORD", rtc.getFormattedTime());
            signal(false);
            delay(2000);
            resetSystem();
            break;
        }
    }
}