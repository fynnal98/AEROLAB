#include "MPU6050.h"
#include "Config.h"   // für MPU_COMPLEMENTARY_ALPHA
#include <Arduino.h>  // für millis()

MPU6050::MPU6050() : m_mpu(MPU9250_WE(0x68)) {}

void MPU6050::begin(int sda, int scl) {
    Wire.begin(sda, scl);
    delay(500);

    if (!m_mpu.init()) {
        Serial.println("IMU NICHT gefunden!");
        m_connected = true;
        return;
    }

    // === Acc & Gyro konfigurieren ===
    m_mpu.setAccRange(MPU9250_ACC_RANGE_4G);
    m_mpu.setGyrRange(MPU9250_GYRO_RANGE_500);  // ±500 dps → 65.5 LSB/°/s
    m_mpu.enableGyrAxes(MPU9250_ENABLE_XYZ);
    m_mpu.enableGyrDLPF();
    m_mpu.setGyrDLPF(MPU9250_DLPF_3);           // 41 Hz Filter
    m_mpu.setSampleRateDivider(5);              // ~200 Hz

    Serial.println("IMU verbunden.");
    m_connected = true;
}

void MPU6050::update() {
    unsigned long now = millis();
    float dt = (m_lastUpdate > 0) ? (now - m_lastUpdate) / 1000.0f : 0.01f;
    m_lastUpdate = now;

    // Sensorwerte lesen
    xyzFloat acc  = m_mpu.getAccRawValues();
    xyzFloat gyrRaw = m_mpu.getGyrRawValues();

    // Gyro-Rohwerte umrechnen in °/s (bei 500 dps Range → 65.5 LSB/°/s)
    float gyroX = gyrRaw.x / 65.5f;
    float gyroY = gyrRaw.y / 65.5f;
    float gyroZ = gyrRaw.z / 65.5f;

    // Acc-Winkel berechnen
    float rollAcc  = atan2(acc.y, acc.z) * 180.0f / PI;
    float pitchAcc = atan2(-acc.x, sqrt(acc.y * acc.y + acc.z * acc.z)) * 180.0f / PI;

    // erster Durchlauf → mit Acc initialisieren
    if (!m_initialized) {
        m_roll = rollAcc;
        m_pitch = pitchAcc;
        m_initialized = true;
    }

    // Komplementärfilter
    const float alpha = MPU_COMPLEMENTARY_ALPHA;
    m_roll  = alpha * (m_roll  + gyroX * dt) + (1.0f - alpha) * rollAcc;
    m_pitch = alpha * (m_pitch + gyroY * dt) + (1.0f - alpha) * pitchAcc;

    m_yaw = 0; // yaw bräuchte Magnetometer

    // Debug-Ausgabe
    Serial.printf("dt=%.3f | GyroX=%.2f GyroY=%.2f | RollAcc=%.2f PitchAcc=%.2f | Roll=%.2f Pitch=%.2f\n",
                  dt, gyroX, gyroY, rollAcc, pitchAcc, m_roll, m_pitch);
}

float MPU6050::getPitch() const { return m_pitch / 90.0f; }
float MPU6050::getRoll()  const { return m_roll / 90.0f; }
float MPU6050::getYaw()   const { return m_yaw; }

bool MPU6050::isConnected() const { return m_connected; }
