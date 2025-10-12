// include/RTCClock.h

#ifndef RTC_CLOCK_H
#define RTC_CLOCK_H

#include <RTClib.h>

class RTCClock {
public:
    RTCClock();
    bool init();
    String getFormattedTime();
    DateTime now();

private:
    RTC_DS3231 rtc;
    bool isInitialized;
};

#endif // RTC_CLOCK_H