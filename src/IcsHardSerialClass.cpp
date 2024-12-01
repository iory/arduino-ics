#include "IcsHardSerialClass.h"

IcsHardSerialClass::IcsHardSerialClass(HardwareSerial* serial, long baudRate, int timeout, uint8_t enPin)
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
        digitalWrite(enPin_, HIGH); // Enable transmission
    }
    serial_->write(txBuffer, txLength);
    serial_->flush();
    delayMicroseconds(100); // Ensure data transmission
    if (enPin_ >= 0) {
        digitalWrite(enPin_, LOW); // Enable reception
    }
    size_t bytesRead = serial_->readBytes(rxBuffer, rxLength);
    return bytesRead == rxLength;
}
