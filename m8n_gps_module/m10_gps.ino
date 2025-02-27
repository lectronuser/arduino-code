#include <Wire.h>

#include "ist8310.hpp"

#define SER1_BAUDRATE 115200
#define SER2_BAUDRATE 38400

#define NCP5623C_ADDR 0x38  // NCP5623C I2C adresi

char my_string[128];

IST8310 sensor;
bool is_ready_compass = false;

void setup()
{
    Serial.begin(SER1_BAUDRATE);
    Serial2.begin(SER2_BAUDRATE);

    Wire.begin();
    
    if(sensor.begin())
    {
      is_ready_compass = true;
    }

    sprintf(my_string, "Compass Initialization %s", (is_ready_compass ? "Successful" : "Failed"));
    Serial.println(my_string);
    delay(100);
    //pinMode(7, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(4, OUTPUT);
    //digitalWrite(7, LOW);
    digitalWrite(6, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(4, LOW);

    setBrightness(31); // Maksimum parlaklık (0-31)
    setColor(255, 0, 0); // Kırmızı (R=255, G=0, B=0)
    delay(1000);
    setColor(0, 255, 0); // Yeşil (R=0, G=255, B=0)
    delay(1000);
    setColor(0, 0, 255); // Mavi (R=0, G=0, B=255)
    delay(1000);
}

void loop()
{
  int temp = 0;
  if(Serial2.available())
  {
    Serial.write(Serial2.read());
  }


  delay(100);
}

void setBrightness(uint8_t brightness) {
    if (brightness > 31) brightness = 31; // Maksimum değer sınırı
    Wire.beginTransmission(NCP5623C_ADDR);
    Wire.write(0x00 | (brightness & 0x1F)); // 0x00 + parlaklık seviyesi
    Wire.endTransmission();
}

// 📌 Renk Ayarlama Fonksiyonu (R, G, B değerleri 0-255 arasında)
void setColor(uint8_t red, uint8_t green, uint8_t blue) {
    Wire.beginTransmission(NCP5623C_ADDR);
    Wire.write(0x01); // Kırmızı kanal
    Wire.write(red);
    Wire.write(0x02); // Yeşil kanal
    Wire.write(green);
    Wire.write(0x03); // Mavi kanal
    Wire.write(blue);
    Wire.endTransmission();

    Serial.print("Yeni renk -> R: "); Serial.print(red);
    Serial.print(" G: "); Serial.print(green);
    Serial.print(" B: "); Serial.println(blue);
}
