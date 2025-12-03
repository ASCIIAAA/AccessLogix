#include "Logger.h"

Logger::Logger() {
    sdAvailable = false;
}

void Logger::init() {
    Serial.print(F("Initializing SD card..."));
    pinMode(PIN_SD_CS, OUTPUT);
    
    if (!SD.begin(PIN_SD_CS)) {
        Serial.println(F("failed!"));
        sdAvailable = false;
        return;
    }
    Serial.println(F("done."));
    sdAvailable = true;

    if (!SD.exists(LOG_FILENAME)) {
        File dataFile = SD.open(LOG_FILENAME,FILE_WRITE);
        if (dataFile) {
            dataFile.println(F("Time,UID,Name,Role,Status"));
            dataFile.close();
        }
    }
}

void Logger::logAccess(User* user, const char* status, const char* timeStr) {
    if (!sdAvailable) return;

    File dataFile = SD.open(LOG_FILENAME, FILE_WRITE);
    if (dataFile) {
        dataFile.print(timeStr);
        dataFile.print(F(","));
        if (user != nullptr) {
            dataFile.print(user->getUid());
            dataFile.print(F(","));
            dataFile.print(user->getName());
            dataFile.print(F(","));
            dataFile.print(user->getRole());
        } 
        else {
            dataFile.print(F("UNKNOWN,-,-"));
        }
        dataFile.print(F(","));
        dataFile.println(status);
        dataFile.close();
        
        Serial.println(F("Log saved to SD."));
    } 
    else {
        Serial.println(F("Error opening log file"));
    }
}