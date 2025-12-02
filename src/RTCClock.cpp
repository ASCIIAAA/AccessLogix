#include "RTCClock.h"

RTCClock::RTCClock() {}

bool RTCClock::init() {
    if (!rtc.begin()) return false;
    if (rtc.lostPower()) {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    return true;
}

void RTCClock::getFormattedTime(char* buffer) {
    DateTime now = rtc.now();
    // Efficiently format directly into the buffer
    sprintf(buffer, "%02d/%02d %02d:%02d:%02d", 
            now.day(), now.month(), now.hour(), now.minute(), now.second());
}