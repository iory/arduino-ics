#ifndef ICS_HARD_SERIAL_H
#define ICS_HARD_SERIAL_H

#include "IcsBaseClass.h"

/**
 * @class IcsHardSerialClass
 * @brief Derived class for hardware serial communication with Kondo KRS servo motors.
 */
class IcsHardSerialClass : public IcsBaseClass {
public:
    IcsHardSerialClass(HardwareSerial* serial, long baudRate, int timeout, uint8_t enPin = -1);
    ~IcsHardSerialClass() override;

    bool begin();

protected:
    bool synchronize(uint8_t* txBuffer, size_t txLength, uint8_t* rxBuffer, size_t rxLength) override;

private:
    HardwareSerial* serial_;
    int8_t enPin_;
    long baudRate_;
    int timeout_;
};

#endif // ICS_HARD_SERIAL_H
