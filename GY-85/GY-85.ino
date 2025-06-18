#include <Wire.h>
#include "ADXL345.hpp"
#include "ITG3205.hpp"

#define BINARY

// Sensor objects
ADXL345 accelerometer;
ITG3205 gyro;

// Timing variables
unsigned long lastSampleTime = 0;

// Data structures
struct SensorData {
  unsigned long int timestamp;
  float accelX, accelY, accelZ;
  float gyroX, gyroY, gyroZ;
  float temp;
};

volatile bool dataReady = false;
SensorData currentData;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  
  accelerometer.init(ADXL345Range::ADXL345_RANGE_4G, ADXL345DataRate::ADXL345_DATARATE_200HZ);
  gyro.init(ITG3205DLPFConfig::DLPF_42HZ, ITG3205Frequency::FREQ_200HZ);

  // Serial.println("Calibrating Sensors...");
  accelerometer.calibrate();
  gyro.calibrate();
  
  accelerometer.enableReadingInterrupt();
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), accelInterrupt, RISING);
  
  // Serial.println("Starting sensor data collection at 200Hz...");
}

void loop() {
  ADXL345Data accelData;
  ITG3205Data gyroData;

  unsigned long now = micros();

  if (dataReady)
  {
    dataReady = false;

    accelerometer.readAccel(accelData);

    currentData.accelX = accelData.x;
    currentData.accelY = accelData.y;
    currentData.accelZ = accelData.z;

    gyro.readGyro(gyroData);

    currentData.gyroX = gyroData.x;
    currentData.gyroY = gyroData.y;
    currentData.gyroZ = gyroData.z;
    currentData.temp = gyro.getTemperature();

    lastSampleTime = now;
    sendData();
  }

  if (now - lastSampleTime >= 10000)
  {
    // Serial.print("[POL]: ");
    accelerometer.readAccel(accelData);
    gyro.readGyro(gyroData);
    lastSampleTime = now;
    sendData();
  }

}

void accelInterrupt() {
  dataReady = true;
}

void sendData()
{
#ifndef BINARY
  Serial.print(micros());
  Serial.print(",");
  Serial.print(currentData.accelX, 4); Serial.print(",");
  Serial.print(currentData.accelY, 4); Serial.print(",");
  Serial.print(currentData.accelZ, 4); Serial.print(",");
  Serial.print(currentData.gyroX, 4);  Serial.print(",");
  Serial.print(currentData.gyroY, 4);  Serial.print(",");
  Serial.println(currentData.gyroZ, 4);
  //Serial.println(currentData.temp, 2);
#else
    currentData.timestamp = micros();
    Serial.write((const uint8_t *) &currentData, sizeof(SensorData));
#endif
}