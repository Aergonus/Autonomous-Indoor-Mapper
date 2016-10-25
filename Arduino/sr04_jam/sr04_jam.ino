#define SR_NUM 4  // Number of sonar sensors
#define SR_RR  5  // Right most sonar sensor
#define SR_MR  6  // Middle Right sonar sensor
#define SR_ML 11  // Middle Left sonar sensor
#define SR_LL 12  // Left most sonar sensor

int srs[SR_NUM] = {
  5,6,11,12
}
int re
void setup() {
   //pinMode(trigPin, OUTPUT);
   for (uint8_t i = 0; i < SONAR_NUM; ++i) {
     pinMode(srs[i], OUTPUT);
   }
}

void loop() {
  /*
  digitalWrite(trigPin, LOW);
  delayMicroseconds(4);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  */
  for (uint8_t i = 0; i < SONAR_NUM; ++i) {
     digitalWrite(srs[i], LOW);
  }
  delayMicroseconds(4);
  
  for (uint8_t i = 0; i < SONAR_NUM; ++i) {
     digitalWrite(srs[i], HIGH);
  }
  delayMicroseconds(10);

  for (uint8_t i = 0; i < SONAR_NUM; ++i) {
     digitalWrite(srs[i], LOW);
  }
  delayMicroseconds(200);
}
