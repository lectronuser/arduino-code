#include <Wire.h>

void setup() {
    Wire.begin();
    Serial.begin(115200);
    while (!Serial);

    Serial.println("    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");

    for (uint8_t row = 0; row < 8; row++) {
        Serial.print(row * 16 < 16 ? "0" : ""); 
        Serial.print(row * 16, HEX);
        Serial.print(": ");

        for (uint8_t col = 0; col < 16; col++) {
            uint8_t address = row * 16 + col;

            if (address < 0x03 || address > 0x77) {
                Serial.print("-- ");
                continue;
            }

            Wire.beginTransmission(address);
            if (Wire.endTransmission() == 0) {
                Serial.print(address < 16 ? "0" : ""); 
                Serial.print(address, HEX);
                Serial.print(" ");
            } else {
                Serial.print("-- ");
            }
        }
        Serial.println();
    }
}

void loop() {
}
