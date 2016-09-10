Plan
----

1. Identify drone parts and ICs without opening anything (thanks to [Hugo Perquin][hugo_perquin] and [FCCID.io][fccid_io]).
2. Discard [MOSFETs][mosfet_datasheet] [failure][dead_mosfets]. Focus on Atmega8 ICs onboard.
3. [Docker cross-compile support for radare2][cross_compile], avrdude, gdb.
4. [Read flash, lockbits and EEPROM from the motor AVRs][brainblog_ardrone].
5. [(Auto-)annotate the AVR interrupt vector table][avr_interrupts].
6. Try to figure out what does what, why and how.
7. [Report and help][radare_issues] with some [radare2 bugs][radare_PRs] along the way, discussing with @trufae.

[dead_mosfets]: https://community.parrot.com/t5/AR-Drone-2/Cause-of-motor-ESC-failure-confirmed/td-p/33995
[mosfet_datasheet]: http://www.diodes.com/_files/datasheets/ds32152.pdf
[hugo_perquin]: http://blog.perquin.com/blog/ardrone-motor-controller/
[fccid_io]: https://fccid.io/RKXMYKONOS
[cross_compile]: http://radare.today/posts/dockcross/
[brainblog_ardrone]: http://blogs.nopcode.org/brainstorm/2016/06/10/the-right-to-repair-my-drone
[avr_interrupts]: https://github.com/radare/radare2/pull/5247
[radare_PRs]: https://github.com/radare/radare2/pulls?q=is%3Apr+author%3Abrainstorm
[radare_issues]: https://github.com/radare/radare2/issues?q=is%3Aissue+author%3Abrainstorm

Note:
* Whenever I get near a screwdriver, things get opened and all over the place, can I delay this until I really need it? Yes!
* Amazing how detailed the photos from the boards are in FCCID
* Wanted to get radare2 right inside the drone for practical reasons. Could not.
* At some point one gets tired of re-reading the datasheet over and over. Annotate!


FCCID.io schematic
------------------

![fccid_io](res/drone_diagram.png)
