#include "IcsBaseClass.h"

uint8_t IcsBaseClass::validateId(uint8_t id) const {
    return (id >= MIN_ID && id <= MAX_ID) ? id : 0xFF;
}

bool IcsBaseClass::validateRange(int value, int min, int max) const {
    return value >= min && value <= max;
}

int IcsBaseClass::setServoPosition(uint8_t id, uint16_t pos) {
    if (validateId(id) == 0xFF || !validateRange(pos, MIN_POS, MAX_POS)) {
        return ICS_FALSE;
    }
    uint8_t txCmd[3] = {0x80 + id, static_cast<uint8_t>((pos >> 7) & 0x7F), static_cast<uint8_t>(pos & 0x7F)};
    uint8_t rxCmd[3];
    if (!synchronize(txCmd, sizeof(txCmd), rxCmd, sizeof(rxCmd))) {
        return ICS_FALSE;
    }
    return ((rxCmd[1] << 7) & 0x3F80) | (rxCmd[2] & 0x007F);
}

int IcsBaseClass::setServoFree(uint8_t id) {
    if (validateId(id) == 0xFF) {
        return ICS_FALSE;
    }
    uint8_t txCmd[3] = {0x80 + id, 0, 0};
    uint8_t rxCmd[3];
    if (!synchronize(txCmd, sizeof(txCmd), rxCmd, sizeof(rxCmd))) {
        return ICS_FALSE;
    }
    return ((rxCmd[1] << 7) & 0x3F80) | (rxCmd[2] & 0x007F);
}

int IcsBaseClass::setStretch(uint8_t id, uint8_t value) {
    if (validateId(id) == 0xFF || !validateRange(value, MIN_PARAM, MAX_PARAM)) {
        return ICS_FALSE;
    }
    uint8_t txCmd[3] = {static_cast<uint8_t>(0xC0 + id), 0x01, value};
    uint8_t rxCmd[3];
    if (!synchronize(txCmd, sizeof(txCmd), rxCmd, sizeof(rxCmd))) {
        return ICS_FALSE;
    }
    return rxCmd[2]; // Return the acknowledged stretch value
}

int IcsBaseClass::setSpeed(uint8_t id, uint8_t value) {
    if (validateId(id) == 0xFF || !validateRange(value, MIN_PARAM, MAX_PARAM)) {
        return ICS_FALSE;
    }
    uint8_t txCmd[3] = {static_cast<uint8_t>(0xC0 + id), 0x02, value};
    uint8_t rxCmd[3];
    if (!synchronize(txCmd, sizeof(txCmd), rxCmd, sizeof(rxCmd))) {
        return ICS_FALSE;
    }
    return rxCmd[2]; // Return the acknowledged speed value
}

int IcsBaseClass::getStretch(uint8_t id) {
    if (validateId(id) == 0xFF) {
        return ICS_FALSE;
    }
    uint8_t txCmd[2] = {static_cast<uint8_t>(0xA0 + id), 0x01};
    uint8_t rxCmd[3];
    if (!synchronize(txCmd, sizeof(txCmd), rxCmd, sizeof(rxCmd))) {
        return ICS_FALSE;
    }
    return rxCmd[2]; // Return the retrieved stretch value
}

int IcsBaseClass::getSpeed(uint8_t id) {
    if (validateId(id) == 0xFF) {
        return ICS_FALSE;
    }
    uint8_t txCmd[2] = {static_cast<uint8_t>(0xA0 + id), 0x02};
    uint8_t rxCmd[3];
    if (!synchronize(txCmd, sizeof(txCmd), rxCmd, sizeof(rxCmd))) {
        return ICS_FALSE;
    }
    return rxCmd[2]; // Return the retrieved speed value
}

int IcsBaseClass::getID() {
  uint8_t txCmd[4] = {0xFF, 0x00, 0x00, 0x00};
  uint8_t rxCmd[1];
  if (!synchronize(txCmd, sizeof(txCmd), rxCmd, sizeof(rxCmd))) {
    return ICS_FALSE;
  }
  delay(520);
  return 0x1F & rxCmd[0];
}

int IcsBaseClass::setID(uint8_t id) {
  if (validateId(id) == 0xFF) {
    return ICS_FALSE;
  }
  uint8_t txCmd[4] = {static_cast<uint8_t>(0xE0 + id), 0x01, 0x01, 0x01};
  uint8_t rxCmd[1];
  if (!synchronize(txCmd, sizeof(txCmd), rxCmd, sizeof(rxCmd))) {
    return ICS_FALSE;
  }
  delay(520);
  return 0x1F & rxCmd[0];
}

int IcsBaseClass::setCurrentLimit(uint8_t id, uint8_t value) {
  if (validateId(id) == 0xFF) {
    return ICS_FALSE;
  }
  uint8_t txCmd[3] = {static_cast<uint8_t>(0xC0 + id), 0x03, value};
  uint8_t rxCmd[3];
  if (!synchronize(txCmd, sizeof(txCmd), rxCmd, sizeof(rxCmd))) {
    return ICS_FALSE;
  }
  return rxCmd[sizeof(rxCmd) - 1];
}

int IcsBaseClass::setTemperatureLimit(uint8_t id, uint8_t value) {
  if (validateId(id) == 0xFF) {
    return ICS_FALSE;
  }
  uint8_t txCmd[3] = {static_cast<uint8_t>(0xC0 + id), 0x04, value};
  uint8_t rxCmd[3];
  if (!synchronize(txCmd, sizeof(txCmd), rxCmd, sizeof(rxCmd))) {
    return ICS_FALSE;
  }
  return rxCmd[sizeof(rxCmd) - 1];
}

int IcsBaseClass::getCurrent(uint8_t id) {
  if (validateId(id) == 0xFF) {
    return ICS_FALSE;
  }
  uint8_t txCmd[2] = {static_cast<uint8_t>(0xA0 + id), 0x03};
  uint8_t rxCmd[3];
  if (!synchronize(txCmd, sizeof(txCmd), rxCmd, sizeof(rxCmd))) {
    return ICS_FALSE;
  }
  return rxCmd[sizeof(rxCmd) - 1];
}

int IcsBaseClass::getTemperature(uint8_t id) {
  if (validateId(id) == 0xFF) {
    return ICS_FALSE;
  }
  uint8_t txCmd[2] = {static_cast<uint8_t>(0xA0 + id), 0x04};
  uint8_t rxCmd[3];
  if (!synchronize(txCmd, sizeof(txCmd), rxCmd, sizeof(rxCmd))) {
    return ICS_FALSE;
  }
  return rxCmd[sizeof(rxCmd) - 1];
}

int IcsBaseClass::getPosition(uint8_t id) {
  if (validateId(id) == 0xFF) {
    return ICS_FALSE;
  }
  uint8_t txCmd[2] = {static_cast<uint8_t>(0xA0 + id), 0x05};
  uint8_t rxCmd[4];
  if (!synchronize(txCmd, sizeof(txCmd), rxCmd, sizeof(rxCmd))) {
    return ICS_FALSE;
  }
  return ((rxCmd[2] << 7) & 0x3F80) | (rxCmd[3] & 0x007F);
}

int IcsBaseClass::getEEPROM(uint8_t id, uint8_t* rxBuffer) {
  if (validateId(id) == 0xFF) {
    return ICS_FALSE;
  }
  uint8_t txCmd[2] = {static_cast<uint8_t>(0xA0 + id), 0x00};

  if (!synchronize(txCmd, sizeof(txCmd), rxBuffer, 66)) {
    return ICS_FALSE;
  }
  return 0;
}

int IcsBaseClass::setEEPROM(uint8_t id, uint8_t* eepromData) {
  if (validateId(id) == 0xFF) {
    return ICS_FALSE;
  }
  uint8_t txCmd[66];
  uint8_t rxCmd[2];
  txCmd[0] = static_cast<uint8_t>(0xC0 + id);
  txCmd[1] = 0x00;
  memcpy(&txCmd[2], eepromData, 64);

  if (!synchronize(txCmd, sizeof(txCmd), rxCmd, sizeof(rxCmd))) {
    return ICS_FALSE;
  }
  return 0;
}

int IcsBaseClass::isRotationMode(uint8_t id) {
  if (validateId(id) == 0xFF) {
    return ICS_FALSE;
  }
  uint8_t rxBuffer[66];
  if (getEEPROM(id, rxBuffer) == ICS_FALSE) {
    return ICS_FALSE;
  };
  return (rxBuffer[2 + 14] & 0x01) == 0x01;
}

int IcsBaseClass::setRotationMode(uint8_t id, bool rotation_mode) {
  if (validateId(id) == 0xFF) {
    return ICS_FALSE;
  }
  uint8_t rxBuffer[66];
  if (getEEPROM(id, rxBuffer) == ICS_FALSE) {
    return ICS_FALSE;
  };
  bool current_rotation_mode = (rxBuffer[2 + 14] & 0x1) == 0x01;
  if (rotation_mode != current_rotation_mode) {
    if (rotation_mode) {
      rxBuffer[2 + 14] |= (1 << 0);
    } else {
      rxBuffer[2 + 14] = rxBuffer[2 + 14] & (~(1 << 0));
    }
    return setEEPROM(id, rxBuffer + 2);
  }
  return 0;
}

int IcsBaseClass::getBaudrate(uint8_t id) {
  if (validateId(id) == 0xFF) {
    return ICS_FALSE;
  }
  uint8_t rxBuffer[66];
  if (getEEPROM(id, rxBuffer) == ICS_FALSE) {
    return ICS_FALSE;
  };
  switch (rxBuffer[2 + 27] & 0x0F) {
  case 0:
    return 1250000;
  case 1:
    return 625000;
  case 10:
    return 115200;
  default:
    return 115200;
  }
}

int IcsBaseClass::setBaudrate(uint8_t id, int baudrate, bool changeSerialBaudrate) {
  if (validateId(id) == 0xFF) {
    return ICS_FALSE;
  }
  if (baudrate != 1250000 && baudrate != 625000 && baudrate != 115200) {
    return ICS_FALSE;
  }
  uint8_t rxBuffer[66];
  if (getEEPROM(id, rxBuffer) == ICS_FALSE) {
    return ICS_FALSE;
  };
  if (baudrate == 1250000)
    rxBuffer[2 + 27] = 0;
  else if (baudrate == 625000)
    rxBuffer[2 + 27] = 1;
  else
    rxBuffer[2 + 27] = 10;
  return setEEPROM(id, rxBuffer + 2);
}

uint32_t IcsBaseClass::scanIDs() {
  uint32_t idBitmap = 0;
  int position;
  for (uint8_t id = 0; id <= 31; ++id) {
    position = getPosition(id);
    if (position != ICS_FALSE) {
      idBitmap |= (1UL << id);
    }
  }
  return idBitmap;
}
