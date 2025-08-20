#include "MyMPU9250.h"

MyMPU9250::MyMPU9250() : m_mpu(Wire, 0x68) {}  // Standard I2C-Adresse

void MyMPU9250::begin(int sda, int scl) {
    Wire.begin(sda, scl, 100000);

    int status = m_mpu.begin();
    Serial.print("m_mpu.begin() Status: ");
    Serial.println(status);

    if (status < 0) {
        Serial.println("MPU9250 NICHT gefunden!");
        m_connected = false;
        return;
    }

    m_mpu.setAccelRange(MPU9250::ACCEL_RANGE_8G);
    m_mpu.setGyroRange(MPU9250::GYRO_RANGE_500DPS);
    m_mpu.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_92HZ);
    m_mpu.setSrd(0);

    m_connected = true;
    Serial.println("MPU9250 verbunden.");
}


void MyMPU9250::update() {
    if (!m_connected) return;
    if (m_mpu.readSensor() < 0) return;

    float ax = m_mpu.getAccelX_mss();
    float ay = m_mpu.getAccelY_mss();
    float az = m_mpu.getAccelZ_mss();

    m_roll  = atan2(ay, az) * 180.0f / PI;
    m_pitch = atan2(-ax, az) * 180.0f / PI;

    // Yaw erstmal deaktiviert, weil Magnetometer langsam ist
    m_yaw = 0;
}


float MyMPU9250::getPitch() const { return m_pitch / 90.0f; }
float MyMPU9250::getRoll()  const { return m_roll / 90.0f;  }
float MyMPU9250::getYaw()   const { return m_yaw / 180.0f; }
bool MyMPU9250::isConnected() const { return m_connected; }
