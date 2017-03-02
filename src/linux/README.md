# PiGPIO library

This directory contains code based on the PiGPIO library [here](http://abyz.co.uk/rpi/pigpio/). 

In addition, it contains the relatively simple protocols for multi-byte transmit (because I was 
unable to get the general block data transfer to work I'm using multiple single byte transfers 
with a wire protocol to define length), and the necessary code to build the Python module which 
is used to actually access everything.
