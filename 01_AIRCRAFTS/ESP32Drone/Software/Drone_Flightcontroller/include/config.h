#ifndef CONFIG_H
#define CONFIG_H



// === SBUS ===
#define SBUS_RX_PIN      20
#define SBUS_INVERTED    true
#define SBUS_SIGNAL_TIMEOUT_MS 1000
#define SBUS_SMOOTHING_ALPHA  0.8f  
#define SBUS_INPUT_MIN 305
#define SBUS_INPUT_MAX 1700
// === PWM ===
#define PWM_FREQ         200
#define PWM_RES_BITS     10
#define PWM_MAX_VALUE    ((1 << PWM_RES_BITS) - 1)

// PWM-Kanäle
#define PWM_CH_MOTOR1    0
#define PWM_CH_MOTOR2    1
#define PWM_CH_MOTOR3    2
#define PWM_CH_MOTOR4    3

// Pins für Motor-PWM
#define PIN_MOTOR1       5
#define PIN_MOTOR2       0
#define PIN_MOTOR3       3
#define PIN_MOTOR4       1

// === IMU (MPU6050) ===
#define IMU_SDA_PIN      8
#define IMU_SCL_PIN      9
#define CORRECTION_BLEND_FACTOR  0.3f
#define MPU_COMPLEMENTARY_ALPHA 0.9f


// === PID Parameter ===
#define PID_ROLL_KP      1.2f
#define PID_ROLL_KI      0.0f
#define PID_ROLL_KD      0.0f

#define PID_PITCH_KP     1.2f
#define PID_PITCH_KI     0.0f
#define PID_PITCH_KD     0.0f

// Akkumessung
#define PIN_VBAT         4       // GPIO-Pin für Spannungsteiler
#define VBAT_R1          100000.0 // Oben (zwischen Akku+ und ADC)
#define VBAT_R2          100000.0 // Unten (zwischen ADC und GND)
#define VBAT_ADC_REF     2.78      // Referenzspannung des ADC
#define VBAT_ADC_RES     4095.0   // 12-bit ADC-Auflösung

#define VBAT_WARNING_THRESHOLD 3.5  
#define PIN_LED_WARNING 10 
#define VBatt_WARNING_Timeout 5000


#endif
