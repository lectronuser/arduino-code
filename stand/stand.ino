#include "compass.hpp"
#include "external_io.hpp"
#include "rgb_led.hpp"
#include "m10_gps.hpp"

Compass compass;
RGB_IS31fl3195 rgb;
GPS m10;
// ExternalIO external_io;

bool is_ready_compass = false;

uint32_t prev_time = 0;

void sendCommand(int data[]);
void blink(uint8_t color);

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);
  is_ready_compass = compass.begin();
  rgb.ledOff();
  delay(100);

  prev_time = millis();
}

void loop()
{

  while(Serial2.available() > 0)
  {
    m10.load(Serial2.read());
  }

  m10.update();
  
  if(is_ready_compass && m10.getIsReady())
  {
    blink(led_control_s::COLOR_GREEN);
    Serial.println(compass.getYawAngle());
  }
  else
  {
    blink(led_control_s::COLOR_RED);
  }

  delay(250);
}

void blink(uint8_t color)
{
  if(millis() - prev_time > 500)
  {
    prev_time = millis();
    if(color == led_control_s::COLOR_RED)
    {
      rgb.red();
    }
    else
    {
      rgb.green();
    }
  }
  else
  {
    rgb.ledOff();
  }
}
