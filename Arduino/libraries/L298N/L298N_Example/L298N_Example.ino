#include <L298N.h>

L298N wheels(24, 25, 22, 23, 12, 13, 255);

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  wheels.moveForward();
  delay(2000);
  wheels.moveBackward();
  delay(2000);
  wheels.rotateRight();
  delay(2000);
  wheels.rotateLeft();
  delay(2000);
  wheels.stop();
  delay(2000);
}
