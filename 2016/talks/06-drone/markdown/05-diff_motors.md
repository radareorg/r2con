diffing hardware
----------------

How does the "dead" motor compare to the "healthy" one?

<img src=res/raspberry_pi_motor_gpio.jpg height=512 width=382></img>

Note:
Diff it with a Raspberry pi!

avrdude dumps
==============

<pre><code class="hljs" data-trim contenteditable>
diff -u motor1/lock.hex motor2/lock.hex
--- motor1/lock.hex     2016-06-06 10:00:38.532028704 +0000
+++ motor2/lock.hex     2016-06-06 09:09:24.738241220 +0000
@@ -1,2 +1,2 @@
-:0100000003FC
+:010000002FD0
:00000001FF
</code></pre>

What are those lockbits? Here is some good info:
http://www.embedds.com/all-you-need-to-know-about-avr-fuses/

Here is a fuse bits calculator:
http://www.engbedded.com/fusecalc

...and of course, the official Atmega8 [datasheet](http://www.atmel.com/Images/Atmel-2486-8-bit-AVR-microcontroller-ATmega8_L_datasheet.pdf)!


Fuse bits (03 FC): "dead" motor
-------------------------------

![lock_bits_dead](res/03fc_fusebits.png)


Fuse bits (2F D0): good motor
-----------------------------

![lock_bits_healthy](res/2fd0_fusebits.png)


EEPROM
------

<pre><code class="hljs" data-trim contenteditable>
$ diff -u motor1/eeprom.hex motor2/eeprom.hex
--- motor2/eeprom.hex     2016-06-06 08:47:26.815120557 +0000
+++ motor1/eeprom.hex     2016-06-06 09:35:17.664976258 +0000
@@ -1,4 +1,4 @@
-:20000000AC8A0001018A0A110BFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0F
+:20000000FF030001010B0A120B0AFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFB6
 :20002000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE0
 :20004000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0
 :20006000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA0
</code></pre>

No idea what this is yet... did I mention we badly need ESIL for AVR to make sense
of memory mappings?
