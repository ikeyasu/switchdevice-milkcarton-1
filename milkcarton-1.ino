/* http://ikeyasu.com */
/* vim: set ts=2  sw=2 sts=2 et si: */
/* astyle --style=google --indent=spaces=2 --pad-oper carton-1.ino */

#ifndef DEBUG_LOG
#define DEBUG_LOG true
#endif
const int DELAY_DEFAULT = 1000; // msec
const int DELAY_MOVING = 30; // msec
const int MOVING_WINDOW_TIME = 1000; // msec

const int CONDITION_X = 100;
const int CONDITION_Y = 100;
const int CONDITION_Z = 100;

const int MAX_PUBLISH = 5;

void dp(char *str) {
  if (!DEBUG_LOG) return;
  Serial.println(str);
}

void dpXYZ(int x, int y, int z) {
  if (!DEBUG_LOG) return;
  Serial.print("X=");
  Serial.print(x);
  Serial.print("  Y=");
  Serial.print(y);
  Serial.print("  Z=");
  Serial.println(z);
}

const int PUBLISH_BUFFER_MAX_LEN = 63;
static char gPublishBuffer[PUBLISH_BUFFER_MAX_LEN] = {'2'};
static int gPublishBufferLen = 1;

void publishClear() {
  gPublishBufferLen = 1;
}

void publish(int x, int y, int z) {
  // analogRead return 0 to 4095.
  // fff is maximum
  // 2,(x),(y),(z),(x),(y),(z),(x),(y),(z)...(up to 63 bytes)..
  const int str_maxlen = 1 + 3 + 1 + 3 + 1 + 3 + 1;
  char str[str_maxlen];
  int len = snprintf(str, str_maxlen, ",%x,%x,%x", x, y, z);
  if (gPublishBufferLen + len > PUBLISH_BUFFER_MAX_LEN) {
    gPublishBuffer[gPublishBufferLen] = '\0';
    Spark.publish("accelerationDisptched", gPublishBuffer);
    publishClear();
  }
  strncpy(gPublishBuffer + gPublishBufferLen, str, len);
  gPublishBufferLen += len;
  dp(gPublishBuffer);
  dpXYZ(x, y, z);
}

void publishAll() {
  gPublishBuffer[gPublishBufferLen] = '\0';
  Spark.publish("accelerationDisptched", gPublishBuffer);
  publishClear();
}

bool detectMoving(int x, int y, int z) {
  bool ret = false;
  ret |= abs(x) >= CONDITION_X;
  ret |= abs(y) >= CONDITION_Y;
  ret |= abs(z) >= CONDITION_Z;
  return ret;
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  static int delayTime = DELAY_DEFAULT;
  static int startMoving = 0;
  int x = analogRead(5);
  int y = analogRead(4);
  int z = analogRead(3);

  if (startMoving == 0) {
    if (detectMoving(x, y, z)) {
      delayTime = DELAY_MOVING;
      startMoving = millis();
      publish(x, y, z);
    }
  } else {
    publish(x, y, z);
    if (millis() - startMoving >= MOVING_WINDOW_TIME) {
      delayTime = DELAY_DEFAULT;
      startMoving = 0;
    }
  }

  delay(delayTime);
}
