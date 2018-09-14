#include <GOTSpeedController.h>

//-------------DEFINE PINS----------------

#define HALL_1_SENSOR     PB3           // Motor Hall Sensor input pins
#define HALL_2_SENSOR     PB4
#define HALL_3_SENSOR     PB5

#define PHASE_A_TOP       PA0           // Top Drive output pins
#define PHASE_B_TOP       PA1
#define PHASE_C_TOP       PA2
#define PHASE_A_BOT       PA3           // Bottom Drive output pins
#define PHASE_B_BOT       PA6
#define PHASE_C_BOT       PA7

//-------------Speed Controller---------------- 
GOTSpeedController speedController;


//-------------Variables for test----------------
int   speedValue;

void setup() {

  Serial.begin(9600);

  speedController.setSensorPins(HALL_1_SENSOR, HALL_2_SENSOR, HALL_3_SENSOR);
  speedController.setOutputPins(PHASE_A_TOP, PHASE_B_TOP, PHASE_C_TOP, PHASE_A_BOT, PHASE_B_BOT, PHASE_C_BOT);
  speedController.setup();
  speedController.adjustSpeed(0); // Set speed to stopped
  
  speedValue = 0;

}

void loop() {

  byte character;
  if (Serial.available()) {
    character = Serial.read();
    if (character == 'q') {
      speedValue += 100;
    }
    if (character == 'a') {
      speedValue -= 100;
    }
  }
  speedValue = constrain(speedValue,-2000, 2000);

  Serial.print("Speed:");
  Serial.println(speedValue);

  speedController.execute();
}
