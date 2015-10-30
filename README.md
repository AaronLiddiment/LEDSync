cLEDSync Instructions
========================


Overview
--------
This class aims to make using and synchronising multiple Teensy 3.x boards easy.
It uses the undocumented pull-up function of a digital input to achieve this.
Each board should have their READY_PIN connected to their WAIT_PIN using a 1N4148
signal diode, then all the WAIT_PINS of each board should be commoned.
The class also allows you to specify upto 5 ID pins which can be permanently
wired to ground to enable unique board numbering, thus allowing for upto 32 boards.
