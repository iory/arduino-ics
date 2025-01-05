#include "IcsBaseClass.h"

const float IcsBaseClass::setting_value_to_temperature[128] = {
  118, 117.0588, 116.4706, 115.8824, 115.2941, 114.7059, 114.1176, 113.5294,
  112.9412, 112.3529, 111.7647, 111.1765, 110.5882, 110.0, 109.4118, 108.8235,
  108.2353, 107.6471, 107.0588, 106.4706, 105.8824, 105.2941, 104.7059, 104.1176,
  103.5294, 102.9412, 102.3529, 101.7647, 101.1765, 100.5882, 100.0, 99.4118, 98.8235,
  98.2353, 97.6471, 97.0588, 96.4706, 95.8824, 95.2941, 94.7059, 94.1176, 93.5294,
  92.9412, 92.3529, 91.7647, 91.1765, 90.5882, 90.0, 89.2308, 88.4615, 87.6923, 86.9231,
  86.1538, 85.3846, 84.6154, 83.8462, 83.0769, 82.3077, 81.5385, 80.7692, 80.0, 79.3333,
  78.6667, 78.0, 77.3333, 76.6667, 76.0, 75.3333, 74.6667, 74.0, 73.3333, 72.6667, 72.0,
  71.3333, 70.6667, 70.0, 69.1667, 68.3333, 67.5, 66.6667, 65.8333, 65.0, 64.1667, 63.3333,
  62.5, 61.6667, 60.8333, 60.0, 59.1667, 58.3333, 57.5, 56.6667, 55.8333, 55.0, 54.1667,
  53.3333, 52.5, 51.6667, 50.8333, 50.0, 49.1667, 48.3333, 47.5, 46.6667, 45.8333, 45.0,
  44.1667, 43.3333, 42.5, 41.6667, 40.8333, 40.0, 39.1667, 38.3333, 37.5, 36.6667, 35.8333,
  35.0, 34.1667, 33.3333, 32.5, 31.6667, 30.8333, 30.0, 29.1667, 28.3333, 27.5, 26.6667};


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

int IcsBaseClass::setServoHold(uint8_t id) {
  if (validateId(id) == 0xFF) {
    return ICS_FALSE;
  }
  int position = getPosition(id);
  if (position == ICS_FALSE) {
    return ICS_FALSE;
  }
  return setServoPosition(id, position);
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
  if (value < 0) value = 0;
  if (value > 63) value = 63;
  uint8_t txCmd[3] = {static_cast<uint8_t>(0xC0 + id), 0x03, value};
  uint8_t rxCmd[3];
  if (!synchronize(txCmd, sizeof(txCmd), rxCmd, sizeof(rxCmd))) {
    return ICS_FALSE;
  }
  return rxCmd[sizeof(rxCmd) - 1];
}

int IcsBaseClass::setCurrentLimitInAmpere(uint8_t id, float value) {
  return setCurrentLimit(id, int(value * 10.0));
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

int IcsBaseClass::setTemperatureLimitInCelsius(uint8_t id, float temperature) {
  int setting_value = getSettingValueFromTemperature(temperature);
  if (setting_value == -1) {
    return -1;
  }
  return setTemperatureLimit(id, setting_value);
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

float IcsBaseClass::getCurrentInAmpere(uint8_t id) {
  int current_value = getCurrent(id);
  if (current_value == -1) {
    return -1000;
  }
  int sign = 1;
  if (current_value >= 64) {
    sign = -1;
    current_value -= 64;
  }
  return sign * current_value / 10.0;
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

float IcsBaseClass::getTemperatureInCelsius(uint8_t id) {
  int setting_value = getTemperature(id);
  if (setting_value == ICS_FALSE) {
    return ICS_FALSE;
  }
  return setting_value_to_temperature[setting_value];
}

int IcsBaseClass::getSettingValueFromTemperature(float temperature) {
  for (int i = 0; i < setting_value_to_temperature_size - 1; ++i) {
    if (setting_value_to_temperature[i] >= temperature && temperature > setting_value_to_temperature[i + 1]) {
      return i + 1;
    }
  }
  if (temperature >= setting_value_to_temperature[0]) {
    return 1;
  }
  if (temperature <= setting_value_to_temperature[setting_value_to_temperature_size - 1]) {
    return setting_value_to_temperature_size;
  }
  return -1;
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
  if (eepromData == nullptr || validateId(id) == 0xFF) {
    return ICS_FALSE;
  }
  uint8_t txCmd[66] = {0};
  uint8_t rxCmd[2] = {0};
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
