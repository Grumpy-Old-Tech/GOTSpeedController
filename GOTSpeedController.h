//
//    GOTSpeedController.h
//
//    MIT License
//
//    Copyright (c) Saturday 15th September 2018, Neville Kripp (Grumpy Old Tech)
//
//    Permission is hereby granted, free of charge, to any person obtaining a copy
//    of this software and associated documentation files (the "Software"), to deal
//    in the Software without restriction, including without limitation the rights
//    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//    copies of the Software, and to permit persons to whom the Software is
//    furnished to do so, subject to the following conditions:
//
//    The above copyright notice and this permission notice shall be included in all
//    copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//    SOFTWARE.

#ifndef GOTSPEEDCONTROLLER_H
#define GOTSPEEDCONTROLLER_H

#include "Arduino.h"
#include <HardwareTimer.h>

#define PWM_MAX           2000
#define SENSOR_PER_REV    1             // Used to calculate speed - number of times sensors activate per rev <-------- needs to be chaanged for calibration

// Drive Modes
#define FORWARD_DRIVE     0
#define FORWARD_BRAKE     1
#define NO_DRIVE          2
#define ESTOP             3
#define REVERSE_BRAKE     4
#define REVERSE_DRIVE     5

class GOTSpeedController
{
  public:
    GOTSpeedController();
    
    void setSensorPins(int hall1, int hall2, int hall3);
    void setOutputPins(int aTop, int bTop, int cTop, int aBot, int bBot, int cBot);
    void setup();
    void execute();
    void adjustSpeed(float speedRequest);
    void emergencyStop();
    
    float motorSpeed;
    
  private:
    void calculateMotorSpeed(int commutationPosition);
    void commutationOff();
    void commutationEStop();
    void commutationForwardDrive(int commutationPosition);
    void commutationReverseDrive(int commutationPosition);
    int getHallSensor();
    
    HardwareTimer *pwmtimer2;
    HardwareTimer *pwmtimer3;
    int           speedReference;
    int           operatingMode;
    int           lastCommutationPostion;
    unsigned long lastSpeedTime;
    
    int hall1Pin;
    int hall2Pin;
    int hall3Pin;
    int aTopPin;
    int bTopPin;
    int cTopPin;
    int aBotPin;
    int bBotPin;
    int cBotPin;
};

#endif
