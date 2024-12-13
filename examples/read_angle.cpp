#include <Arduino.h>
#include <IcsHardSerialClass.h>
#include <vector>
#include <RunningStats.h>

std::vector<RunningStats> stats_vector;

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
    stats_vector.clear();
    for (int servo_id = 0; servo_id < 18; ++servo_id) {
      position = krs->getPosition(servo_id);
      if (position != -1) {
        ids.push_back(servo_id);
        RunningStats stats;
        stats_vector.push_back(stats);
      }
    }
  }
}

void loop() {
  cnt++;
  DEBUG_SERIAL.print("Pulse    ");
  for (size_t i = 0; i < ids.size(); ++i) {
    position = krs->getPosition(ids[i]);
    sprintf(log_msg, "%3d: %04d,", ids[i], position);
    DEBUG_SERIAL.print(log_msg);
    stats_vector[i].update(position);
  }
  DEBUG_SERIAL.print('\n');
  DEBUG_SERIAL.print("Mean     ");
  for (size_t i = 0; i < ids.size(); ++i) {
    float mean = stats_vector[i].mean();
    sprintf(log_msg, "%3d: %04d.%01d,", ids[i], int(mean), int(int(1000 * mean) % 1000 / 100));
    DEBUG_SERIAL.print(log_msg);
  }
  DEBUG_SERIAL.print('\n');
  DEBUG_SERIAL.print("Variance ");
  for (size_t i = 0; i < ids.size(); ++i) {
    float std = stats_vector[i].std();
    sprintf(log_msg, "%3d: %4d.%01d,", ids[i], int(std), int(int(1000 * std) % 1000 / 100));
    DEBUG_SERIAL.print(log_msg);
  }
  if (cnt % 100 == 0) {
    for (size_t i = 0; i < ids.size(); ++i) {
      stats_vector[i].reset();
    }
    cnt = 0;
  }
  DEBUG_SERIAL.print('\n');
}
