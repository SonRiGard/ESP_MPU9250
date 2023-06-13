#include "MPU9250.h"
#include "SoftwareSerial.h"
#include <Wire.h>
#include <TinyGPSPlus.h>

//#include "mpu9250_gy271.h"
#include "math.h"
#define pi 3.141592653589793238462
float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};
float a12, a22, a31, a32, a33;
extern float roll,yaw,pitch;

extern float an ,ae, ad ; 
float lin_acc[3]={0,0,0};
float pos_x=0.0,pos_y=0.0,pos_z=0.0;
uint32_t newTime{0}, oldTime{0};
extern float deltaT;

//#include "SoftwareSerial.h"
MPU9250 mpu;
static const int RX2Pin = 17, TX2Pin = 16;
static const uint32_t GPSBaud = 9600;
// The TinyGPS++ object
TinyGPSPlus gps;
// The serial connection to the GPS device
SoftwareSerial ss(RX2Pin, TX2Pin);

TinyGPSCustom pdop(gps, "GNGLL", 1); // $GPGSA sentence, 15th element
TinyGPSCustom hdop(gps, "GNGLL", 3); // $GPGSA sentence, 16th element


void print_roll_pitch_yaw();
void print_Gyro(void);
void print_Acc(void);
void print_Mag(void);
void read_GPS (void);
void Init_GPS(void);
void Position (void);
void print_position(void);
void update_quaternion (void);
void print_linear_acc(void) ;

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

    //Init_GPS();


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
        update_quaternion();
        Position();
        
        //mpu.read_sensor_9dof();
        //read_GPS ()

        // Serial.print(yaw2);
        // Serial.print(",");
        // Serial.print(yaw3);
        // Serial.print(",");
        //print_position();
       // Serial.print(",");
        //print_roll_pitch_yaw();Serial.println("");
      print_linear_acc();Serial.println("");

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

void update_quaternion (void){
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
        //print_Gyro();Serial.print(",");
        //getlinear acc

        lin_acc[0] = an + a31;
        lin_acc[1] = ae + a32;
        lin_acc[2] = ad - a33;

}

void read_GPS (void){
    Serial.print(F(" LAT=")); Serial.print(atof(pdop.value())/100,7);
  Serial.print(F("\tLON=")); Serial.println(atof(hdop.value())/100,7);
  delay(100);

  while (ss.available() > 0)
    gps.encode(ss.read());



  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

void Init_GPS(void){
    ss.begin(GPSBaud);
    Serial.println(F("UsingCustomFields.ino"));
    Serial.println(F("Demonstrating how to extract any NMEA field using TinyGPSCustom"));
    Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
    Serial.println(F("by Mikal Hart"));
    Serial.println();
}

void Position (void) {
    pos_x = 0.5 * lin_acc[0] * deltaT * deltaT*1000000;
    pos_y = 0.5 * lin_acc[1] * deltaT * deltaT*1000000;
    pos_z = 0.5 * lin_acc[2] * deltaT * deltaT*1000000;
}

void print_position(void) {
    Serial.print(pos_x);
    Serial.print(",");
    Serial.print(pos_y);
    Serial.print(",");
    Serial.print(pos_z);
}

void print_linear_acc(void) {
    Serial.print(lin_acc[0]);
    Serial.print(",");
    Serial.print(lin_acc[1]);
    Serial.print(",");
    Serial.print(lin_acc[2]);
    Serial.print(",");

    Serial.print(an);
    Serial.print(",");
    Serial.print(ae);
    Serial.print(",");
    Serial.print(ad);
}