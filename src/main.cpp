#include <Arduino.h>
#include "config.h"
#include "RFIDReader.h"
#include "LcdDisplay.h"
#include "RTCClock.h"
#include "KeypadReader.h"
#include "UserDB.h"
#include "Logger.h"

char uidBuffer[13];      
char timeBuffer[20];     
char enteredPass[15]; 
int passIndex = 0;  

unsigned long biometricStartTime = 0; 
bool isStudentInside = false;         
unsigned long lastActivityTime = 0;   
const unsigned long SAFETY_TIMEOUT = 15000; 

bool studentTracker[2] = {false, false}; 


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
        delay(100); digitalWrite(PIN_BUZZER, LOW);
        delay(100); digitalWrite(PIN_BUZZER, HIGH);
        delay(100); digitalWrite(PIN_BUZZER, LOW);
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
    biometricStartTime = 0;
    
    currentState = WAITING_FOR_CARD;
    
    lcd.showMessage(F("System Ready"), F("Scan Card..."));
    Serial.println(F("\n--- WAITING FOR CARD ---"));
}

void setup() {
    Serial.begin(115200);
    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);
    
    lcd.init();
    lcd.showMessage(F("Booting..."), F("System Init"));
    
    rtc.init();
    rfid.init();
    userDB.init(); 
    logger.init(); 

    Serial.println(F("System Initialized."));
    resetSystem();
}

void loop() {
    if (isStudentInside) {
        if (millis() - lastActivityTime > SAFETY_TIMEOUT) {
            Serial.println(F("!!! SAFETY ALERT: STUDENT UNRESPONSIVE !!!"));
            lcd.setCursor(0,0); lcd.print(F(" SAFETY ALERT!  "));
            lcd.setCursor(0,1); lcd.print(F("  SCAN TO RST   "));
            digitalWrite(PIN_BUZZER, HIGH);
            delay(100);
            digitalWrite(PIN_BUZZER, LOW);
            delay(100);
        }
    }

    switch (currentState) {
        case WAITING_FOR_CARD: {
            if (rfid.readCard(uidBuffer, sizeof(uidBuffer))) {
                Serial.print(F("Card Scanned: "));
                Serial.println(uidBuffer);
                
                if (isStudentInside) lastActivityTime = millis();

                currentUser = userDB.findUser(uidBuffer);

                if (currentUser != nullptr) {
                    if (currentUser->isActive()) {
                        int trackIdx = -1;
                        if (strcmp(currentUser->getName(), "Aditi") == 0) trackIdx = 0;
                        else if (strcmp(currentUser->getName(), "Anushka") == 0) trackIdx = 1;

                        if (trackIdx != -1) {
                            if (studentTracker[trackIdx] == true) {
                                Serial.println(F("!!! CLONE DETECTED !!!"));
                                lcd.clear();
                                lcd.setCursor(0,0); lcd.print(F("CLONE DETECTED!!"));
                                lcd.setCursor(0,1); lcd.print(F("ALARM TRIGGERED "));
                                
                                for(int i=0; i<5; i++) {
                                    digitalWrite(PIN_BUZZER, HIGH); delay(200);
                                    digitalWrite(PIN_BUZZER, LOW); delay(200);
                                }
                                
                                rtc.getFormattedTime(timeBuffer);
                                logger.logAccess(currentUser, "ALARM_CLONE", timeBuffer);
                                
                                delay(2000);
                                resetSystem();
                                break; 
                            }
                        }
                        
                        if (strcmp(currentUser->getRole(), "S") == 0) {
                             int currentHour = rtc.getHour();
                             if (currentHour < 8 || currentHour >= 18) {
                               // if (false ){ 
                                 Serial.println(F("Shift Error"));
                                 lcd.clear();
                                 lcd.showMessage(F("Access Denied"), F("Lab Closed"));
                                 rtc.getFormattedTime(timeBuffer);
                                 logger.logAccess(currentUser, "DENIED_SHIFT", timeBuffer);
                                 signal(false);
                                 delay(2000);
                                 resetSystem();
                                 break; 
                             }
                        }

                        Serial.print(F("User Found: "));
                        Serial.println(currentUser->getName());
                        
                        lcd.clear();
                        lcd.showMessage("User Found:", currentUser->getName());
                        delay(1000);
                        lcd.showMessage(F("Enter Pass:"), F("Press * when done"));
                        
                        currentState = WAITING_FOR_PASS;
                        passIndex = 0;
                        memset(enteredPass, 0, sizeof(enteredPass));
                        biometricStartTime = 0; 
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
                if (isStudentInside) lastActivityTime = millis();
                
                Serial.print(F("Key: "));
                Serial.println(key); 

                if (key == '*') {
                    unsigned long duration = millis() - biometricStartTime;
                    if (duration < 800 || duration > 10000) {
                        Serial.println(F("Bio-Fail"));
                        lcd.showMessage(F("Access Denied"), F("Suspicious Entry"));
                        rtc.getFormattedTime(timeBuffer);
                        logger.logAccess(currentUser, "DENIED_BIO", timeBuffer);
                        signal(false);
                        delay(2000);
                        resetSystem();
                        break; 
                    }

                    Serial.print(F("Verifying: ")); Serial.println(enteredPass);
                    
                    if (currentUser->checkDuress(enteredPass)) {
                        Serial.println(F("DURESS!"));
                        lcd.showMessage(F("Access Granted"), F("Welcome!")); 
                        rtc.getFormattedTime(timeBuffer);
                        logger.logAccess(currentUser, "ALARM_DURESS", timeBuffer);
                        signal(true); 
                        delay(3000);
                        resetSystem();
                    }
                    else if (currentUser->checkPassword(enteredPass)) {
                        currentState = ACCESS_GRANTED; 
                    } 
                    else {
                        currentState = ACCESS_DENIED;
                    }
                }
                else if (key == '#') {
                    passIndex = 0;
                    memset(enteredPass, 0, sizeof(enteredPass));
                    biometricStartTime = 0; 
                    lcd.showMessage(F("Pass Cleared"), F("Enter Again + *"));
                } 
                else {
                    if (passIndex == 0) biometricStartTime = millis();
                    if (passIndex < 12) {
                        enteredPass[passIndex] = key; 
                        passIndex++;
                        enteredPass[passIndex] = '\0'; 
                        
                        char mask[13];
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
            lcd.showMessage(F("Access Granted"), F("Welcome!"));
            
            if (strcmp(currentUser->getName(), "Aditi") == 0) studentTracker[0] = true;
            else if (strcmp(currentUser->getName(), "Anushka") == 0) studentTracker[1] = true;

            if (strcmp(currentUser->getRole(), "S") == 0) {
                isStudentInside = true;
                lastActivityTime = millis();
            } else if (currentUser->isAdmin()) {
                isStudentInside = false;
                studentTracker[0] = false; 
                studentTracker[1] = false; 
                Serial.println(F("System Reset: All Users Cleared"));
            }
            
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