#include "MPU9250.h"
#include "SoftwareSerial.h"
#include <Wire.h>
//#include "mpu9250_gy271.h"
#include "math.h"
#define pi 3.141592653589793238462
float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};
float a12, a22, a31, a32, a33;
extern float roll,yaw,pitch;


uint32_t newTime{0}, oldTime{0};
extern float deltaT;

//#include "SoftwareSerial.h"
MPU9250 mpu;


void print_roll_pitch_yaw();
void print_Gyro(void);
void print_Acc(void);
void print_Mag(void);

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
    oldTime=micros();
    // print_calibration();
}

void loop()
{
        // if (mpu.my_update()) {
        //     static uint32_t prev_ms = millis();
        //     if (millis() > prev_ms + 25) {
        //         print_roll_pitch_yaw();
        //        prev_ms = millis();
        //     }
        // }

        newTime = micros();
        deltaT = newTime - oldTime;
        // Serial.print(newTime);       Serial.print(",");
        // Serial.print(oldTime);       Serial.print(",");
        oldTime = newTime;
        deltaT = deltaT * 0.001 * 0.001;

        mpu.Process_IMU();

        // Serial.print(Gyro_X);        Serial.print(",");
        // Serial.print(Gyro_Y);        Serial.print(",");
        // Serial.print(Gyro_Z);        Serial.print(",");
        // Serial.print(Accel_X);        Serial.print(",");
        // Serial.print(Accel_Y);        Serial.print(",");
        // Serial.print(Accel_Z);        Serial.print(",");
        // Serial.print(Mag_X);        Serial.print(",");
        // Serial.print(Mag_Y);        Serial.print(",");
        // Serial.println(-Mag_Z);       //Serial.print(",");

        q[0] = q0;
        q[1] = q1;
        q[2] = q2;
        q[3] = q3;
        a12 =   2.0f * (q[1] * q[2] + q[0] * q[3]);
        a22 =   q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3];
        a31 =   2.0f * (q[0] * q[1] + q[2] * q[3]);
        a32 =   2.0f * (q[1] * q[3] - q[0] * q[2]);
        a33 =   q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3];
        float sinp = a32;
        if (fabsf(sinp) >= 1)
        	pitch = copysign(M_PI/2,sinp);
        else
        	pitch =  asin(sinp);
        //pitch = -asinf(a32);
        roll  = atan2f(a31, a33);
        yaw   = atan2f(a12, a22);
        pitch *= 180.0f / pi;
        yaw = atan2f(sinf(yaw),cosf(yaw));
        yaw   *= 180.0f / pi;
       	roll  *= 180.0f / pi;
        
        yaw2 += gd*deltaT*RAD_TO_DEG;
        yaw3 = atan2f(me,mn)*RAD_TO_DEG;

        Serial.print(yaw2);
        Serial.print(",");
        Serial.print(yaw3);
        Serial.print(",");
        print_roll_pitch_yaw();Serial.println("");
        // print_Gyro();Serial.print(",");
        // print_Acc();Serial.print(",");
        // print_Mag();Serial.println("");

}      
void print_Gyro(void){
    //Serial.print("Yaw, Pitch, Roll: ");
    Serial.print(gn*RAD_TO_DEG);
    Serial.print(",");
    Serial.print(ge*RAD_TO_DEG);
    Serial.print(",");
    Serial.print(gd*RAD_TO_DEG);
}
void print_Acc(void){
    //Serial.print("Yaw, Pitch, Roll: ");
    Serial.print(an);
    Serial.print(",");
    Serial.print(ae);
    Serial.print(",");
    Serial.print(ad);
}
void print_Mag(void){
    //Serial.print("Yaw, Pitch, Roll: ");
    Serial.print(mn);
    Serial.print(",");
    Serial.print(me);
    Serial.print(",");
    Serial.print(md);
}
void print_roll_pitch_yaw() {
    Serial.print(roll);
    Serial.print(",");
    Serial.print(pitch);
    Serial.print(",");
    Serial.print(yaw);
}