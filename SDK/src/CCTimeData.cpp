#include "CCTimeData.h"

CCTimeInfo CCTimeData::Now() {
    time(reinterpret_cast<time_t *>(&m_time));
    p = localtime(reinterpret_cast<const time_t *>(&m_time));
    return CCTimeInfo{p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour,
                      p->tm_min, p->tm_sec, GetWeek(p->tm_wday)};
}

int CCTimeData::GetWeek(int week) {
    switch (week) {
        case 0: return 7;
        case 1: return 1;
        case 2: return 2;
        case 3: return 3;
        case 4: return 4;
        case 5: return 5;
        case 6: return 6;
        default: return -1;
    }
}

CCString CCTimeData::to_String() const {
    return CCString::format(50,"%d-%d-%d %d:%d:%d", GetTimeInfo.year, GetTimeInfo.month,
                            GetTimeInfo.day,GetTimeInfo.hour,GetTimeInfo.minute,GetTimeInfo.second);
}
