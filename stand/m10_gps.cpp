#include "m10_gps.hpp"

void GPS::load(char c)
{
    gps.encode(c);
}

void GPS::update()
{
    if (gps.location.isValid())
    {
        if (gps.location.isUpdated())
        {
            position.longitude = gps.location.lng();
            position.latitude = gps.location.lat();
            position.fix_age = static_cast<uint16_t>(gps.location.age());
            position_status.location = true;
        }
    }

    if (gps.altitude.isUpdated())
    {
        position.altitude_m = gps.altitude.meters();
        position_status.altitude = true;
    }

    if (gps.satellites.isUpdated())
    {
        position.satellite_count = static_cast<int16_t>(gps.satellites.value());
    }

    if (gps.hdop.isUpdated())
    {
        position.hdop = gps.hdop.hdop();
    }

    if (gps.course.isUpdated())
    {
        position.course_deg = gps.course.deg();
    }

    if (gps.date.isUpdated())
    {
        timestamp.year = gps.date.year();
        timestamp.month = gps.date.month();
        timestamp.day = gps.date.day();
        timestamp_status.date = true;
    }

    if (gps.time.isUpdated())
    {
        timestamp.hour = gps.time.hour();
        timestamp.minute = gps.time.minute();
        timestamp.second = gps.time.second();
        timestamp_status.time = true;
    }
}

void GPS::display() const
{
    Serial.println(F("===== GPS position ====="));

    if (position_status.location)
    {
        Serial.print(F("Fix Age: "));
        Serial.println(position.fix_age);

        Serial.print(F("Latitude: "));
        Serial.println(position.latitude, 7);

        Serial.print(F("Longitude: "));
        Serial.println(position.longitude, 7);
    }

    if (position_status.altitude)
    {
        Serial.print(F("Altitude (m): "));
        Serial.println(position.altitude_m, 2);
        Serial.print(F("HDOP: "));
        Serial.println(position.hdop);
        Serial.print(F("Course (degrees): "));
        Serial.println(gps.course.deg());
        Serial.print(F("Satellites: "));
        Serial.println(position.satellite_count);
    }

    if (timestamp_status.date && timestamp_status.time)
    {
        Serial.println();
        Serial.print(timestamp.day);
        Serial.print(F("."));
        Serial.print(timestamp.month);
        Serial.print(F("."));
        Serial.print(timestamp.year);

        Serial.print(F(" "));
        Serial.print(timestamp.hour);

        Serial.print(F(":"));
        Serial.print(timestamp.minute);

        Serial.print(F(":"));
        Serial.println(timestamp.second);
    }

    Serial.println(F("====================="));
}
