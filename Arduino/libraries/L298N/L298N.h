/*
 *  L298N.h - Library for controlling two motors with the L298N Dual H-Bridge.
 *  Created by Jeffrey Tam, November 15, 2015
 */
#ifndef L298N_h
#define L298N_h

#include <Arduino.h>

class L298N
{
  public:
    L298N(int left1, int left2, int right1, int right2, int leftPwmPin, int rightPwmPin, int pwmDuty);
    void moveForward();
    void moveBackward();
    void rotateRight();
    void rotateLeft();
    void stop();

  private:
    class Motor
    {
      public:
        Motor(int p1, int p2);
        void forward();
        void backward();
        void stop();
      private:
        int pin1;
        int pin2;
    };

    Motor *leftMotor;
    Motor *rightMotor;
};

#endif

