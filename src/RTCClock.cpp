#include "RTCClock.h"

RTCClock::RTCClock() {
}

bool RTCClock::init() {
    if (!rtc.begin()) return false;
    if (rtc.lostPower()) {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    return true;
}

String RTCClock::getFormattedTime() {
    DateTime now = rtc.now();
    char timeBuffer[20];
    sprintf(timeBuffer, "%02d/%02d %02d:%02d:%02d", now.day(), now.month(), now.hour(), now.minute(), now.second());
    return String(timeBuffer);
}