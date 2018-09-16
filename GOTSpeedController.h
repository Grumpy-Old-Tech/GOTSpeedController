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
    
    void setSensorPins(uint8 hall1, uint8 hall2, uint8 hall3);
    void setOutputPins(uint8 aTop, uint8 bTop, uint8 cTop, uint8 aBot, uint8 bBot, uint8 cBot);
    void setSensorPerRev(int number);
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
    int           sensorPerRev;
    
    uint8 hall1Pin;
    uint8 hall2Pin;
    uint8 hall3Pin;
    uint8 aTopPin;
    uint8 bTopPin;
    uint8 cTopPin;
    uint8 aBotPin;
    uint8 bBotPin;
    uint8 cBotPin;
};

#endif
