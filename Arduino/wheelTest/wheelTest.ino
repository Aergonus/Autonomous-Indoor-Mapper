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

#define PING_LEFT 11
#define PING_FRONT_LEFT 10
#define PING_FRONT_RIGHT 9
#define PING_RIGHT 8

NewPing sonar[SONAR_NUM] = { // Sensor object array.
//NewPing sonar(trigger_pin, echo_pin, max_cm_distance);
  NewPing(PING_RIGHT, PING_RIGHT, MAX_DISTANCE),
  NewPing(PING_FRONT_RIGHT, PING_FRONT_RIGHT, MAX_DISTANCE),
  NewPing(PING_FRONT_LEFT, PING_FRONT_LEFT, MAX_DISTANCE),
  NewPing(PING_LEFT, PING_LEFT, MAX_DISTANCE)
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

int incomingByte = 0;   // for incoming serial data
int state = 0, orient = 0, xcor = 0, ycor = 0, current_distance = 0; 

void loop() {
    /* Ping sequentially ultrasonics */
    for (uint8_t i = 0; i < SONAR_NUM; i++) {
      if (millis() >= pingTimer[i]) {
        pingTimer[i] += PING_INTERVAL * SONAR_NUM;
        if (i == 0 && currentSensor == SONAR_NUM - 1)
          oneSensorCycle(); // Do something with results.
        sonar[currentSensor].timer_stop();
        currentSensor = i;
        cm[currentSensor] = 0;
        sonar[currentSensor].ping_timer(echoCheck);
      }
    }
	
	current_distance = (cm[PING_FRONT_LEFT] == 0 ? cm[PING_FRONT_RIGHT] : cm[PING_FRONT_LEFT])/2
					+ (cm[PING_FRONT_RIGHT] == 0 ? cm[PING_FRONT_LEFT] : cm[PING_FRONT_RIGHT])/2;
	if (Serial.available() > 0) {
		// read the incoming byte:
		incomingByte = Serial.read();
		switch (state) {
			case -1:
				if (incomingByte == 'p'){
					if (orient == 0){
						orient = 1; // Turn right
						state = 2; // Turn right
					} else if (orient == 1){
						orient = -1; // Turn double left
						state = 4; // Turn double left
					} else if (orient == -1){
						orient = 0;
						state = 4;
					}
				}
				break;
			case 0: // Oriented correctly for picture
				wheels.stop();
				delay(5000);
				if (incomingByte == 'r'){
					Serial.print("x:");
					Serial.print(xcor);
					Serial.print(" y:");
					Serial.print(ycor);
					Serial.print(" o:");
					Serial.print(orient);
					Serial.print(" d:");
					Serial.println(current_distance);
				}
				state = -1;
				break;
			case 1:
				// MOVE FORWARD
				  wheels.moveForward();
				  delay(2600);
				  wheels.stop();
				if (current_distance != 0 and current distance < 10){
					state = 5;
					break;
				}
				state = 0;
				break;
			case 2:
				// TURN RIGHT
				wheels.rotateRight();
			    delay(1300); // want: 90 deg to face right
			    wheels.stop();
				state = 0;
				break;
			case 3: 
				// TURN LEFT x2
 			    wheels.rotateLeft();
			    delay(1300); // want: 90 deg to face left
			    wheels.stop();
 			    wheels.rotateLeft();
			    delay(1300); // want: 90 deg to face left
			    wheels.stop();
				state = 0;
				break;
			case 4:
				// TURN RIGHT
				wheels.rotateRight();
			    delay(1300); // want: 90 deg to face front
			    wheels.stop();
				state = 1;
				break;
		}
	}
}
