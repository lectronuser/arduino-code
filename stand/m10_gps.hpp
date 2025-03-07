#ifndef M10_GPS_HPP
#define M10_GPS_HPP

#include <TinyGPSPlus.h>

struct GpsTimestamp
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

struct GpsPosition
{
    uint16_t fix_age_ms;  // Fix bilgisinin yaşı (ms cinsinden)
    double longitude;     // Boylam (derece)
    double latitude;      // Enlem (derece)
    double altitude_m;    // Yükseklik (metre)
    double hdop;          // Yatay doğruluk değeri (HDOP)
    float course_deg;  // Yön (derece cinsinden)
    int16_t satellite_count; // Bağlı uydu sayısı
};

struct GpsPositionStatus
{
    GpsPositionStatus() : location(false), altitude(false) {}

    bool location;
    bool altitude;

    bool isReady() const
    {
        return location && altitude;
    }
};

struct GpsTimestampStatus
{
    GpsTimestampStatus() : date(false), time(false) {}

    bool date;
    bool time;

    bool isReady() const
    {
        return date && time;
    }
};

class GPS
{
public:
    GPS() = default;
    void load(char c);
    void update();
    bool getIsReady() const { return position_status.isReady(); }
    void display() const;
    GpsPosition position;
    GpsTimestamp timestamp;

private:
    TinyGPSPlus gps;
    unsigned long last = 0UL;
    uint32_t fix_age;
    GpsPositionStatus position_status;
    GpsTimestampStatus timestamp_status;
};

#endif