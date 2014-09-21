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
              publish("accelerationDisptched",
              StrCaseEq("2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1")));
  for (int i = 0; i < 11; i++) {
    publish(1, 1, 1);
  }
  publishClear();
  releaseSparkMock();
}

TEST(publish, all) {
  SparkMock* sparkMock = sparkMockInstance();
  EXPECT_CALL(*sparkMock,
              publish("accelerationDisptched",
              StrCaseEq("2,1,1,1")));
  publish(1,1,1);
  publishAll();
  releaseSparkMock();
}

TEST(loop, normal) {
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

  EXPECT_CALL(*arduinoMock, analogRead(4))
    .WillRepeatedly(Return(110));
  EXPECT_CALL(*arduinoMock, analogRead(3))
    .WillRepeatedly(Return(110));
  EXPECT_CALL(*arduinoMock, millis())
    .WillRepeatedly(Return(0));
  EXPECT_CALL(*sparkMock,
              publish("accelerationDisptched",
              StrCaseEq("2,0,6e,6e,1,6e,6e,2,6e,6e,3,6e,6e,4,6e,6e,5,6e,6e,6,6e,6e")));
  EXPECT_CALL(*arduinoMock, delay(30)).Times(10);
  for (int i = 0; i < 10; i++) {
    EXPECT_CALL(*arduinoMock, analogRead(5))
      .WillRepeatedly(Return(i));
    loop();
  }
  EXPECT_CALL(*sparkMock,
              publish("accelerationDisptched",
              StrCaseEq("2,7,6e,6e,8,6e,6e,9,6e,6e")));
  publishAll();

  releaseSparkMock();
  releaseArduinoMock();
}
