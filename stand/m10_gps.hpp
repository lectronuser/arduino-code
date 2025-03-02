#ifndef __M10_GPS__
#define __M10_GPS__

#include <TinyGPSPlus.h>

struct GpsDate
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

struct GpsData
{
    uint32_t fix_age; // ms
    double longitude;
    double latitude;
    double altitude; // meters
    double hdop;
    uint16_t course;
    int32_t satellites;
    GpsDate date;
};


/*
Fix Age (ms): 0
Latitude: 40.065063
Longitude: 32.917808
Altitude (m): 947.80
HDOP: 9.65
Course (degrees): 269.28
Satellites: 7
*/

struct GpsReady
{
    GpsReady() : location(false), sats(false), date(false),
                 time(false), altitude(false), htop(false),
                 course(false) {}
    bool location;
    bool altitude;
    bool course;
    bool sats;
    bool htop;
    bool date;
    bool time;

    bool allTrue() const
    {
        bool is_ready = location && altitude && sats && date && time && course && htop;
        return is_ready;
    }
};

class GPS
{
public:
    GPS() = default;
    void load(char c);
    void update();
    bool getIsReady() { return is_ready.allTrue(); }
    void display() const;
    GpsData data;

private:
    TinyGPSPlus gps;
    unsigned long last = 0UL;
    uint32_t fix_age;
    GpsReady is_ready;
};

#endif