** but what is wrong? **

With the MOSFETs tested and healthy, there seems to be a software-only issue preventing
to get one of the motors up and running:

<pre><code class="hljs" data-trim contenteditable>
$ telnet 192.168.1.1
# bin/program.elf
(...)
BLC motor 3 soft version 1.43, hard version 3.0, supplier 1.1, lot number 11/10, FVT1 17/11/10
BLC call for motor 4
BLC motor 4 soft version 1.43, hard version 3.0, supplier 1.1, lot number 11/10, FVT1 17/11/10
BLC motor 1 dead
BLC reflash required, perform off/on cycle
(...)
/home/aferran/.ardrone/mykonos/version/Mykonos/Soft/Build/../
../Soft/Toy//Os/elinux/Control/motors.c:1593. 
Reason is Motors have not been initialized correctly !!!
</code></pre>


MOSFETs
=======
![mosfets](res/mosfets.png)


Atmel Atmega8A
==============
![atmega8a](res/atmega8a_pinout.png)


Drone BLC
=========
![blc_drone](res/drone_motor_pinout.jpg)
