#include "SbusReceiver.h"

void SbusReceiver::begin() {
    Serial1.begin(100000, SERIAL_8E2, SBUS_RX_PIN, -1, SBUS_INVERTED);
    delay(100);
    while (Serial1.available()) Serial1.read();
    m_sbus.Begin();
}

void SbusReceiver::update() {
    if (m_sbus.Read()) {
        m_data = m_sbus.data();
        lastSignalTime = millis();
        signalAvailable = true;     
    }
}

float SbusReceiver::mapChannel(int raw, bool symmetric) {
    const int inMin = SBUS_INPUT_MIN;
    const int inMax = SBUS_INPUT_MAX;
    
    if (symmetric) {
        return constrain(((float)(raw - inMin) / (inMax - inMin)) * 2.0f - 1.0f, -1.0f, 1.0f);
    } else {
        return constrain((float)(raw - inMin) / (inMax - inMin), 0.0f, 1.0f);
    }
}

float SbusReceiver::getRoll() {
    float raw = mapChannel(m_data.ch[0]);
    m_filteredRoll = SBUS_SMOOTHING_ALPHA * raw + (1.0f - SBUS_SMOOTHING_ALPHA) * m_filteredRoll;
    return m_filteredRoll;
}

float SbusReceiver::getPitch() {
    float raw = mapChannel(m_data.ch[1]);
    m_filteredPitch = SBUS_SMOOTHING_ALPHA * raw + (1.0f - SBUS_SMOOTHING_ALPHA) * m_filteredPitch;
    return m_filteredPitch;
}

float SbusReceiver::getThrottle() {
    float raw = mapChannel(m_data.ch[2], false);
    m_filteredThrottle = SBUS_SMOOTHING_ALPHA * raw + (1.0f - SBUS_SMOOTHING_ALPHA) * m_filteredThrottle;
    return m_filteredThrottle;
}

float SbusReceiver::getYaw() {
    float raw = mapChannel(m_data.ch[3]);
    m_filteredYaw = SBUS_SMOOTHING_ALPHA * raw + (1.0f - SBUS_SMOOTHING_ALPHA) * m_filteredYaw;
    return m_filteredYaw;
}

float SbusReceiver::getRawChannel(int ch) {return m_data.ch[ch];}

bool SbusReceiver::isSignalLost() {
    return m_data.failsafe || (millis() - lastSignalTime) > SBUS_SIGNAL_TIMEOUT_MS;
}

bool SbusReceiver::isArmedSwitchOn() {
    return getRawChannel(9) > 1000;
}
