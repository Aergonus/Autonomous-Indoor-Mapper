#include <L298N.h>
#include <PololuWheelEncoders.h>

int pinLeft1 = 4; // yellow wires
int pinLeft2 = 5; // yellow wires
int pinRight1 = 2; // green wires
int pinRight2 = 3; // green wires

L298N wheels(pinLeft1, pinLeft2, pinRight1, pinRight2, 11, 11, 255);

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
