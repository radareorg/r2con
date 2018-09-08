# r2con 2018 egypt challenge
This was actually one of the easy challenges but at least I managed to write some semi-proper write-up on time. I hope that it can help some people starting to use radare2 and do some _marketing_ for r2dec, which I think (just as r2) is amazing.

## Description

```
Are you a master in lockpicking?

Try to open this lock bruteforcing the combination!

Put the flag inside r2con{flag} and check if you got the right flag.
```

Target binary: forceme_9f132ba529bfd562380d448f4904c76f

## Let's get started
If we run the binary we will see that it requires an input:

```
$ ./forceme_9f132ba529bfd562380d448f4904c76f

****** As a Lockpicking master, Open the Lock! *******

   .----.
  / / .--.\
 / /      \
| |       | |
| |.----. | |
/// .::::. \ \
||| ::/  ::  |
||; ::\__/:: |
\ '::::' /  /
 `=':-..-'`
Usage: forceme [code]

```

First of all we will open the target binary with radare2 and take a look at the main's function disassembly:

```
[0x00400713 20% 185 ./forceme_9f132ba529bfd562380d448f4904c76f]> pd $r @ main                            
/ (fcn) main 543                                                                                         
|   main (int argc, char **argv, char **envp);                                                           
|           ; var char **s @ rbp-0x20                                                                    
|           ; var unsigned int local_14h @ rbp-0x14                                                      
|           ; var unsigned int local_4h @ rbp-0x4                                                        
|           ; arg unsigned int argc @ rdi                                                                
|           ; arg char **argv @ rsi                                                                      
|           ; DATA XREF from entry0 (0x4004dd)                                                           
|           0x00400713      55             push rbp                                                      
|           0x00400714      4889e5         mov rbp, rsp                                                  
|           0x00400717      4883ec20       sub rsp, 0x20                                                 
|           0x0040071b      897dec         mov dword [local_14h], edi       ; argc                       
|           0x0040071e      488975e0       mov qword [s], rsi          ; argv                            
|           0x00400722      837dec02       cmp dword [local_14h], 2       ; [0x2:4]=-1 ; 2               
|       ,=< 0x00400726      7428           je 0x400750                 ;[1]                              
|       |   0x00400728      bfb80a4000     mov edi, str.As_a_Lockpicking_master__Open_the_Lock       ; 0x
|       |   0x0040072d      e84efdffff     call sym.imp.puts           ;[2]   ; int puts(const char *s)  
|       |   0x00400732      b800000000     mov eax, 0                                                    
|       |   0x00400737      e8edfeffff     call sym.banner             ;[3]                              
|       |   0x0040073c      bfef0a4000     mov edi, str.Usage:_forceme__code       ; 0x400aef ; "Usage: f
|       |   0x00400741      e83afdffff     call sym.imp.puts           ;[2]   ; int puts(const char *s)  
|       |   0x00400746      b801000000     mov eax, 1                                                    
|      ,==< 0x0040074b      e9e0010000     jmp 0x400930                ;[4]                              
```

Now we decompile it using r2dec to get a quick overview of what the code is doing:

```
$ r2 -Ad forceme_9f132ba529bfd562380d448f4904c76f
...

:> pdd @ sym.main
#include <stdio.h>
int64_t main () {
    *(local_14h) = edi;
    *(local_20h) = rsi;
    if (*(local_14h) != 2) {
        puts ("****** As a Lockpicking master, Open the Lock! *******");
        eax = 0;
        banner ();
        puts ("Usage: forceme [code]");
        eax = 1;
    }
    else {
        *(local_4h) = 1;
        rax = *(local_20h);
        rax += 8;
        rax = *(rax);
        rdi = rax;
        strlen (rdi);
        if (rax == 9) {
            rax = *(local_20h);
            rax += 8;
            rax = *(rax);
            rdi = rax;
            eax = check (rdi);
            if (eax != 0) {
                rax = *(local_20h);
                rax += 8;
                rax = *(rax);
                eax = rax;
                eax = al;
                edi = eax;
                code (edi);
                al = (eax == 0x90) ? 1 : 0;
                eax = al;
                *(local_4h) &= eax;
                rax = *(local_20h);
                rax += 8;
                rax = *(rax);

```

To understand where the values come from we take a look again at the function header that we got from the disassembly:

```
|   main (int argc, char **argv, char **envp);                                                           
|           ; var char **s @ rbp-0x20                                                                    
|           ; var unsigned int local_14h @ rbp-0x14                                                      
|           ; var unsigned int local_4h @ rbp-0x4                                                        
|           ; arg unsigned int argc @ rdi                                                                
|           ; arg char **argv @ rsi
```

Considering this and the decompiled code, we see that the main function puts some constraints to our input:
- must be one value (argc = 2, see `if (*(local_14h) != 2) {`)
- must be of len 9 (see `rdi = rax; strlen (rdi); if (rax == 9) {`)

If we keep looking we will see that our input is checked against a function called check():  `eax = check (rdi);`. Let's take a look at it:

```
[0x004004c0]> s sym.check
[0x004005eb]> pdd
int64_t check () {
    *(local_8h) = rdi;
    while (al != 0) {
        rax = *(local_8h);
        eax = rax;
        if (al > 0x60) {
            rax = *(local_8h);
            eax = rax;
            if (al > 0x7a) {
                eax = 0;
                goto label_0;
            }
            else {
                *(local_8h) += 1;
                rax = *(local_8h);
                eax = rax;
            }
        }
    }
    eax = 1;
label_0:
    return rax;
}
```

We see here that the input must be one character between 'a' and 'z':
- `if (al > 0x60)`, where 0x60 = 'a'
- `if (al > 0x7a) {`, where 0x7a = 'z'


If all conditions were met in the main, it will call the code() function:

```
[0x004005c6]> pdd @ sym.code
int32_t code () {
    eax = edi;
    *(local_14h) = al;
    *(local_8h) = 7;
    *(local_4h) = 0x45;
    eax = *(local_8h);
    eax <<= 4;
    edx = eax;
    eax = local_14h;
    eax ^= *(local_4h);
    eax += edx;
    *(local_8h) = eax;
    eax = *(local_8h);
    eax &= 0x7fffffff;
    return eax;
}

```
This function does one shift of `7 <<= 4`, that is `7*2*2*2*2 = 0x70 (112d)`. Just use radare2 to calculate that (using << instead of <<=, then grep (~) for the hex and int32 values):

```
[0x00000000]> ? 7<<4~hex,int32
hex     0x70
int32   112
```

The next operation is a XOR (^) of our letter (X) with `0x45`, this will be added (+) to the previous result and the character we are looking for as a result (Y):

`(X ^ 0x45) + 0x70 = Y`

After this, main() compares each of the characters Y with all these values:
```
[0x004005c6]> pdd @ sym.main~al = (
                al = (eax == 0x90) ? 1 : 0;
                al = (eax == 0xa6) ? 1 : 0;
                al = (eax == 0x9c) ? 1 : 0;
                al = (eax == 0x99) ? 1 : 0;
                al = (eax == 0xa7) ? 1 : 0;
                al = (eax == 0xa0) ? 1 : 0;
                al = (eax == 0x99) ? 1 : 0;
                al = (eax == 0x90) ? 1 : 0;
                al = (eax == 0xaf) ? 1 : 0;
```

or from the disassembly:

```
[0x004005c6]> pdf @ sym.main~cmp
|           0x00400722      cmp dword [local_14h], 2                   ; [0x2:4]=-1 ; 2
|      |    0x0040076a      cmp rax, 9                                 ; 9
|     |||   0x004007a7      cmp eax, 0x90                              ; 144
|     |||   0x004007d1      cmp eax, 0xa6                              ; 166
|     |||   0x004007fb      cmp eax, 0x9c                              ; 156
|     |||   0x00400825      cmp eax, 0x99                              ; 153
|     |||   0x0040084f      cmp eax, 0xa7                              ; 167
|     |||   0x00400879      cmp eax, 0xa0                              ; 160
|     |||   0x004008a3      cmp eax, 0x99                              ; 153
|     |||   0x004008cd      cmp eax, 0x90                              ; 144
|     |||   0x004008f7      cmp eax, 0xaf                              ; 175
|     |||   0x00400905      cmp dword [local_4h], 0
```

Knowing this, we can just do the inverse operation, do you remember your math lessons? Now is the time to use that:

`(X ^ 0x45) + 0x70 = Y`

`(X ^ 0x45) = Y - 0x70`

`(X ^ 0x45) ^ 0x45 = (Y - 0x70) ^ 0x45`

`(Y - 0x70) ^ 0x45 = X`


And calculate it for each of the values we got from running `pdd @ sym.main~al = (`:

```

[0x004005c6]> ? (0x90-0x70)^0x45~string[1]
"e"
[0x004005c6]> ? (0xa6-0x70)^0x45~string[1]
"s"
[0x004005c6]> ? (0x9c-0x70)^0x45~string[1]
"i"
[0x004005c6]> ? (0x99-0x70)^0x45~string[1]
"l"
[0x004005c6]> ? (0xa7-0x70)^0x45~string[1]
"r"
[0x004005c6]> ? (0xa0-0x70)^0x45~string[1]
"u"
[0x004005c6]> ? (0x99-0x70)^0x45~string[1]
"l"
[0x004005c6]> ? (0x90-0x70)^0x45~string[1]
"e"
[0x004005c6]> ? (0xaf-0x70)^0x45~string[1]
"z"

```

That is: `esilrulez`:

```
$ ./forceme_9f132ba529bfd562380d448f4904c76f esilrulez
Success! You Got it!

    .-----
   / / .-/
  / /      
 | |        -
 | |.----. | |
/// .::::. \ \
||| ::/  ::  |
||; ::\__/:: |
\ '::::' /  /
  `=':-..-'`

```

So, actually it should be solved with ESIL :) Maybe next time. If you want to try it yourself there is a good example in [radare2 Explorations book](https://monosorce.gitbooks.io/radare2-explorations/content/tut3/tut3_-_esil.html). Check it out!

[@grepharder](https://twitter.com/grepharder)
