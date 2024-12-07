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

int IcsBaseClass::getCurrentLimit(uint8_t id) {
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

int IcsBaseClass::getTemperatureLimit(uint8_t id) {
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
