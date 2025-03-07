#include <math.h>
#include <MAVLink.h>
#include "compass.hpp"
#include "external_io.hpp"
#include "rgb_led.hpp"
#include "m10_gps.hpp"

#define SYS_ID (1)
#define COMPONENT_ID (200)
#define TEST

Compass compass;
RGB_IS31fl3195 rgb;
GPS m10;
// ExternalIO external_io;

bool isReady = false;
uint32_t last_msg_update_time = 0;
uint32_t last_heartbeat_time = 0;

void setup()
{
    Serial.begin(115200);
    Serial2.begin(115200);
    compass.begin();
    rgb.ledOff();
    delay(100);
}

void loop()
{
#ifndef TEST
    while (Serial2.available() > 0)
    {
        m10.load(Serial2.read());
    }

    m10.update();
    isReady = compass.getIsReady() && m10.getIsReady();
#else
    isReady = true;
#endif

    blink(isReady ? led_control_s::COLOR_GREEN : led_control_s::COLOR_RED);

    if (isReady && millis() - last_msg_update_time > 250)
    {
        last_msg_update_time = millis();
        if (millis() - last_heartbeat_time > 3000)
        {
            last_heartbeat_time = millis();
            sendHeartbeat();
            return;
        }
        sendMAVLinkMessage(compass.getYawAngle());
    }
}

void sendHeartbeat()
{
    mavlink_message_t msg;
    mavlink_heartbeat_t hb{};

    uint8_t type = MAV_TYPE_GENERIC;
    uint8_t autopilot = MAV_AUTOPILOT_GENERIC;
    uint8_t base_mode = 0;
    uint32_t custom_mode = 0;
    uint8_t system_status = MAV_STATE_STANDBY;
    uint8_t mavlink_version = MAVLINK_VERSION;

    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    mavlink_msg_heartbeat_pack(SYS_ID, COMPONENT_ID, &msg, type, autopilot, base_mode, custom_mode, system_status);

    uint16_t len = mavlink_msg_to_send_buffer(buffer, &msg);
    Serial.write(buffer, len);
}

void sendMAVLinkMessage(float yaw)
{
    mavlink_message_t msg;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];

    mavlink_global_position_int_t global_position;

#ifdef TEST
    m10.position.fix_age_ms = 1;
    m10.position.latitude = 40.065063;
    m10.position.longitude = 32.917808;
    m10.position.altitude_m = 947.80;
    m10.position.course_deg = 269.28;
    m10.position.hdop = 1.67; 
    m10.position.satellite_count = 4;
    yaw = 181.52;
#endif

    global_position.time_boot_ms = static_cast<uint32_t>(m10.position.fix_age_ms);

    global_position.lat = static_cast<int32_t>(m10.position.latitude * 1e7);
    global_position.lon = static_cast<int32_t>(m10.position.longitude * 1e7);
    global_position.alt = static_cast<int32_t>(m10.position.altitude_m * 1000);

    global_position.relative_alt = global_position.alt;
    global_position.vx = static_cast<int16_t>(m10.position.course_deg * 10);
    global_position.vy = static_cast<int16_t>(min(m10.position.hdop * 100, 255.0));
    global_position.vz = static_cast<int16_t>(m10.position.satellite_count);

    global_position.hdg = static_cast<uint16_t>(yaw * 10);
    mavlink_msg_global_position_int_encode(1, 200, &msg, &global_position);
    uint16_t len = mavlink_msg_to_send_buffer(buffer, &msg);
    Serial.write(buffer, len);
}

void blink(uint8_t color)
{
    static bool led_state = false;
    static uint32_t last_blink_time = 0;

    if (millis() - last_blink_time >= 1000)
    {
        last_blink_time = millis();
        led_state = !led_state;

        if (led_state)
            rgb.ledOff();
        else
        {
            if (color == led_control_s::COLOR_RED)
                rgb.red();
            else if (color == led_control_s::COLOR_GREEN)
                rgb.green();
        }
    }
}
