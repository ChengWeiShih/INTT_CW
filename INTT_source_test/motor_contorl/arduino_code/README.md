******Step motor controll - Arduino******

Cable connections:

PUL+ & DIR+  connect to 5V
PUL-                 connect to pin8
DIR-                 connect to pin7

GND & VCC    connect to power supply
A+, A-, B+, B-  connect to the step motor
==============================

Three features for the code:
- standard
- single
- free
To use any one of the features use the type the features' name in serial monitor 
e.g. type "single" in the serial monitor to use the single function

"standard" will run from U1 to U13 and U14 to U26. Using this feature, you need to 
enter the measureing time. It will be stop by every cell with the measuring time you enter.

"single" can run to any of cell you want. e.g. enter "12" the motor will move to U12.
when using this features, be careful to move the motor the U1 first.

"free" freely type in any steps you wnat the motor to move. e.g. "30" for move 30 steps
forward. "-50" move 50 steps backward.



How to use the code:
1. upload the code to arduino
2. open up the the serial monitor
3. set up three parameters:
    - steps_gap_A (cells distance of type A sensor)
    - steps_gap_B (cells distance of type B sensor)
    - AB_gap (distance between sensor A & B)
    (see below "How to set the parameters?")
4. after setup the parameters, upload the code again
5. drive the motor to U1 positions
6. start meausrement
    


How to set the parameters?
  Use free function to get the parameters. 
  Once you open up the free function, you can enter how many steps you want motor to move.
    e.g. "50" move forward 50 steps, "-100" move backward 100 steps
  Using this free function you can then find out the three parameters

