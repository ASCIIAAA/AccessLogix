#include "Logger.h"

void Logger::setup() {
    // Nothing to set up for a simple serial logger
}

void Logger::logInfo(const char* message) {
    Serial.print("[INFO] ");
    Serial.println(message);
}

void Logger::logError(const char* message) {
    Serial.print("[ERROR] ");
    Serial.println(message);
}