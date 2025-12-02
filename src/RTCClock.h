#ifndef RTC_CLOCK_H
#define RTC_CLOCK_H

#include <RTClib.h>

class RTCClock {
public:
    RTCClock();
    bool init();
    void getFormattedTime(char* buffer);
    uint8_t getHour(); 

private:
    RTC_DS3231 rtc;
};

#endif