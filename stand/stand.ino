#include "compass.hpp"
#include "external_io.hpp"
#include "rgb_led.hpp"

Compass compass;
RGB_IS31fl3195 rgb;

// ExternalIO external_io;

bool is_ready_compass = false;
bool is_ready_gps = false;

void sendCommand(int data[]);

void setup()
{
  Serial.begin(115200);
  is_ready_compass = compass.begin();
  rgb.ledOff();
}

void loop()
{

  // if(false)
  // {
  //   Serial.println(compass.getYawAngle());
  // }

}