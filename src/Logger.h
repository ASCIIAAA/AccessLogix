#pragma once
#include <Arduino.h>
#include <SD.h>
#include "config.h"
#include "User.h"

class Logger {
private:
    bool sdAvailable;

public:
    Logger();
    void init();
    void logAccess(User* user, String status, String timeStr);
};