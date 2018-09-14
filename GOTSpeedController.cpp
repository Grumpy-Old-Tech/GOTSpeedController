//
//    GOTSpeedController.cpp
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

#include "Arduino.h"
#include "GOTSpeedController.h"

#if defined(BOARD_generic_stm32f103c)
#include <HardwareTimer.h>
#endif

GOTSpeedController::GOTSpeedController() {
    
    pwmtimer2 = new HardwareTimer(2);
    pwmtimer3 = new HardwareTimer(3);
    speedReference = 0;
    operatingMode = NO_DRIVE;
}

void GOTSpeedController::setSensorPins(int hall1, int hall2, int hall3) {
    
    hall1Pin = hall1;
    hall2Pin = hall2;
    hall3Pin = hall3;
}

void GOTSpeedController::setOutputPins(int aTop, int bTop, int cTop, int aBot, int bBot, int cBot) {
    
    aTopPin = aTop;
    bTopPin = bTop;
    cTopPin = cTop;
    aBotPin = aBot;
    bBotPin = bBot;
    cBotPin = cBot;
}

void GOTSpeedController::setup() {
    
    // Setup Motor Hall Sensor inputs
    pinMode(hall1Pin,INPUT);
    pinMode(hall2Pin,INPUT);
    pinMode(hall3Pin,INPUT);
    
    // Setup Drive outputs
    pinMode(aTopPin,PWM);
    pinMode(bTopPin,PWM);
    pinMode(cTopPin,PWM);
    pinMode(aBotPin,PWM);
    pinMode(bBotPin,PWM);
    pinMode(cBotPin,PWM);
    
    // Drive outputs are PWM however we need to run the PWM frequency higher than the standard.
    // to do this we change the prescaler of the timer used for the PWM outputs. In the case
    // of these pins its timer 2 and timer 3 we need to change
    
    pwmtimer2->pause();
    pwmtimer2->setPrescaleFactor(1);
    pwmtimer2->setOverflow(PWM_MAX);
    pwmtimer2->refresh();
    pwmtimer2->resume();
    
    pwmtimer3->pause();
    pwmtimer3->setPrescaleFactor(1);
    pwmtimer3->setOverflow(PWM_MAX);
    pwmtimer3->refresh();
    pwmtimer3->resume();
    
    commutationOff();
}

void GOTSpeedController::execute() {
    
    int commutationPostion = getHallSensor();
    
    calculateMotorSpeed(commutationPostion);
    
    switch (operatingMode)
    {
        case ESTOP:
            commutationEStop();
            break;
        case FORWARD_DRIVE:
            commutationForwardDrive(commutationPostion);
            break;
        case NO_DRIVE:
            commutationOff();
            break;
        case REVERSE_DRIVE:
            commutationReverseDrive(commutationPostion);
            break;
        default:
            break;
    }
}

void GOTSpeedController::adjustSpeed(float speedRequest) {
    
  if (speedRequest > 0) {
        
    operatingMode = FORWARD_DRIVE;
  }
  else if (speedRequest < 0) {
        
    operatingMode = REVERSE_DRIVE;
  }
  else {
        
    operatingMode = NO_DRIVE;
  }
    
  speedReference = (int)abs(constrain(speedRequest,-PWM_MAX, PWM_MAX));
}

void GOTSpeedController::emergencyStop() {
    
    speedReference = 0;
    operatingMode = ESTOP;
}

void GOTSpeedController::calculateMotorSpeed(int commutationPosition) {
    
    if (lastCommutationPostion != 1 && commutationPosition == 1) {
        
        unsigned long currentSpeedTime = micros();
        float revolutionDuration = (lastSpeedTime - currentSpeedTime) / 1000000.0;
        float revolutionFrequency = 1 / (revolutionDuration * SENSOR_PER_REV);
        motorSpeed = revolutionFrequency / 60.0;
        lastSpeedTime = currentSpeedTime;
    }
    lastCommutationPostion = commutationPosition;
}

void GOTSpeedController::commutationOff() {
    
    pwmWrite(aTopPin,0);
    pwmWrite(bTopPin,0);
    pwmWrite(cTopPin,0);
    pwmWrite(aBotPin,0);
    pwmWrite(bBotPin,0);
    pwmWrite(cBotPin,0);
}

void GOTSpeedController::commutationEStop() {
    
    pwmWrite(aTopPin,0);
    pwmWrite(bTopPin,0);
    pwmWrite(cTopPin,0);
    pwmWrite(aBotPin,PWM_MAX);
    pwmWrite(bBotPin,PWM_MAX);
    pwmWrite(cBotPin,PWM_MAX);
}

// 0 - 2000 speedRequest
void GOTSpeedController::commutationForwardDrive(int commutationPosition) {
    
    switch (commutationPosition)
    {
        case 1:
            pwmWrite(aTopPin,0);
            pwmWrite(bTopPin,PWM_MAX);
            pwmWrite(cTopPin,0);
            pwmWrite(aBotPin,speedReference);
            pwmWrite(bBotPin,0);
            pwmWrite(cBotPin,0);
            break;
        case 5:
            pwmWrite(aTopPin,0);
            pwmWrite(bTopPin,PWM_MAX);
            pwmWrite(cTopPin,0);
            pwmWrite(aBotPin,0);
            pwmWrite(bBotPin,0);
            pwmWrite(cBotPin,speedReference);
            break;
        case 4:
            pwmWrite(aTopPin,PWM_MAX);
            pwmWrite(bTopPin,0);
            pwmWrite(cTopPin,0);
            pwmWrite(aBotPin,0);
            pwmWrite(bBotPin,0);
            pwmWrite(cBotPin,speedReference);
            break;
        case 6:
            pwmWrite(aTopPin,PWM_MAX);
            pwmWrite(bTopPin,0);
            pwmWrite(cTopPin,0);
            pwmWrite(aBotPin,0);
            pwmWrite(bBotPin,speedReference);
            pwmWrite(cBotPin,0);
            break;
        case 2:
            pwmWrite(aTopPin,0);
            pwmWrite(bTopPin,0);
            pwmWrite(cTopPin,PWM_MAX);
            pwmWrite(aBotPin,0);
            pwmWrite(bBotPin,speedReference);
            pwmWrite(cBotPin,0);
            break;
        case 3:
            pwmWrite(aTopPin,0);
            pwmWrite(bTopPin,0);
            pwmWrite(cTopPin,PWM_MAX);
            pwmWrite(aBotPin,speedReference);
            pwmWrite(bBotPin,0);
            pwmWrite(cBotPin,0);
            break;
        default:
            pwmWrite(aTopPin,0);
            pwmWrite(bTopPin,0);
            pwmWrite(cTopPin,0);
            pwmWrite(aBotPin,0);
            pwmWrite(bBotPin,0);
            pwmWrite(cBotPin,0);
            break;
    }
}

void GOTSpeedController::commutationReverseDrive(int commutationPosition) {
    
    switch (commutationPosition)
    {
        case 1:
            pwmWrite(aTopPin,PWM_MAX);
            pwmWrite(bTopPin,0);
            pwmWrite(cTopPin,0);
            pwmWrite(aBotPin,0);
            pwmWrite(bBotPin,speedReference);
            pwmWrite(cBotPin,0);
            break;
        case 5:
            pwmWrite(aTopPin,PWM_MAX);
            pwmWrite(bTopPin,0);
            pwmWrite(cTopPin,0);
            pwmWrite(aBotPin,0);
            pwmWrite(bBotPin,0);
            pwmWrite(cBotPin,speedReference);
            break;
        case 4:
            pwmWrite(aTopPin,0);
            pwmWrite(bTopPin,PWM_MAX);
            pwmWrite(cTopPin,0);
            pwmWrite(aBotPin,0);
            pwmWrite(bBotPin,0);
            pwmWrite(cBotPin,speedReference);
            break;
        case 6:
            pwmWrite(aTopPin,0);
            pwmWrite(bTopPin,PWM_MAX);
            pwmWrite(cTopPin,0);
            pwmWrite(aBotPin,speedReference);
            pwmWrite(bBotPin,0);
            pwmWrite(cBotPin,0);
            break;
        case 2:
            pwmWrite(aTopPin,0);
            pwmWrite(bTopPin,0);
            pwmWrite(cTopPin,PWM_MAX);
            pwmWrite(aBotPin,speedReference);
            pwmWrite(bBotPin,0);
            pwmWrite(cBotPin,0);
            break;
        case 3:
            pwmWrite(aTopPin,0);
            pwmWrite(bTopPin,0);
            pwmWrite(cTopPin,PWM_MAX);
            pwmWrite(aBotPin,0);
            pwmWrite(bBotPin,speedReference);
            pwmWrite(cBotPin,0);
            break;
        default:
            pwmWrite(aTopPin,0);
            pwmWrite(bTopPin,0);
            pwmWrite(cTopPin,0);
            pwmWrite(aBotPin,0);
            pwmWrite(bBotPin,0);
            pwmWrite(cBotPin,0);
            break;
    }
}

int GOTSpeedController::getHallSensor() {
    
    int hall1Value = digitalRead(hall1Pin);
    int hall2Value = digitalRead(hall2Pin);
    int hall3Value = digitalRead(hall3Pin);
    
    int hallValue = hall1Value + 2 * hall2Value + 4 * hall3Value;
    
    if (hallValue > 6)
        hallValue = 0;
    
    return hallValue;
}
