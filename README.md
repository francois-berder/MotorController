## MotorController

MotorController is a small board which mostly consists of a PIC16F1825 and a H-bridge to control one motor.
The PIC uses the same protocol than servo so you can plug this board directly to a radio receptor.
Kicad files are available in ```controller.zip```. This archive also contains the ZIP file that I sent to Seeedstudio to make the PCB.

I will soon upload photos of the assembled board.

#### How to build

```bash
$ git submodule init
$ git submodule update
$ make
```

Note that you only need to update the git submodule once.


This creates firmware.hex in ```bin/``` folder.
There are some issues with make dependencies. Hence, if you make any change to
framework, first run ```make clean``` then ```make```. You can also run ```make debug``` if you want to compile assertions.

#### EEPROM layout


If the firmware is built in release mode (default), then a python script modifies the hex file to store information in the EEPROM.

| offset | size | description |
| ------ | ---- | ----------- |
|   0    |   8  | UNIX UTC release timestamp in big endian |
|   8    |   4  | XC8 compiler version in ASCII |
|   12   |  20  | SHA1 hash of last commit of MotorController |

##### Known issues

  - Not connected pins of the PIC16F1825 are left floating, you might want to change the design of the PCB to connect them to ground