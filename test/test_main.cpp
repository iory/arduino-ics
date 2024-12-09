#include <Arduino.h>
#include <unity.h>

#include <IcsHardSerialClass.h>

// const byte EN_PIN = -1;
// const byte TX_PIN = 17;
// const byte RX_PIN = 18;
const byte EN_PIN = 6;
const byte RX_PIN = 5;
const byte TX_PIN = 38;

const byte SERVO_ID = 0;


const long BAUDRATE = 1250000;
const int TIMEOUT = 1000;
IcsHardSerialClass *krs;

void setUp(void) {
}

void tearDown(void) {
}

void test_krs_read(void) {
  int position = krs->getPosition(SERVO_ID);
  TEST_ASSERT_NOT_EQUAL(-1, position);
}

void test_krs_setServoPosition(void) {
  int position = krs->getPosition(SERVO_ID);
  while (krs->setServoPosition(SERVO_ID, 7500) == -1) {
    delay(10);
  };
  delay(1000);
  while (krs->setServoPosition(SERVO_ID, 10000) == -1) {
    delay(10);
  };
  delay(2000);
  position = krs->setServoFree(SERVO_ID);
  TEST_ASSERT_INT_WITHIN(100, 10000, position);
}


void test_krs_setStretch(void) {
  krs->setStretch(SERVO_ID, 100);
  delay(100);
  TEST_ASSERT_EQUAL_INT(krs->getStretch(SERVO_ID), 100);
  delay(100);
  TEST_ASSERT_NOT_EQUAL(-1, krs->setStretch(SERVO_ID, 30));
  delay(100);
  TEST_ASSERT_EQUAL_INT(krs->getStretch(SERVO_ID), 30);
}

void test_krs_setSpeed(void) {
  krs->setSpeed(SERVO_ID, 100);
  delay(100);
  TEST_ASSERT_EQUAL_INT(krs->getSpeed(SERVO_ID), 100);
  delay(100);
  TEST_ASSERT_NOT_EQUAL(-1, krs->setSpeed(SERVO_ID, 30));
  delay(100);
  TEST_ASSERT_EQUAL_INT(krs->getSpeed(SERVO_ID), 30);
}

void test_krs_setCurrentLimit(void) {
  krs->setCurrentLimit(SERVO_ID, 20);
  delay(100);
  TEST_ASSERT_NOT_EQUAL(-1, krs->getCurrent(SERVO_ID));
  delay(100);
  TEST_ASSERT_NOT_EQUAL(-1, krs->setCurrentLimit(SERVO_ID, 10));
  delay(100);
  TEST_ASSERT_NOT_EQUAL(-1, krs->getCurrent(SERVO_ID));
}

void test_krs_setTemperatureLimit(void) {
  krs->setTemperatureLimit(SERVO_ID, 20);
  delay(100);
  TEST_ASSERT_NOT_EQUAL(-1, krs->getTemperature(SERVO_ID));
}


void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN(); // IMPORTANT LINE!

  Serial1.begin(BAUDRATE, SERIAL_8E1, RX_PIN, TX_PIN, false, TIMEOUT);
  krs = new IcsHardSerialClass(&Serial1, BAUDRATE, TIMEOUT, EN_PIN);
  krs->begin();

  RUN_TEST(test_krs_read);
  RUN_TEST(test_krs_setServoPosition);
  RUN_TEST(test_krs_setStretch);
  RUN_TEST(test_krs_setSpeed);
  RUN_TEST(test_krs_setCurrentLimit);
  RUN_TEST(test_krs_setTemperatureLimit);
  UNITY_END(); // stop unit testing
}


void loop() {}
