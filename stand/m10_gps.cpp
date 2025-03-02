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
            data.longitude = gps.location.lng();
            data.latitude = gps.location.lat();
            data.fix_age = gps.location.age();
            is_ready.location = true;
        }
    }

    if (gps.altitude.isUpdated())
    {
        data.altitude = gps.altitude.meters();
        is_ready.altitude = true;
    }

    if (gps.satellites.isUpdated())
    {
        data.satellites = gps.satellites.value();
        is_ready.sats = true;
    }

    if (gps.hdop.isUpdated())
    {
        data.hdop = gps.hdop.hdop();
        is_ready.htop = true;
    }

    if (gps.course.isUpdated())
    {
        data.course = gps.course.deg();
        is_ready.course = true;
    }

    if (gps.date.isUpdated())
    {
        data.date.year = gps.date.year();
        data.date.month = gps.date.month();
        data.date.day = gps.date.day();
        is_ready.date = true;
    }

    if (gps.time.isUpdated())
    {
        data.date.hour = gps.time.hour();
        data.date.minute = gps.time.minute();
        data.date.second = gps.time.second();
        is_ready.time = true;
    }
}

void GPS::display() const
{
    Serial.println(F("===== GPS DATA ====="));

    if (is_ready.location)
    {
        Serial.print(F("Fix Age (ms): "));
        Serial.println(data.fix_age);

        Serial.print(F("Latitude: "));
        Serial.println(data.latitude, 6);

        Serial.print(F("Longitude: "));
        Serial.println(data.longitude, 6);
    }

    if (is_ready.altitude)
    {
        Serial.print(F("Altitude (m): "));
        Serial.println(data.altitude, 2);
    }

    if (is_ready.htop)
    {
        Serial.print(F("HDOP: "));
        Serial.println(data.hdop);
    }

    if (is_ready.course)
    {
        Serial.print(F("Course (degrees): "));
        Serial.println(gps.course.deg());
    }

    if (is_ready.sats)
    {
        Serial.print(F("Satellites: "));
        Serial.println(data.satellites);
    }

    if (is_ready.date && is_ready.time)
    {
        Serial.println();
        Serial.print(data.date.day);
        Serial.print(F("."));
        Serial.print(data.date.month);
        Serial.print(F("."));
        Serial.print(data.date.year);

        Serial.print(F(" "));
        Serial.print(data.date.hour);

        Serial.print(F(":"));
        Serial.print(data.date.minute);

        Serial.print(F(":"));
        Serial.println(data.date.second);
    }

    Serial.println(F("====================="));
}
