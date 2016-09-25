#include "L298N.h"

L298N::L298N(int left1, int left2, int right1, int right2, int leftPwmPin, int rightPwmPin, int pwmDuty)
{
  leftMotor = new Motor(left1, left2);
  rightMotor = new Motor(right1, right2);

  analogWrite(leftPwmPin, pwmDuty);
  analogWrite(rightPwmPin, pwmDuty);
}

void L298N::moveForward()
{
  leftMotor->forward();
  rightMotor->forward();  
}

void L298N::moveBackward()
{
  leftMotor->backward();
  rightMotor->backward();
}

void L298N::rotateRight()
{
  leftMotor->forward();
  rightMotor->backward();
}

void L298N::rotateLeft()
{
  leftMotor->backward();
  rightMotor->forward();
}

void L298N::stop()
{
  leftMotor->stop();
  rightMotor->stop();
}

L298N::Motor::Motor(int p1, int p2)
{
   pin1 = p1;
   pin2 = p2;

   pinMode(pin1, OUTPUT);
   pinMode(pin2, OUTPUT);
}

void L298N::Motor::forward()
{
   digitalWrite(pin1, LOW);
   digitalWrite(pin2, HIGH);
}

void L298N::Motor::backward()
{
   digitalWrite(pin1, HIGH);
   digitalWrite(pin2, LOW);
}

void L298N::Motor::stop()
{
   digitalWrite(pin1, LOW);
   digitalWrite(pin2, LOW);
}
