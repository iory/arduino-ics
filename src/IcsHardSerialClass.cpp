#include "IcsHardSerialClass.h"

IcsHardSerialClass::IcsHardSerialClass(HardwareSerial* serial, long baudRate, int timeout, int enPin, int openDrainTxPin)
  : serial_(serial), enPin_(enPin), baudRate_(baudRate), timeout_(timeout), openDrainTxPin_(openDrainTxPin) {}

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
  if (openDrainTxPin_ >= 0) {
    digitalWrite(openDrainTxPin_, LOW);
  }
  serial_->write(txBuffer, txLength);
  serial_->flush();
  if (openDrainTxPin_ >= 0) {
    digitalWrite(openDrainTxPin_, HIGH);
  }
  if (enPin_ >= 0) {
    while (serial_->available() > 0) {
      serial_->read();
    }
    digitalWrite(enPin_, LOW);
  } else {
    int receivedBytes = 0;
    unsigned long startTime = millis();
    while (receivedBytes < txLength) {
      if (serial_->available() > 0) {
        uint8_t byte = serial_->read();
        if (byte == txBuffer[receivedBytes]) {
          receivedBytes++;
        } else {
          receivedBytes = 0;
        }
        startTime = millis();
      }
      if (millis() - startTime > timeout_) {
        return false;
      }
    }
  }

  size_t bytesRead = 0;
  unsigned long receivedTime = millis();
  unsigned long adjustedTimeout = timeout_;
  if (txLength > 64) {
    // overwrite eeprom takes too much time.
    adjustedTimeout *= 50;
  }

  while (bytesRead < rxLength) {
    size_t availableBytes = serial_->available();
    if (availableBytes > 0) {
      size_t toRead = min(rxLength - bytesRead, availableBytes);
      size_t readCount = serial_->readBytes(&rxBuffer[bytesRead], toRead);
      bytesRead += readCount;
      receivedTime = millis();
    }
    if (millis() - receivedTime > adjustedTimeout) {
      return false;
    }
  }
  return bytesRead == rxLength;
}

int IcsHardSerialClass::setBaudrate(uint8_t id, int baudrate, bool changeSerialBaudrate) {
  int ret = IcsBaseClass::setBaudrate(id, baudrate);
  if (ret == ICS_FALSE) {
    return ICS_FALSE;
  }
  if (changeSerialBaudrate) {
    baudRate_ = baudrate;
    serial_->begin(baudRate_, SERIAL_8E1);
  }
}

int IcsHardSerialClass::changeBaudrate(int baudrate) {
  if (baudrate != 1250000 && baudrate != 625000 && baudrate != 115200) {
    return ICS_FALSE;
  }
  baudRate_ = baudrate;
  serial_->begin(baudRate_, SERIAL_8E1);
  return 0;
}

uint32_t IcsHardSerialClass::scanIDs() {
  uint32_t idBitmap = 0;
  int currentBaudrate = baudRate_;
  int baudrates[3] = { 115200, 625000, 1250000 };
  for (size_t i = 0; i < 3; ++i) {
    changeBaudrate(baudrates[i]);
    for (uint8_t id = 0; id <= 31; ++id) {
      if (getPosition(id) != ICS_FALSE) {
        if (currentBaudrate != baudrates[i]) {
          setBaudrate(id, currentBaudrate);
        }
        idBitmap |= (1UL << id);
      }
    }
  }
  changeBaudrate(currentBaudrate);
  return idBitmap;
}
