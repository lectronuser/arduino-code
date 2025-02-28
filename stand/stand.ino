#include "m10_gps.hpp"

#define START_BYTE (uint8_t)0xFF
#define STOP_BYTE  (uint8_t)0xAA

Compass_IST8310 compass;
// RGB_IS31fl3195 rgb_led;
// RGB_NCP5623 rgb_led;
// ExternalIO external_io;

bool is_ready_compass = false;
bool is_ready_gps = false;

void sendCommand(int data[]);

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);
  is_ready_compass = compass.begin();

}

void loop()
{
    if(Serial2.available() > 0)
    {
      Serial.write(Serial2.read());
    }
}

/*
yaw  ()
altitude (double)
satellites (double)
course  (double)
lat (double)
lng (double)
*/
void sendCommand(int data[]) 
{
  static uint8_t buffer[10] ;
  buffer[0]   = START_BYTE;
  buffer[1]   = 0;
  buffer[1]  |= ((msg[0]) ? 0x01 : 0x00);
  buffer[1]  |= ((msg[1]) ? 0x02 : 0x00);
  buffer[1]  |= ((msg[2]) ? 0x04 : 0x00);
  buffer[1]  |= ((msg[3]) ? 0x08 : 0x00);
  buffer[1]  |= (uint8_t)((msg[4] & 0x03FF) << 4);                       
  buffer[2]   = (uint8_t)((msg[4] & 0x03FF) >> 4 | (msg[5] & 0x03FF) << 6);
  buffer[3]   = (uint8_t)((msg[5] & 0x03FF) >> 2);
  buffer[4]   = (uint8_t)(msg[6] & 0x03FF);
  buffer[5]   = (uint8_t)((msg[6] & 0x03FF) >> 8 | (msg[7] & 0x03FF) << 2);
  buffer[6]   = (uint8_t)((msg[7] & 0x03FF) >> 6 | (msg[8] & 0x03FF) << 4);
  buffer[7]   = (uint8_t)((msg[8] & 0x03FF) >> 4 | (msg[9] & 0x03FF) << 6);
  buffer[8]   = (uint8_t)((msg[9] & 0x03FF) >> 2);
  buffer[9]   = STOP_BYTE;
  
  for(uint8_t data_: buffer){
    Serial.write(data_);
  }
}