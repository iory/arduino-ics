#ifndef ICS_HARD_SERIAL_H
#define ICS_HARD_SERIAL_H

#include "IcsBaseClass.h"
#include "mutex_helper.h"

/**
 * @class IcsHardSerialClass
 * @brief Derived class for hardware serial communication with Kondo KRS servo motors.
 */
class IcsHardSerialClass : public IcsBaseClass {
public:
    IcsHardSerialClass(HardwareSerial* serial,
                       long baudRate,
                       int timeout,
                       int enPin = -1,
                       int openDrainTxPin = -1);
    ~IcsHardSerialClass() override;

    bool begin();
    int setBaudrate(uint8_t id, int baudrate, bool changeSerialBaudrate = false) override;
    int changeBaudrate(int baudrate);
    uint32_t scanIDs() override;

protected:
    bool synchronize(uint8_t* txBuffer,
                     size_t txLength,
                     uint8_t* rxBuffer,
                     size_t rxLength) override;
    bool synchronizeImpl(uint8_t* txBuffer, size_t txLength, uint8_t* rxBuffer, size_t rxLength);

private:
    HardwareSerial* serial_;
    int8_t enPin_;
    int8_t openDrainTxPin_;
    long baudRate_;
    int timeout_;
    MutexHelper mutex_;
};

#endif  // ICS_HARD_SERIAL_H
