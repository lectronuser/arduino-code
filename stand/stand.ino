#include "compass.hpp"
#include "external_io.hpp"
#include "rgb_led.hpp"
#include "m10_gps.hpp"

#define START_BYTE  0xAA  
#define STOP_BYTE   0xFF  

Compass compass;
RGB_IS31fl3195 rgb;
GPS m10;
// ExternalIO external_io;

uint32_t prev_time = 0;

void sendCommand();
void blink(uint8_t color);

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

  while(Serial2.available() > 0)
  {
    m10.load(Serial2.read());
  }

  m10.update();
  
  if(compass.getIsReady() && m10.getIsReady())
  {
    blink(led_control_s::COLOR_GREEN);
    sendCommand(compass.getYawAngle());
  }
  else
  {
    blink(led_control_s::COLOR_RED);
  }
}

void blink(uint8_t color)
{
  static int select = 0;
  if(millis() - prev_time > 1000)
  {
    prev_time = millis();
    select++;
    select %= 2;
    if(select == 1)
    {
      rgb.ledOff();
    }
    else if(color == led_control_s::COLOR_RED)
    {
      rgb.red();
    }
    else if(color == led_control_s::COLOR_GREEN)
    {
      rgb.green();
    }
  }
}

void sendCommand(int yaw)
{
  static uint8_t buffer[26];

  buffer[0] = START_BYTE;

  int32_t lat = static_cast<uint32_t>(m10.data.latitude * 1e7); 
  int32_t lon = static_cast<uint32_t>(m10.data.longitude * 1e7);
  int16_t alt = static_cast<uint16_t>(m10.data.altitude);
  uint8_t fix_age = static_cast<uint8_t>(m10.data.fix_age);
  uint8_t htop = static_cast<uint8_t>(m10.data.hdop * 10);
  uint8_t satellites = static_cast<uint8_t>(m10.data.satellites);

  memcpy(&buffer[1], &lat, 4);
  memcpy(&buffer[5], &lon, 4);
  memcpy(&buffer[9], &alt, 2);
  memcpy(&buffer[11], &fix_age, 1);
  memcpy(&buffer[12], &htop, 1);
  memcpy(&buffer[13], &m10.data.course, 2);
  memcpy(&buffer[15], &satellites, 1);
  memcpy(&buffer[16], &m10.data.date.year, 2);

  buffer[18] =  m10.data.date.month;
  buffer[19] =  m10.data.date.day;
  buffer[20] =  m10.data.date.hour;
  buffer[21] =  m10.data.date.minute;
  buffer[22] =  m10.data.date.second;
  memcpy(&buffer[23], &yaw, 2);
  buffer[25] = STOP_BYTE;

  Serial.write(buffer, sizeof(buffer));

  // for(uint8_t data_: buffer){
  //   Serial.write(data_);
  // }
}