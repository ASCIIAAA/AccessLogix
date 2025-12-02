#ifndef RTC_CLOCK_H
#define RTC_CLOCK_H

#include <RTClib.h>

class RTCClock {
public:
    RTCClock();
    bool init();
    // CHANGED: Write time to a provided variable, don't return a new String
    void getFormattedTime(char* buffer);

private:
    RTC_DS3231 rtc;
};

#endif