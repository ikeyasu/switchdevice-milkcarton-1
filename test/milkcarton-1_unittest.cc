/* Copyright 2014 http://switchdevice.com */
/* vim: set ts=2  sw=2 sts=2 et si: */
/* astyle --style=google --indent=spaces=2 --pad-oper milkcarton-1_unittest.cc*/

#define DEBUG_LOG false

#include "gtest/gtest.h"
#include "arduino-mock/arduino.h"
#include "arduino-mock/serial.h"
Serial_ Serial;

#include "arduino-mock/spark.h"
Spark_ Spark;

#include "../milkcarton-1.ino"

using ::testing::StrCaseEq;
using ::testing::Return;

TEST(setup, normal) {
  SerialMock* serialMock = serialMockInstance();
  EXPECT_CALL(*serialMock, begin(9600));
  setup();
  releaseSerialMock();
}

TEST(detectMoving, normal) {
  // reset old value
  detectMoving(0, 0, 0);
  detectMoving(0, 0, 0);

  EXPECT_FALSE(detectMoving(0, 0, 0));
  EXPECT_TRUE(detectMoving(100, 100, 100));
  EXPECT_FALSE(detectMoving(0, 0, 0));
  EXPECT_FALSE(detectMoving(10, 10, 10));
  EXPECT_FALSE(detectMoving(0, 0, 0));
  EXPECT_TRUE(detectMoving(100, 0, 0));
}

TEST(publish, normal) {
  SparkMock* sparkMock = sparkMockInstance();
  EXPECT_CALL(*sparkMock,
              publish("accelerationDisptched", StrCaseEq("1,1,1,1")));
  publish(1, 1, 1);
  releaseSparkMock();
}

TEST(loop, normal) {
  // reset old value
  detectMoving(0, 0, 0);
  detectMoving(0, 0, 0);

  ArduinoMock* arduinoMock = arduinoMockInstance();
  SparkMock* sparkMock = sparkMockInstance();
  EXPECT_CALL(*arduinoMock, analogRead(5))
    .WillOnce(Return(0));
  EXPECT_CALL(*arduinoMock, analogRead(4))
    .WillOnce(Return(0));
  EXPECT_CALL(*arduinoMock, analogRead(3))
    .WillOnce(Return(0));
  EXPECT_CALL(*arduinoMock, delay(1000));
  loop();

  EXPECT_CALL(*arduinoMock, analogRead(5))
    .WillOnce(Return(110));
  EXPECT_CALL(*arduinoMock, analogRead(4))
    .WillOnce(Return(110));
  EXPECT_CALL(*arduinoMock, analogRead(3))
    .WillOnce(Return(110));
  EXPECT_CALL(*arduinoMock, millis())
    .WillOnce(Return(0));
  EXPECT_CALL(*sparkMock,
              publish("accelerationDisptched", StrCaseEq("1,6e,6e,6e")));
  EXPECT_CALL(*arduinoMock, delay(30));
  loop();

  releaseSparkMock();
  releaseArduinoMock();
}
