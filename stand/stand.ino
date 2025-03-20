#include <math.h>
#include <MAVLink.h>
#include "compass.hpp"
#include "external_io.hpp"
#include "rgb_led.hpp"
#include "m10_gps.hpp"

#define DEG_TO_RAD(deg) ((deg) * (M_PI / 180.0))

#define SYS_ID (2)
#define COMPONENT_ID (200)
#define HEARTBEAT_MS (50000)
#define GPS_MS (1000)
#define ATTITUDE_MS (200)

#define TEST_COMPASS_ENABLE 0
#define TEST_GPS_ENABLE 1

Compass compass;
RGB_IS31fl3195 rgb;
GPS m10;
// ExternalIO external_io;

bool is_compass_ready = false;
bool is_gps_ready = false;
uint32_t last_gps_time = 0;
uint32_t last_attitude_time = 0;
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
#if TEST_COMPASS_ENABLE
  is_compass_ready = true;
#else
  is_compass_ready = compass.getIsReady();
#endif

#if TEST_GPS_ENABLE
  is_gps_ready = true;
#else
  while (Serial2.available() > 0)
  {
    m10.load(Serial2.read());
  }

  m10.update();
  is_gps_ready = m10.getIsReady();
  
#endif

  blink(is_gps_ready ? led_control_s::COLOR_GREEN : led_control_s::COLOR_RED);

  if (is_gps_ready && millis() - last_gps_time > GPS_MS)
  {
    last_gps_time = millis();
    sendMAVLinkGPSMessage();
  }
  if (is_compass_ready && millis() - last_attitude_time > ATTITUDE_MS)
  {
    last_attitude_time = millis();
#if TEST_COMPASS_ENABLE
    sendMAVLinkAttitudeMessage(0, 0, 0);
#else
    sendMAVLinkAttitudeMessage(0, 0, compass.getYawAngle());
#endif
  }
  if (millis() - last_heartbeat_time > HEARTBEAT_MS)
  {
    last_heartbeat_time = millis();
    sendHeartbeat();
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

void sendMAVLinkGPSMessage()
{
  mavlink_message_t msg;
  uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
  mavlink_hil_gps_t hil_gps;

#ifdef TEST_GPS_ENABLE
  m10.position.fix_age = 1;
  m10.position.latitude = 40.0738894;
  m10.position.longitude = 33.0015557;
  m10.position.altitude_m = 947.80;
  m10.position.course_deg = 269.28;
  m10.position.hdop = 1.67;
  m10.position.satellite_count = 12;
#endif

  hil_gps.lat = static_cast<int32_t>(m10.position.latitude * 1e7);
  hil_gps.lon = static_cast<int32_t>(m10.position.longitude * 1e7);
  hil_gps.alt = static_cast<int32_t>(m10.position.altitude_m * 1000);
  hil_gps.fix_type = static_cast<uint8_t>(m10.position.fix_age);

  hil_gps.eph = static_cast<int16_t>(min(m10.position.hdop * 100, 255.0));
  hil_gps.satellites_visible = m10.position.satellite_count;
  hil_gps.cog = static_cast<int16_t>(m10.position.course_deg * 10);

  mavlink_msg_hil_gps_encode(SYS_ID, COMPONENT_ID, &msg, &hil_gps);
  uint16_t len = mavlink_msg_to_send_buffer(buffer, &msg);
  Serial.write(buffer, len);
}

void sendMAVLinkAttitudeMessage(float roll, float picth, float yaw)
{
  mavlink_message_t msg;
  uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
  mavlink_attitude_t attitude;

  attitude.roll = DEG_TO_RAD(roll);
  attitude.pitch = DEG_TO_RAD(picth);
  attitude.yaw = DEG_TO_RAD(yaw);

  mavlink_msg_attitude_encode(SYS_ID, COMPONENT_ID, &msg, &attitude);
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
