#ifndef CC_TimeData_H
#define CC_TimeData_H

#include "CCString.h"
#include <ctime>
#include <sstream>

struct CCTimeInfo
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int week;
};

class CCTimeData
{
private:
    time_t * m_time{};
    struct tm * p{};
public:
    CCTimeData(){GetTimeInfo = Now();};
    CCTimeInfo Now();
    CCString to_String() const;
private:
    static int GetWeek(int week);
    CCTimeInfo GetTimeInfo;
public:

};


#endif
