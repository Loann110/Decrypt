#include "MPU9250.h"
#include <SoftwareSerial.h>
#include <Wire.h>

// Define SoftwareSerial pins for communication with E32
#define E32_RX 10  // Arduino receives data from E32
#define E32_TX 11  // Arduino sends data to E32
#define M0 8       // Mode selection pin
#define M1 9       // Mode selection pin

// Définir les broches pour I2C (par défaut, SDA = GP0 et SCL = GP1 pour I2C0)
#define SDA_PIN 1
#define SCL_PIN 2


SoftwareSerial e32Serial(E32_RX, E32_TX);  // RX, TX


MPU9250 mpu;


void setup() {

    Serial.begin(57600);
    e32Serial.begin(9600);
    // Set pin modes for M0 and M1
    pinMode(M0, OUTPUT);
    pinMode(M1, OUTPUT);
    
    // Set module to normal mode (M0 = LOW, M1 = LOW)
    digitalWrite(M0, LOW);
    digitalWrite(M1, LOW);


    //--------------------------------------------------------------------------MPU920-------------------------------------------------------------------
    
    Wire.begin();
    delay(2000);

    if (!mpu.setup(0x68)) {  // if AD0 grounded
        while (1) {
            Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
            delay(5000);
        }
    }

    // calibrate anytime you want to
    Serial.println("Accel Gyro calibration will start in 5sec.");
    Serial.println("Please leave the device still on the flat plane.");
    mpu.verbose(true);
    delay(5000);
    mpu.calibrateAccelGyro();

    Serial.println("Mag calibration will start in 5sec.");
    Serial.println("Please Wave device in a figure eight until done.");
    delay(5000);
    mpu.calibrateMag();

    print_calibration();
    mpu.verbose(false);
                
}


void loop() {
    if (mpu.update()) {
        static uint32_t prev_ms = millis();

        if (millis() > prev_ms + 25) {

            if (Serial.available()) {

              String result = "Yaw: " + String(mpu.getYaw(), 2) + ", Pitch: " + String(mpu.getPitch(), 2) + ", Roll: " + String(mpu.getRoll(), 2);
              e32Serial.print(result);  // Send to E32
              Serial.println("Sent to E32: " + result);
            }
            print_roll_pitch_yaw(); 
            prev_ms = millis();
        }
    }
}


void print_roll_pitch_yaw() {

    Serial.print("Yaw, Pitch, Roll: ");
    Serial.print(mpu.getYaw(), 2);
    Serial.print(", ");
    Serial.print(mpu.getPitch(), 2);
    Serial.print(", ");
    Serial.println(mpu.getRoll(), 2);
    
}


void print_calibration() {

    Serial.println("< calibration parameters >");
    Serial.println("accel bias [g]: ");
    Serial.print(mpu.getAccBiasX() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getAccBiasY() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getAccBiasZ() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.println();
    Serial.println("gyro bias [deg/s]: ");
    Serial.print(mpu.getGyroBiasX() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getGyroBiasY() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getGyroBiasZ() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.println();
    Serial.println("mag bias [mG]: ");
    Serial.print(mpu.getMagBiasX());
    Serial.print(", ");
    Serial.print(mpu.getMagBiasY());
    Serial.print(", ");
    Serial.print(mpu.getMagBiasZ());
    Serial.println();
    Serial.println("mag scale []: ");
    Serial.print(mpu.getMagScaleX());
    Serial.print(", ");
    Serial.print(mpu.getMagScaleY());
    Serial.print(", ");
    Serial.print(mpu.getMagScaleZ());
    Serial.println();
}