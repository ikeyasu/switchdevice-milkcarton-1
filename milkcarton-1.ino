/* http://ikeyasu.com */
/* vim: set ts=2  sw=2 sts=2 et si: */
/* astyle --style=google --indent=spaces=2 --pad-oper carton-1.ino */

boolean DEBUG = true;

void setup() {
  Serial.begin(9600);
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

void loop() {
  int x = analogRead(A5);
  int y = analogRead(A4);
  int z = analogRead(A3);

  dpXTZ(x, y, z);

  delay(300);
}
