#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

class Logger {
public:
    void setup();
    void logInfo(const char* message);
    void logError(const char* message);
};

#endif