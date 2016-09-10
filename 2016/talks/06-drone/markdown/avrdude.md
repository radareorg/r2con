AVRdude saves the day
---------------------

<pre><code class="hljs" data-trim contenteditable>
avrdude -p m8 -C /etc/avrdude.conf -c motor_1 -e
avrdude -p m8 -C /etc/avrdude.conf -c motor_1 -U flash:w:flash.hex:i
</code></pre>

Erasing the entire chip and flashing the flash section back.


AVRdude inside the drone
------------------------

Thought: why not put AVRdude inside the drone for when things go wrong again
with the motor(s)?

ARMv5 does not have "hardfloat" support, so any recent cross-compiler will
result into an "Illegal instruction" error.


Setting up a cross-compiling toolchain is a bit of a PITA.

![docker_xcompile](res/xcompile_jackie_chan.jpg)


Wrap the mess inside docker with dockcross
------------------------------------------

http://radare.today/posts/dockcross/

(and cross-compile radare2 for ARMv5 while we are at it)

Note:
Unfortunately the Atmega8 did not have a functioning serial connection and had
to be reprogrammed via MISO/MOSI pins, so no dice for avrdude reprogramming or
the custom flashing protocol from the drone.
