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

TEST(setup, normal) {
  ArduinoMock* arduinoMock = arduinoMockInstance();
  SerialMock* serialMock = serialMockInstance();
  EXPECT_CALL(*serialMock, begin(9600));
  setup();
  delete serialMock;
  delete arduinoMock;
}

TEST(detectMoving, normal) {
  EXPECT_FALSE(detectMoving(0, 0, 0));
  EXPECT_TRUE(detectMoving(100, 100, 100));
  EXPECT_TRUE(detectMoving(0, 0, 0));
  EXPECT_FALSE(detectMoving(10, 10, 10));
  EXPECT_FALSE(detectMoving(0, 0, 0));
  EXPECT_TRUE(detectMoving(100, 0, 0));
}

TEST(publish, normal) {
  SparkMock* sparkMock = sparkMockInstance();
  EXPECT_CALL(*sparkMock,
              publish("accelerationDisptched", StrCaseEq("1,1,1,1")));
  publish(1, 1, 1);
  delete sparkMock;
}
