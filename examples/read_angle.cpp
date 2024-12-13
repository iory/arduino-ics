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
char log_msg[50];

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

  for (int servo_id = 0; servo_id < 18; ++servo_id) {
    position = krs->getPosition(servo_id);
    if (position != -1) {
      ids.push_back(servo_id);
    }
  }
}

void loop() {
  for (size_t i = 0; i < ids.size(); ++i) {
    position = krs->getPosition(ids[i]);
    sprintf(log_msg, "%3d: %04d,", ids[i], position);
    DEBUG_SERIAL.print(log_msg);
  }
  DEBUG_SERIAL.print('\n');
}
