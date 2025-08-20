#include "MPU6050.h"

MPU6050::MPU6050() : m_mpu(MPU9250_WE(0x68)) {}

void MPU6050::begin(int sda, int scl) {
    Wire.begin(sda, scl);
    delay(500);

    if (!m_mpu.init()) {
        Serial.println("IMU NICHT gefunden, versuche trotzdem Rohdaten.");
        m_connected = true;
        return;
    }

    Serial.println("IMU verbunden.");
    m_connected = true;
}

void MPU6050::update() {
    xyzFloat acc = m_mpu.getAccRawValues();

    // Roll und Pitch aus Acc-Daten berechnen
    m_roll  = atan2(acc.y, acc.z) * 180.0f / PI;
    m_pitch = atan2(-acc.x, sqrt(acc.y * acc.y + acc.z * acc.z)) * 180.0f / PI;

    // yaw erstmal nicht -> bräuchte Magnetometer
    m_yaw = 0;
}

float MPU6050::getPitch() const { return m_pitch / 90.0f; }
float MPU6050::getRoll()  const { return m_roll / 90.0f; }
float MPU6050::getYaw()   const { return m_yaw; }

bool MPU6050::isConnected() const { return m_connected; }
