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

    while (serial_->available() > 0) {
      serial_->read();
    }
    if (enPin_ >= 0) {
        digitalWrite(enPin_, LOW);
    }
    // If enPin_ is not used, read and discard the first 2 bytes
    if (enPin_ < 0) {
        uint8_t discardBuffer[2];
        if (serial_->readBytes(discardBuffer, 2) != 2) {
            return false; // Failed to read the extra bytes
        }
    }
    size_t bytesRead = serial_->readBytes(rxBuffer, rxLength);
    return bytesRead == rxLength;
}
