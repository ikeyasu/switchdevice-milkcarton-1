/* http://ikeyasu.com */
/* vim: set ts=2  sw=2 sts=2 et si: */
/* astyle --style=google --indent=spaces=2 --pad-oper carton-1.ino */

boolean DEBUG = true;

void dp(char *str) {
  if (!DEBUG) return;
  Serial.println(str);
}

void dpXTZ(int x, int y, int z) {
  if (!DEBUG) return;
  Serial.print("X=");
  Serial.print(x);
  Serial.print("  Y=");
  Serial.print(y);
  Serial.print("  Z=");
  Serial.println(z);
}

void setup() {
  Serial.begin(9600);
}

void publish(int x, int y, int z) {
  // analogRead return 0 to 4095.
  // "f,fff,fff,fff" as maximum
  // (format),(x),(y),(z)
  const int str_maxlen = 1 + 1 + 3 + 1 + 3 + 1 + 3 + 1;
  char str[str_maxlen];
  snprintf(str, str_maxlen, "1,%03x,%03x,%03x", x, y, z);
  dp(str);
  Spark.publish("accelerationDisptched", str);
}

void loop() {
  int x = analogRead(A5);
  int y = analogRead(A4);
  int z = analogRead(A3);

  publish(x, y, z);
  dpXTZ(x, y, z);

  delay(300);
}
