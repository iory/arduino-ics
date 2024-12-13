#ifndef ICS_BASE_H
#define ICS_BASE_H

#include <Arduino.h>

/**
 * @class IcsBaseClass
 * @brief Base class for controlling Kondo KRS servo motors via ICS 3.5/3.6 protocol.
 */
class IcsBaseClass {
public:
    // Servo ID range
    static constexpr int MAX_ID = 31;
    static constexpr int MIN_ID = 0;

    // Position limits
    static constexpr int MAX_POS = 11500;
    static constexpr int MIN_POS = 3500;

    // Error handling
    static constexpr int ICS_FALSE = -1;

    // Parameter limits
    static constexpr int MAX_PARAM = 127;
    static constexpr int MIN_PARAM = 1;

  // Angle limits
  static constexpr float MAX_DEG = 180.0f;
  static constexpr float MIN_DEG = -180.0f;

  IcsBaseClass() = default;
  virtual ~IcsBaseClass() = default;

  // Servo methods
  virtual int setServoPosition(uint8_t id, uint16_t pos);
  virtual int setServoFree(uint8_t id);

  // Parameter setting
  virtual int setStretch(uint8_t id, uint8_t value);
  virtual int setSpeed(uint8_t id, uint8_t value);
  virtual int setCurrentLimit(uint8_t id, uint8_t value);
  virtual int setTemperatureLimit(uint8_t id, uint8_t value);
  virtual int setID(uint8_t id);
  virtual int setBaudrate(uint8_t id, int baudrate, bool changeSerialBaudrate = false);

  // Data retrieval
  virtual int getStretch(uint8_t id);
  virtual int getSpeed(uint8_t id);
  virtual int getCurrent(uint8_t id);
  virtual int getTemperature(uint8_t id);
  virtual int getPosition(uint8_t id);
  virtual int getID();
  virtual int getBaudrate(uint8_t id);

  virtual int getEEPROM(uint8_t id, uint8_t* rxBuffer);
  virtual int setEEPROM(uint8_t id, uint8_t* eepromData);

  virtual int isRotationMode(uint8_t id);
  virtual int setRotationMode(uint8_t id, bool rotation_mode);

protected:
    // ID and range validation
    uint8_t validateId(uint8_t id) const;
    bool validateRange(int value, int min, int max) const;

    // Angle conversion
    static int angleToPosition(float angle);
    static float positionToAngle(int position);

    // Communication method (to be implemented by derived classes)
    virtual bool synchronize(uint8_t* txBuffer, size_t txLength, uint8_t* rxBuffer, size_t rxLength) = 0;
};

#endif // ICS_BASE_H
