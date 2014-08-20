/* http://ikeyasu.com */
/* vim: set ts=2  sw=2 sts=2 et si: */
/* astyle --style=google --indent=spaces=2 --pad-oper carton-1.ino */

#include <math.h> 

const float VCC = 5.04;
const float VD = 3.335;
const float PI = 3.14159;

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
  Serial.print(z);
}

void dpXTZV(float xv, float yv, float zv) {
  if (!DEBUG) return;
  Serial.print(" Xv=");
  Serial.print(xv);
  Serial.print("  Yv=");
  Serial.print(yv);
  Serial.print("  Zv=");
  Serial.print(zv);
}

void dpXTZG(float xg, float yg, float zg) {
  if (!DEBUG) return;
  Serial.print(" Xg=");
  Serial.print(xg);
  Serial.print("  Yg=");
  Serial.print(yg);
  Serial.print("  Zg=");
  Serial.print(zg);
}

void dpATXTZG(float atzxg, float atyzg, float atxyg) {
  if (!DEBUG) return;
  Serial.print(" atzxg=");
  Serial.print(atzxg);
  Serial.print("  atyzg=");
  Serial.print(atyzg);
  Serial.print("  atxyg=");
  Serial.println(atxyg);
}

void loop() {
  float x = analogRead(0) * VCC / 1024;
  float y = analogRead(1) * VCC / 1024;
  float z = analogRead(2) * VCC / 1024;

  dpXTZ(x, y, z);

  float xv = x - VD / 2.0 + 0.01;
  float yv = y - VD / 2.0 + 0.025;
  float zv = z - VD / 2.0 - 0.03;

  dpXTZV(xv, yv, zv);

  float xg = xv / ((103.43 * VD - 11.25) / 1000);
  float yg = yv / ((103.43 * VD - 11.25) / 1000);
  float zg = zv / ((103.43 * VD - 11.25) / 1000);

  dpXTZG(xg, yg, zg);

  float atzxg = atan2(zg, xg) * 360 / 2.0 / PI;
  float atyzg = atan2(yg, zg) * 360 / 2.0 / PI;
  float atxyg = atan2(xg, yg) * 360 / 2.0 / PI;

  dpATXTZG(atzxg, atyzg, atxyg);

  delay(300);
}

