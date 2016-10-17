#include <NewPing.h> // Ultrasonic Library
#include <DRV8835MotorShield.h> // Motor Drivers

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define SONAR_NUM     4  // Number of sensors.
#define MAX_DISTANCE 400 // Max distance in cm.
#define PING_INTERVAL 40 // Milliseconds between pings.
#define SR_THRESH 10 // Minimum stopping distance for SONAR
#define SR_RR  5  // Right most sonar sensor
#define SR_MR  6  // Middle Right sonar sensor
#define SR_ML 11  // Middle Left sonar sensor
#define SR_LL 12  // Left most sonar sensor

unsigned long pingTimer[SONAR_NUM]; // When each pings.
unsigned int cm[SONAR_NUM]; // Store ping distances.
uint8_t currentSensor = 0; // Which sensor is active.
 
NewPing sonar[SONAR_NUM] = { // Sensor object array.
//NewPing sonar(trigger_pin, echo_pin, max_cm_distance);
  NewPing(SR_RR, SR_RR, MAX_DISTANCE),
  NewPing(SR_MR, SR_MR, MAX_DISTANCE),
  NewPing(SR_ML, SR_ML, MAX_DISTANCE),
  NewPing(SR_LL, SR_LL, MAX_DISTANCE)
};

#define IR_NUM  5
#define IR_THRESH 980 // Minimum threshold to detect IR, empirically tested/trained
#define IR_LL 0 // Left most IR sensor A0
#define IR_ML 1 // Middle Left IR sensor A1
#define IR_MM 2 // Middle Middle IR sensor A2
#define IR_MR 3 // Middle Right IR sensor A3
#define IR_RR 4 // Right most IR sensor A4

int dIR[IR_NUM]; // Storage for IR readings
//int mIR[IR_NUM]; // Minimum IR Threshold Training Storage

DRV8835MotorShield motors;
LiquidCrystal_I2C  lcd(0x27,2,1,0,4,5,6,7); // 0x27 is the I2C bus address for an unmodified backpack

#define LED_PIN 13

bool modeset = false;
int mode = 0;
#define Obj_Avoid 1
#define IR_Track 2

void setup() {
  Serial.begin(9600);
  
  // activate LCD module
  lcd.begin (16,2); // for 16 x 2 LCD module
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home(); // Set cursor to 0,0
  
  pinMode(LED_PIN, OUTPUT);
  
  // uncomment one or both of the following lines if your motors' directions need to be flipped
  //motors.flipM1(true);
  motors.flipM2(true);
  
  pingTimer[0] = millis() + 75; // First ping start in ms.
  for (uint8_t i = 1; i < SONAR_NUM; i++)
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
  /*
  for (uint8_t i = 0; i < IR_NUM; ++i) {
    mIR[i] = 1025;
  }
  */
  lcd.print("Robot Online");
}
 
void loop() {
  modeset = false;
  //Serial.print("m");
  //Serial.print(mode);
  //Serial.print(" ");
  // Check Ultrasonics
  for (uint8_t i = 0; i < SONAR_NUM; ++i) {
    if (millis() >= pingTimer[i]) {
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;
      if (i == 0 && currentSensor == SONAR_NUM - 1)
        sonarCheck(); // Check results
      sonar[currentSensor].timer_stop();
      currentSensor = i;
      cm[currentSensor] = 0;
      sonar[currentSensor].ping_timer(echoCheck);
    }
  }
  debugSONAR();
  
  // Check IRs
  for (uint8_t i = 0; i < IR_NUM; ++i) {
    dIR[i] = analogRead(i);
    // mIR[i] = (mIR[i] > dIR[i]) ? dIR[i] : mIR[i]; // A priori IR threshold tuning
    if (dIR[i] < IR_THRESH){
      mode = IR_Track;
      modeset = true;
      lcd.setCursor (0,1);
      lcd.print("IR ");
      lcd.print(i);
      lcd.print("        "); // Clear Display
    }
  }
  //debugIR();
  // Check if resume normal operation
  mode = modeset ? mode : 0;
  // Execute control
  switch(mode) {
    case 0:
      // Move forward, calm walking
      walkSTRAIGHT();
      //TODO: Add animation
      break;
    case Obj_Avoid:
      // 
      //Serial.print("Obs!");
      avoidObs();
      break;
    case IR_Track:
      // 
      //Serial.print("IR!");
      trackIR();
      break;
    default:
      lcd.setCursor (0,0);
      lcd.print("ERROR!");
      lcd.setCursor (0,1);
      lcd.print("ERROR!");
      break;
  }

}
 
void echoCheck() { // If ping echo, set distance to array.
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}
void sonarCheck() {
  for (uint8_t i = 0; i < SONAR_NUM; ++i) {
    if (mode == 0 && cm[i] != 0 && cm[i] <= SR_THRESH){
      lcd.setCursor (0,1);
      lcd.print("Avoiding ");
      lcd.print(i);
      mode = Obj_Avoid;
      modeset = true;
    }
  }
}
void avoidObs() {
  int localmin = SR_THRESH;
  int minsr;
  for (uint8_t i = 0; i < SONAR_NUM; ++i) {
    if(cm[i] != 0 && localmin > cm[i]){
      localmin = cm[i];
      minsr = i;
    }
  }
  lcd.setCursor (0,1);
  lcd.print("Tracking SR ");
  lcd.print(minsr);
  
  if (minsr > SONAR_NUM/2){
    turnLEFT();
  } else {
    turnRIGHT();
  }
  int delay_amt = abs(minsr - SONAR_NUM/2)*500;
  delay(delay_amt);
  walkSTRAIGHT();

  // Reset SONARs
  for (uint8_t i = 0; i < SONAR_NUM; ++i) {
    if (millis() >= pingTimer[i]) {
      pingTimer[i] += delay_amt;
    }
  }
}

void trackIR() {
  int localmin = IR_THRESH;
  int minir = -1;
  for (uint8_t i = 0; i < IR_NUM; ++i) {
    if(dIR[i] != 0 && localmin > dIR[i]){
      localmin = dIR[i];
      minir = i;
    }
  }

  if (minir == IR_MM){
    stopBRAKE();
  } else if (dIR[IR_MR] < IR_THRESH || dIR[IR_ML] < IR_THRESH) {
    if (dIR[IR_MR] > dIR[IR_ML]) {
      turnRIGHT();
    } else if (dIR[IR_ML] > dIR[IR_MR]) {
      turnLEFT();
    }
  } else if (minir == IR_RR) {
    turnRIGHT();
  } else if (minir == IR_LL) {
    turnLEFT();
  } else if (minir == -1) {
      lcd.setCursor (0,0);
      lcd.print("ERROR!");
      lcd.setCursor (0,1);
      lcd.print("ERROR!");
  }
}

void turnLEFT() {
  motors.setM1Speed(-120);
  motors.setM2Speed(100);
}
void turnRIGHT() {
  motors.setM1Speed(120);
  motors.setM2Speed(-100);
}
void walkSTRAIGHT() {
  motors.setM1Speed(120);
  motors.setM2Speed(100);
}
void stopBRAKE() {
  motors.setM1Speed(0);
  motors.setM2Speed(0);
}
void debugSONAR() { // Print out SONAR readings
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    Serial.print(i);
    Serial.print("=");
    Serial.print(cm[i]);
    Serial.print("cm ");
  }
  Serial.println();
}
void debugIR() { // Print out IR readings
  for (uint8_t i = 0; i < IR_NUM; i++) {
    Serial.print(i);
    Serial.print("=");
    Serial.print(dIR[i]);
    Serial.print(" ");
  }
  Serial.println();
}

