// src/RTCClock.cpp

#include "RTCClock.h"
#include <Arduino.h>

RTCClock::RTCClock() : isInitialized(false) {
    // Constructor
}

bool RTCClock::init() {
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        isInitialized = false;
        return false;
    }

    if (rtc.lostPower()) {
        Serial.println("RTC lost power, let's set the time!");
        // This line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // After the first upload, you can comment out the line above
        // to prevent resetting the time every time you upload.
    }

    isInitialized = true;
    return true;
}

String RTCClock::getFormattedTime() {
    if (!isInitialized) {
        return "TIME ERROR";
    }
    DateTime now = rtc.now();
    char timeBuffer[9]; // "HH:MM:SS\0"
    sprintf(timeBuffer, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    return String(timeBuffer);
}

DateTime RTCClock::now() {
    return rtc.now();
}