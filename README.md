# GOTSpeedController
This library implements a brushless motor speed controller. The Controller is designed to work with motors fitted with Hall Effect sensors. This Class is currently under test.

Please note: This library is deisgned to work with the STM32 series of microcontrollers and currently cannot be used with a normal Arduino processor.

## EXECUTION
This Class is designed to be called on everytime the loop processes. It is also reliant of the loop being processed at a sufficently high frequency to allow propoer motor control.

## INPUT / OUTPUT PINS
The Hall Effect sensor pins can be configured using the setSensorPins() method.
The Drive pins can be configured using the setOutputPins() method.

After setting the input and output pins the setup()  method should be called.

## EXECUTE
This Class is designed to be called on everytime the loop processes. It is also reliant of the loop being processed at a sufficently high frequency to allow propoer motor control. The execute() method processes the drive outputs dependant on the hall sensor reading and the commands.

## SPEED CONTROL
The motor speed is set using the adjustSpeed() method, a drive signal of 0 is used to set the motor to the stationary. Negative reading will cause the motor to run in reverse, positive in the forward direction. An Emergency Stop method emergencyStop() is also provided.

## SPEED MEASUREMENT
The motor speed can be read by accessing the motorSpeed property.
