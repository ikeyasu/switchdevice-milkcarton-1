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

void publish(int x, int y, int z) {
  // analogRead return 0 to 4095.
  // "f,fff,fff,fff" as maximum
  // (format),(x),(y),(z)
  const int str_maxlen = 1 + 1 + 3 + 1 + 3 + 1 + 3 + 1;
  char str[str_maxlen];
  snprintf(str, str_maxlen, "1,%x,%x,%x", x, y, z);
  dp(str);
  Spark.publish("accelerationDisptched", str);
  dpXYZ(x, y, z);
}

bool detectMoving(int x, int y, int z) {
  static int oldX = 0;
  static int oldY = 0;
  static int oldZ = 0;
  bool ret = false;
  ret |= abs(oldX - x) >= CONDITION_X;
  ret |= abs(oldY - y) >= CONDITION_Y;
  ret |= abs(oldZ - z) >= CONDITION_Z;
  oldX = x;
  oldY = y;
  oldZ = z;
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
