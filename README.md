# 16x2-KY40-Interface-AVR-MEGA
Saving this so I can probably break it down into a class. 
========================================================
2019 All materials Engineered and written by Kristoper
Bickham


Basically it's a one button and one rotary encoder (with press)
control + display to form an interface.

Right now this is populated with test for a menu that controls
a 2DOF robotic arm. I'm going to get it working, then train it 
to touch on an X/Y plane. Hopefully integrate a neural network 
and vision system. I have a thrown out iLife robot vaccum I'm using
as the main system housing. This will mount on top. It is the robot's arm.

The menu have "levels" of selection, right now 2 or 3. Select brings you up
a layer until the top, then it will 
drop back to the previous.


Feel free to take parts of this, but put my name in there if you do.
-----------------------------------------------------
=============================================================
The images in here are of the system, so you can see what it looks like
I'll update the .stl's soon, it's pretty simple. One this is done, 
you should be able to print and assemble the arm for under 40 bucks. I'm 
powering the arm with a board I made out of a TI LM2576 (the 3 amp)
switcing reg at 5v. Main board is currentl an Arduino MEGA2560, but I have an
old board I made from an ATMEL1280 I'm going to put in there once its done.

Power:
1 LM2576 5V 3A Reg (toroidal and heatsink, big output capacitor)


1 AC to DC adapter from a router (to v reg)

Testing Mount:
1 Old Flatscreen TV base that mounts to  short 4 sided cube frame.


Control System:
1 Long ass ribbon (old IDE cable from a PC I cut in the middle)     
3 3D printed pieces forming housing. The purpose of this is:
             Cap -> 1 - 16x2 Char. Display -> cap   (Interface Out)
             Base -> 1 - Arduino MEGA2560             (MCU)
             Top -> 1 - KY-40 Rotary Encoder         (Input)
                     1 Circular Press Buttom         (Input)

            
Arms:
2 ULN2003 Drivers
2 3D Printed Arm
2 28-BYJ Stepper Motors 

1 Billion - Jumper Wires (Sorry. I started a library...to cut wires down to 3 using shift regulators. I have to
                     change the way I'm driving the motors I believe. MOSFET board with some shift regs coming)

