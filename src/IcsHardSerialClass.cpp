#include "IcsHardSerialClass.h"

IcsHardSerialClass::IcsHardSerialClass(HardwareSerial* serial, long baudRate, int timeout, int enPin)
    : serial_(serial), enPin_(enPin), baudRate_(baudRate), timeout_(timeout) {}

IcsHardSerialClass::~IcsHardSerialClass() {
    if (serial_) {
        serial_->end();
    }
}

bool IcsHardSerialClass::begin() {
    if (!serial_) {
        return false;
    }
    serial_->begin(baudRate_, SERIAL_8E1);
    serial_->setTimeout(timeout_);
    if (enPin_ >= 0) {
        pinMode(enPin_, OUTPUT);
        digitalWrite(enPin_, LOW);
    }
    return true;
}

bool IcsHardSerialClass::synchronize(uint8_t* txBuffer, size_t txLength, uint8_t* rxBuffer, size_t rxLength) {
    if (!serial_) {
        return false;
    }

    serial_->flush();

    if (enPin_ >= 0) {
        digitalWrite(enPin_, HIGH);
    }
    serial_->write(txBuffer, txLength);
    serial_->flush();
    if (enPin_ >= 0) {
        while (serial_->available() > 0) {
            serial_->read();
        }
        digitalWrite(enPin_, LOW);
    } else {
        int receivedBytes = 0;
        unsigned long startTime = millis();
        while (receivedBytes < 2) {
            if (serial_->available() > 0) {
                uint8_t byte = serial_->read();
                if (byte == txBuffer[receivedBytes]) {
                    receivedBytes++;
                } else {
                    startTime = millis();
                }
            }
            if (millis() - startTime > 1000) {
                return false;
            }
        }
    }

    size_t bytesRead = 0;
    unsigned long receiveStart = millis();

    while (bytesRead < rxLength) {
        if (serial_->available() > 0) {
            rxBuffer[bytesRead++] = serial_->read();
        }
        if (millis() - receiveStart > 1000) {
            return false;
        }
    }
    return bytesRead == rxLength;
}
