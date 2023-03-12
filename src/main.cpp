#include "MPU9250.h"
#include <Wire.h>

//#include "SoftwareSerial.h"
MPU9250 mpu;

void print_roll_pitch_yaw();
void setup() {
    Serial.begin(9600);
    Wire.begin();
    delay(5000);

    if (!mpu.mysetup(0x68)) {  // change to your own address
        while (1) {
            Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
            delay(5000);
        }
    }

}

void loop() {
    
    if (mpu.my_update()) {
        static uint32_t prev_ms = millis();
        if (millis() > prev_ms + 25) {
            print_roll_pitch_yaw();
            prev_ms = millis();
        }
    }
    delay(500);
}

void print_roll_pitch_yaw() {
    Serial.print("Yaw, Pitch, Roll: ");
    Serial.print(mpu.getYaw(), 2);
    Serial.print(", ");
    Serial.print(mpu.getPitch(), 2);
    Serial.print(", ");
    Serial.println(mpu.getRoll(), 2);
}
