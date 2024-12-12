#include <Arduino.h>
#include <IcsHardSerialClass.h>

#ifdef ARDUINO_AVR_NANO_EVERY
#define DEBUG_SERIAL Serial
#else
#define DEBUG_SERIAL USBSerial
#endif

const byte SERVO_ID = 0;
const int TIMEOUT = 1000;
IcsHardSerialClass *krs;

unsigned int last_report_time = 0;
unsigned int sample_count = 0;
float measured_hz = 0;

void setup() {
  DEBUG_SERIAL.begin(115200);
  while (!DEBUG_SERIAL) {
    delay(10);
  }
  DEBUG_SERIAL.println("start");


  // Wait for board to stabilize
  delay(2000);

#ifdef ARDUINO_AVR_NANO_EVERY
  const int EN_PIN = -1;
  const long BAUDRATE = 115200;
  krs = new IcsHardSerialClass(&Serial1, BAUDRATE, TIMEOUT, EN_PIN);
#else
  const int EN_PIN = 6; // ATOM S3 pin
  const byte TX_PIN = 38;
  const byte RX_PIN = 5;
  const long BAUDRATE = 1250000;
  Serial1.begin(BAUDRATE, SERIAL_8E1, RX_PIN, TX_PIN, false, TIMEOUT);
  krs = new IcsHardSerialClass(&Serial1, BAUDRATE, TIMEOUT, EN_PIN);
#endif
  krs->begin();
}

void loop() {
  int position = krs->getPosition(SERVO_ID);
  if (position != -1) {
    sample_count++;
  }

  // Report every 1000 ms
  unsigned int current_time = millis();
  if (current_time - last_report_time >= 1000) {
    measured_hz = 1.0 * sample_count / ((current_time - last_report_time) / 1000.0); // Calculate Hz

    char log_msg[100];
    sprintf(log_msg, "Samples: %d, Hz: %d\n", sample_count, int(measured_hz));
    DEBUG_SERIAL.print(log_msg);
    sample_count = 0; // Reset sample count
    last_report_time = current_time; // Update last report time
  }
}
