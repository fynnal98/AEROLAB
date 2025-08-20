#ifndef MY_MPU9250_H
#define MY_MPU9250_H

#include <Arduino.h>
#include <Wire.h>
#include "Config.h"
#include <MPU9250.h>   // Bolderflight Library

class MyMPU9250 {
public:
    MyMPU9250();
    void begin(int sda = IMU_SDA_PIN, int scl = IMU_SCL_PIN);
    void update();

    float getPitch() const;
    float getRoll() const;
    float getYaw() const;

    bool isConnected() const;

private:
    MPU9250 m_mpu;   // Objekt aus der Bolderflight-Lib
    bool m_connected = false;

    float m_pitch = 0;
    float m_roll  = 0;
    float m_yaw   = 0;
};

#endif
