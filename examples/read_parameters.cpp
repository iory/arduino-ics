#include <Arduino.h>
#include <IcsHardSerialClass.h>
#include <vector>

#ifdef ARDUINO_AVR_NANO_EVERY
#define DEBUG_SERIAL Serial
#else
#define DEBUG_SERIAL USBSerial
#endif


#ifndef EN_PIN
#define EN_PIN -1
#endif

#ifndef RX_PIN
#define RX_PIN 18
#endif

#ifndef TX_PIN
#define TX_PIN 17
#endif

const int TIMEOUT = 10;
IcsHardSerialClass *krs;
std::vector<int> ids;
int position;
float current;
float temperature;
int stretch;
int speed;
char log_msg[50];
int cnt = 0;

void setup() {
  DEBUG_SERIAL.begin(115200);
  while (!DEBUG_SERIAL) {
    delay(10);
  }
  DEBUG_SERIAL.println("start");


  // Wait for board to stabilize
  delay(2000);

#ifdef ARDUINO_AVR_NANO_EVERY
  const long BAUDRATE = 115200;
  krs = new IcsHardSerialClass(&Serial1, BAUDRATE, TIMEOUT, EN_PIN);
#else
  const long BAUDRATE = 1250000;
  Serial1.begin(BAUDRATE, SERIAL_8E1, RX_PIN, TX_PIN, false, TIMEOUT);
  krs = new IcsHardSerialClass(&Serial1, BAUDRATE, TIMEOUT, EN_PIN);
#endif
  krs->begin();

  while (ids.size() == 0) {
    ids.clear();

    uint32_t idBits = krs->scanIDs();
    for (int servo_id = 0; servo_id < 32; ++servo_id) {
      if (static_cast<int>(idBits) & (1 << servo_id)) {
        ids.push_back(servo_id);
        krs->setServoHold(servo_id);
        krs->setStretch(servo_id, 127);
        krs->setSpeed(servo_id, 127);
        krs->setCurrentLimitInAmpere(servo_id, 4);
        krs->setTemperatureLimitInCelsius(servo_id, 80);
      }
    }
  }
}

void loop() {
  DEBUG_SERIAL.print("Pulse       ");
  for (size_t i = 0; i < ids.size(); ++i) {
    position = krs->getPosition(ids[i]);
    sprintf(log_msg, "%3d: %04d  ,", ids[i], position);
    DEBUG_SERIAL.print(log_msg);
  }
  DEBUG_SERIAL.print('\n');
  DEBUG_SERIAL.print("Current     ");
  for (size_t i = 0; i < ids.size(); ++i) {
    current = krs->getCurrentInAmpere(ids[i]);
    sprintf(log_msg, "%3d: %c%4d.%01d,", ids[i],
            current < 0 ? '-' : ' ',
            abs(int(current)), int(int(1000 * abs(current)) % 1000 / 100));
    DEBUG_SERIAL.print(log_msg);
  }
  DEBUG_SERIAL.print('\n');
  DEBUG_SERIAL.print("Temperature ");
  for (size_t i = 0; i < ids.size(); ++i) {
    temperature = krs->getTemperatureInCelsius(ids[i]);
    sprintf(log_msg, "%3d:  %4d.%01d,", ids[i],
            abs(int(temperature)), int(int(1000 * abs(temperature)) % 1000 / 100));
    DEBUG_SERIAL.print(log_msg);
  }
  DEBUG_SERIAL.print('\n');
  DEBUG_SERIAL.print("Stretch     ");
  for (size_t i = 0; i < ids.size(); ++i) {
    stretch = krs->getStretch(ids[i]);
    sprintf(log_msg, "%3d:     %3d,", ids[i], stretch);
    DEBUG_SERIAL.print(log_msg);
  }
  DEBUG_SERIAL.print('\n');
  DEBUG_SERIAL.print("Speed       ");
  for (size_t i = 0; i < ids.size(); ++i) {
    speed = krs->getSpeed(ids[i]);
    sprintf(log_msg, "%3d:     %3d,", ids[i], speed);
    DEBUG_SERIAL.print(log_msg);
  }
  DEBUG_SERIAL.print('\n');
  delay(10);
}
