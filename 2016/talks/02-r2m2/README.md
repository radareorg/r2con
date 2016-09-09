Fri 9 - 17:00 - r2m2 using miasm2 as radare2 plugins (guedou)

Reversing a binary using a rare CPU architecture requires to write a lot of
code, such as disassembler, assembler, or block splitting logic. Once
implemented, there is still a need for a graphical interface dedicated to
reverse. Sooner or later, more needs might also arise: symbolic execution,
emulation, jit, debugging, ... 

[miasm2](https://github.com/cea-sec/miasm) is a powerful reverse engineering
framework written in Python. It greatly simplifies the definition of new CPU
architectures, and allows to assemble, disassemble and jit code. 

At the high level, this talk will describe how to write radare2 plugins. More
specifically, it will explain how to call miasm2 from radare2 and provide some
miasm2 examples.

r2m2 is available on [github](https://github.com/guedou/r2m2/).
