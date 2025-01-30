#include <Arduino.h>
#include <IcsHardSerialClass.h>

#ifdef ARDUINO_AVR_NANO_EVERY
    #define DEBUG_SERIAL Serial
#else
    #define DEBUG_SERIAL USBSerial
#endif

#ifndef EN_PIN
    #define EN_PIN -1
#endif

#ifndef RX_PIN
    #define RX_PIN 18
#endif

#ifndef TX_PIN
    #define TX_PIN 17
#endif

const byte SERVO_ID = 0;
const int TIMEOUT = 1000;
IcsHardSerialClass *krs;

void setup() {
    DEBUG_SERIAL.begin(115200);
    while (!DEBUG_SERIAL) {
        delay(10);
    }
    DEBUG_SERIAL.println("start");

    // Wait for board to stabilize
    delay(2000);

#ifdef ARDUINO_AVR_NANO_EVERY
    const long BAUDRATE = 115200;
    krs = new IcsHardSerialClass(&Serial1, BAUDRATE, TIMEOUT, EN_PIN);
#else
    const long BAUDRATE = 1250000;
    Serial1.begin(BAUDRATE, SERIAL_8E1, RX_PIN, TX_PIN, false, TIMEOUT);
    krs = new IcsHardSerialClass(&Serial1, BAUDRATE, TIMEOUT, EN_PIN);
#endif
    krs->begin();
}

void loop() {
    while (krs->setServoPosition(SERVO_ID, 5000) == -1) {
        DEBUG_SERIAL.println("Try to send krs->setServoPosition(SERVO_ID, 5000)");
        delay(10);
    }
    while (abs(krs->getPosition(SERVO_ID) - 5000) >= 100) {
        delay(10);
    }
    while (krs->setServoPosition(SERVO_ID, 10000) == -1) {
        DEBUG_SERIAL.println("Try to send krs->setServoPosition(SERVO_ID, 10000)");
        delay(10);
    }
    while (abs(krs->getPosition(SERVO_ID) - 10000) >= 100) {
        delay(10);
    }
}
