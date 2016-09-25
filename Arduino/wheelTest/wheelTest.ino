#include <L298N.h>
#include <PololuWheelEncoders.h>
#include <NewPing.h>

int pinLeftMotor1 = 4; // green wires
int pinLeftMotor2 = 5; // green wires
int pinRightMotor1 = 2; // yellow wires
int pinRightMotor2 = 3; // yellow wires

/*
unsigned char encLeftA = 10;
unsigned char encLeftB = 11;
unsigned char encRightA = 8;
unsigned char encRightB = 9;
*/

#define SONAR_NUM     4  // Number of sensors.
#define MAX_DISTANCE 400 // Max distance in cm.
#define PING_INTERVAL 33 // Milliseconds between pings.

unsigned long pingTimer[SONAR_NUM]; // When each pings.
unsigned int cm[SONAR_NUM]; // Store ping distances.
uint8_t currentSensor = 0; // Which sensor is active.

NewPing sonar[SONAR_NUM] = { // Sensor object array.
//NewPing sonar(trigger_pin, echo_pin, max_cm_distance);
  NewPing(8, 8, MAX_DISTANCE),
  NewPing(9, 9, MAX_DISTANCE),
  NewPing(10, 10, MAX_DISTANCE),
  NewPing(11, 11, MAX_DISTANCE)
};

L298N wheels(pinLeftMotor1, pinLeftMotor2, pinRightMotor1, pinRightMotor2, 11, 11, 255);

// PololuWheelEncoders::PololuWheelEncoders();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //PololuWheelEncoders::init(encLeftA, encLeftB, encRightA, encRightB);

  pingTimer[0] = millis() + 75; // First ping start in ms.
  for (uint8_t i = 1; i < SONAR_NUM; i++)
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
}

void loop() {
  
  // put your main code here, to run repeatedly:

  // Take four pics (front, left, back, right)
  // and then orient to face forward again
  wheels.stop();
  delay(5000);
  // TAKE FRONT PIC
  wheels.rotateLeft();
  delay(1300); // want: 90 deg to face left
  wheels.stop();
  delay(5000);
  // TAKE LEFT PIC
  wheels.rotateLeft();
  delay(1300); // want: 90 deg to face back
  wheels.stop();
  delay(5000);
  // TAKE BACK PIC
  wheels.rotateLeft();
  delay(1300); // want: 90 deg to face right
  wheels.stop();
  delay(5000);
  // TAKE RIGHT PIC
  wheels.rotateLeft();
  delay(1300); // want: 90 deg to face front
  wheels.stop();

/*
  wheels.moveForward();
  delay(2600);
  wheels.stop();
  delay(99999999);
*/
  
  /*
  int leftM = PololuWheelEncoders::getCountsM1();
  int rightM = PololuWheelEncoders::getCountsM2();
  */
  
  /*
  Serial.print(leftM);
  Serial.print('\t');
  Serial.print(rightM);
  Serial.print('\n');
  delay(500);
  
  wheels.moveForward();
  delay(100);
  leftM = PololuWheelEncoders::getCountsM1();
  rightM = PololuWheelEncoders::getCountsM2();
  Serial.print(leftM);
  Serial.print('\t');
  Serial.print(rightM);
  Serial.print('\n');
  delay(2000);
  
  wheels.moveBackward();
  delay(100);
  leftM = PololuWheelEncoders::getCountsM1();
  rightM = PololuWheelEncoders::getCountsM2();
  Serial.print(leftM);
  Serial.print('\t');
  Serial.print(rightM);
  Serial.print('\n');
  delay(2000);
  
  wheels.rotateRight();
  delay(100);
  leftM = PololuWheelEncoders::getCountsM1();
  rightM = PololuWheelEncoders::getCountsM2();
  Serial.print(leftM);
  Serial.print('\t');
  Serial.print(rightM);
  Serial.print('\n');
  delay(2000);
  
  wheels.rotateLeft();
  delay(100);
  leftM = PololuWheelEncoders::getCountsM1();
  rightM = PololuWheelEncoders::getCountsM2();
  Serial.print(leftM);
  Serial.print('\t');
  Serial.print(rightM);
  Serial.print('\n');
  delay(2000);
  
  wheels.stop();
  delay(100);
  leftM = PololuWheelEncoders::getCountsM1();
  rightM = PololuWheelEncoders::getCountsM2();
  Serial.print(leftM);
  Serial.print('\t');
  Serial.print(rightM);
  Serial.print('\n');
  delay(2000);

  */
  
/*
  leftM = PololuWheelEncoders::getCountsM1();
  rightM = PololuWheelEncoders::getCountsM2();

  Serial.print(leftM);
  Serial.print('\t');
  Serial.print(rightM);
  Serial.print('\n');

*/


/*
  leftM = PololuWheelEncoders::getCountsM1();
  rightM = PololuWheelEncoders::getCountsM2();

  Serial.print(leftM);
  Serial.print('\t');
  Serial.print(rightM);
  Serial.print('\n');

*/
}
