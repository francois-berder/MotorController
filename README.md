## MotorController [![Build Status](https://travis-ci.org/francois-berder/MotorController.svg?branch=master)](https://travis-ci.org/francois-berder/MotorController)

MotorController is a small board which mostly consists of a PIC16F1825 and a H-bridge to control one motor.
The PIC uses the same [protocol than a servo](https://en.wikipedia.org/wiki/Servo_control) so you can plug this board directly to a radio receptor.
Kicad files are available in ```controller.zip```. This archive also contains the ZIP file that I sent to Seeedstudio to make the PCB.

![MotorController image](motorcontroller.png "MotorController rev 1.1")

### How to build

```bash
$ git submodule init
$ git submodule update
$ make
```
You can also run ```make debug``` if you want to compile assertions.
Note that you only need to update the git submodule once.


This creates firmware.hex in ```bin/``` folder.

### Firmware description

When the PIC starts running, it computes the neutral position. This takes a few seconds as it computes the average of 72 to 96 entries. During these moments, do **not** touch your radio. The LED will be blinking fast during this operation. Once this is finished, the PIC turns on the LED.

The PIC will reset if:
   - no data was received for one second (watchdog resets PIC)
   - too many invalid data have been received

Signal from the radio is converted as a number which, if valid, is in range 450..1050. Any number out of this range is considered invalid. Neutral position is expected to be around 750. Hence, there are approximately 300 different positions in each direction. The PWM output precision is 8 bits. Any number greater than 255 is capped. Also, the PIC will stop the motor if numbers are in range neutral-min_power..neutral+min_power.
The firmware takes about 1 second from full stop to maximum speed. You can adjust this behavior by changing values in ```config.h```.

All of these numbers are dependent on the clock speed of the PIC. The firmware configures the PIC to use its internal oscillator at 2MHz. This is the minimum clock speed to ensure that the PWM output precision is 8 bits. If you want to save more power (current firmware uses 700μA-1mA) at the expense of motor control precision, you can lower the clock speed. Also, disabling the LED would also help reduce the power consumption.

### EEPROM layout


If the firmware is built in release mode (default), then a python script modifies the hex file to store information in the EEPROM.

| offset | size | description |
| ------ | ---- | ----------- |
|   0    |   8  | UNIX UTC release timestamp in big endian |
|   8    |   4  | XC8 compiler version in ASCII e.g. "1.42" |
|   12   |  20  | SHA1 hash of last commit of MotorController |

### Known issues

  - Not connected pins of the PIC16F1825 are left floating, you might want to change the design of the PCB to connect them to ground
  - Lack of resistor to limit current between pins RC1, RC3 and 4N35 optocouplers.
  - There are some issues with make dependencies. Hence, if you make any change to framework, first run ```make clean``` then ```make```.
