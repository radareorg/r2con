# Canada - Legend of Zoltan - 400pts
### Task file: zoltan_46adb5be98c671a73229cfdbdc055eea 

Let's run the application and enter `help` command.  We will have the following goal:

```[+] Fairy: Find the flag inside the dungeon but, be aware of the monsters and the fatigue!```

Let's print all strings that starts with `[+]` by typing `/ [+]`:

```asm
0x08137fde hit6_0 .nified_Ideograph[+] Move one stepba.
0x0813d45a hit6_1 .matFloat bitSize[+] Oh man...! How .
0x0813d644 hit6_2 .with mp.p == nil[+] Dude...! Trust .
0x0813dee6 hit6_3 .XqV3rcduA3HWp6yg[+] Fairy: Find the.
0x0813df40 hit6_4 .and the fatigue![+] Ouch! You have .
```
Let's check where it used by searching asm instructions that have this reference:
```asm
[0x08094250]> /c 813dee6
0x08100d7c   # 6: lea eax, [0x813dee6]
0x08100d7d   # 5: add eax, 0x813dee6
[0x08094250]> afi. 0x08100d7c
sym.main.help
```
There are symbols, so let's print all symbols that have `main.` in their name:
```asm
[0x08094250]> is | grep "main."
110 0x00178358 0x081c0358 GLOBAL    OBJ    8 main.boy
112 0x0017c568 0x081c4568 GLOBAL    OBJ    8 main.shell
114 0x0017c408 0x081c4408 GLOBAL    OBJ    4 main.p
116 0x0018ddd5 0x081d5dd5 GLOBAL    OBJ    1 main.map_shown
118 0x0017c404 0x081c4404 GLOBAL    OBJ    4 main.m
119 0x00103d70 0x0814bd70 GLOBAL    OBJ    8 main.statictmp_1
120 0x0018de1c 0x081d5e1c GLOBAL    OBJ    4 main.fk
121 0x0018de14 0x081d5e14 GLOBAL    OBJ    4 main.count
122 0x0018de18 0x081d5e18 GLOBAL    OBJ    4 main.exhausted
124 0x0017c570 0x081c4570 GLOBAL    OBJ    8 main.sol
126 0x00103d80 0x0814bd80 GLOBAL    OBJ    8 main.statictmp_3
127 0x00103d78 0x0814bd78 GLOBAL    OBJ    8 main.statictmp_2
128 0x00103bcc 0x0814bbcc GLOBAL    OBJ    2 main.statictmp_4
129 0x00103bce 0x0814bbce GLOBAL    OBJ    2 main.statictmp_5
130 0x00103bd0 0x0814bbd0 GLOBAL    OBJ    2 main.statictmp_6
131 0x00103bd2 0x0814bbd2 GLOBAL    OBJ    2 main.statictmp_7
132 0x00103bd4 0x0814bbd4 GLOBAL    OBJ    2 main.statictmp_8
133 0x00178568 0x081c0568 GLOBAL    OBJ   12 main.dungeons
134 0x00103d88 0x0814bd88 GLOBAL    OBJ    8 main.statictmp_9
135 0x0018ddd4 0x081d5dd4 GLOBAL    OBJ    1 main.initdone.
136 0x0016b0e0 0x081b30e0 GLOBAL    OBJ  528 main.statictmp_10
146 0x0017c468 0x081c4468 GLOBAL    OBJ    4 runtime.main_init_done
268 0x0018ddea 0x081d5dea GLOBAL    OBJ    1 runtime.mainStarted
369 0x00103bf8 0x0814bbf8 GLOBAL    OBJ    4 runtime.mainPC
2522 0x000489b0 0x080909b0 GLOBAL   FUNC   54 runtime.main.func1
2523 0x000489f0 0x080909f0 GLOBAL   FUNC   44 runtime.main.func2
3996 0x000b81c0 0x081001c0 GLOBAL   FUNC  348 main.(*dungeon).status
3997 0x000b8320 0x08100320 GLOBAL   FUNC  133 main.addBase64Padding
3998 0x000b83b0 0x081003b0 GLOBAL   FUNC  554 main.win
3999 0x000b85e0 0x081005e0 GLOBAL   FUNC  740 main.options
4000 0x000b88d0 0x081008d0 GLOBAL   FUNC  647 main.show_map
4001 0x000b8b60 0x08100b60 GLOBAL   FUNC   96 main.scroll_down_map
4002 0x000b8bc0 0x08100bc0 GLOBAL   FUNC   96 main.scroll_up_map
4003 0x000b8c20 0x08100c20 GLOBAL   FUNC  127 main.print
4004 0x000b8ca0 0x08100ca0 GLOBAL   FUNC   62 main.egg
4005 0x000b8ce0 0x08100ce0 GLOBAL   FUNC  114 main.banner
4006 0x000b8d60 0x08100d60 GLOBAL   FUNC   62 main.help
4007 0x000b8da0 0x08100da0 GLOBAL   FUNC  155 main.flag
4008 0x000b8e40 0x08100e40 GLOBAL   FUNC 1653 main.move
4009 0x000b94c0 0x081014c0 GLOBAL   FUNC  983 main.layout
4010 0x000b98a0 0x081018a0 GLOBAL   FUNC   46 main.quit
4011 0x000b98d0 0x081018d0 GLOBAL   FUNC 1243 main.main
4012 0x000b9db0 0x08101db0 GLOBAL   FUNC  462 main.generateDungeon
4013 0x000b9f80 0x08101f80 GLOBAL   FUNC  292 main.print.func1
4014 0x000ba0b0 0x081020b0 GLOBAL   FUNC  108 main.main.func1
4015 0x000ba120 0x08102120 GLOBAL   FUNC  112 main.init
4016 0x000ba190 0x08102190 GLOBAL   FUNC   68 type..hash.main.player
4017 0x000ba1e0 0x081021e0 GLOBAL   FUNC   69 type..eq.main.player
```

It looks like `sym.main.win` is what we are expecting to get in the end.

```asm
[0x08094250]> pd 50 @ sym.main.win
/ (fcn) sym.main.win 554
|   sym.main.win (int arg_90h, int arg_94h, int arg_98h);
|           ; var int local_10h_2 @ ebp-0x10
|           ; var int local_4h @ esp+0x4
|           ; var int local_8h @ esp+0x8
|           ; var int local_ch @ esp+0xc
|           ; var int local_10h @ esp+0x10
|           ; var int local_14h @ esp+0x14
|           ; var int local_18h @ esp+0x18
|           ; var int local_2ch @ esp+0x2c
|           ; var int local_30h @ esp+0x30
|           ; var int local_34h @ esp+0x34
|           ; var int local_38h @ esp+0x38
|           ; var int local_3ch @ esp+0x3c
|           ; var int local_5ch @ esp+0x5c
|           ; var int local_7ch @ esp+0x7c
|           ; var int local_80h @ esp+0x80
|           ; var int local_84h @ esp+0x84
|           ; var int local_88h @ esp+0x88
|           ; arg int arg_90h @ esp+0x90
|           ; arg int arg_94h @ esp+0x94
|           ; arg int arg_98h @ esp+0x98
|           ; CODE XREF from sym.main.win (0x81005d5)
|           ; CALL XREF from sym.main.move (0x8100fca)
|           0x081003b0      658b0d000000.  mov ecx, dword gs:[0]
|           0x081003b7      8b89fcffffff   mov ecx, dword [ecx - 4]
|           0x081003bd      8d4424f4       lea eax, [esp - 0xc]
|           0x081003c1      3b4108         cmp eax, dword [ecx + 8]    ; [0x8:4]=-1 ; 8
|       ,=< 0x081003c4      0f8606020000   jbe 0x81005d0
|       |   0x081003ca      81ec8c000000   sub esp, 0x8c
|       |   0x081003d0      8d44245c       lea eax, [local_5ch]        ; 0x5c ; '\' ; 92
|       |   0x081003d4      890424         mov dword [esp], eax
|       |   0x081003d7      8b8424940000.  mov eax, dword [arg_94h]    ; [0x94:4]=-1 ; 148
|       |   0x081003de      89442404       mov dword [local_4h], eax
|       |   0x081003e2      8b8424980000.  mov eax, dword [arg_98h]    ; [0x98:4]=-1 ; 152
|       |   0x081003e9      89442408       mov dword [local_8h], eax
|       |   0x081003ed      e8de31f8ff     call sym.runtime.stringtoslicebyte
|       |   0x081003f2      8b44240c       mov eax, dword [local_ch]   ; [0xc:4]=-1 ; 12
|       |   0x081003f6      8b4c2410       mov ecx, dword [local_10h]  ; [0x10:4]=-1 ; 16
|       |   0x081003fa      8b542414       mov edx, dword [local_14h]  ; [0x14:4]=-1 ; 20
|       |   0x081003fe      890424         mov dword [esp], eax
|       |   0x08100401      894c2404       mov dword [local_4h], ecx
|       |   0x08100405      89542408       mov dword [local_8h], edx
|       |   0x08100409      e8c2c2ffff     call sym.crypto_sha256.Sum256
|       |   0x0810040e      8d74240c       lea esi, [local_ch]         ; 0xc ; 12
|       |   0x08100412      8d7c243c       lea edi, [local_3ch]        ; 0x3c ; '<' ; 60
|       |   0x08100416      e8553af9ff     call fcn.08093e70
|       |   0x0810041b      8d44243c       lea eax, [local_3ch]        ; 0x3c ; '<' ; 60
|       |   0x0810041f      890424         mov dword [esp], eax
|       |   0x08100422      c74424042000.  mov dword [local_4h], 0x20  ; [0x20:4]=-1 ; 32
|       |   0x0810042a      c74424082000.  mov dword [local_8h], 0x20  ; [0x20:4]=-1 ; 32
|       |   0x08100432      e8193efaff     call sym.crypto_aes.NewCipher
|       |   0x08100437      8b442414       mov eax, dword [local_14h]  ; [0x14:4]=-1 ; 20
|       |   0x0810043b      8b4c2410       mov ecx, dword [local_10h]  ; [0x10:4]=-1 ; 16
|       |   0x0810043f      8b542418       mov edx, dword [local_18h]  ; [0x18:4]=-1 ; 24
|       |   0x08100443      8b5c240c       mov ebx, dword [local_ch]   ; [0xc:4]=-1 ; 12
|       |   0x08100447      85c0           test eax, eax
|      ,==< 0x08100449      0f8550010000   jne 0x810059f
|      ||   0x0810044f      898c24840000.  mov dword [local_84h], ecx
|      ||   0x08100456      899c24800000.  mov dword [local_80h], ebx
|      ||   0x0810045d      8b055c031c08   mov eax, dword [0x81c035c]  ; [0x81c035c:4]=86
|      ||   0x08100463      8b0d58031c08   mov ecx, dword obj.main.boy ; [0x81c0358:4]=0x813de90
|      ||   0x08100469      890c24         mov dword [esp], ecx
|      ||   0x0810046c      89442404       mov dword [local_4h], eax
|      ||   0x08100470      e8abfeffff     call sym.main.addBase64Padding
|      ||   0x08100475      8b05dc431c08   mov eax, dword [obj.encoding_base64.URLEncoding] ; [0x81c43dc:4]=0
|      ||   0x0810047b      8b4c240c       mov ecx, dword [local_ch]   ; [0xc:4]=-1 ; 12
|      ||   0x0810047f      8b542408       mov edx, dword [local_8h]   ; [0x8:4]=-1 ; 8
|      ||   0x08100483      890424         mov dword [esp], eax
|      ||   0x08100486      89542404       mov dword [local_4h], edx
|      ||   0x0810048a      894c2408       mov dword [local_8h], ecx
|      ||   0x0810048e      e8fdf5ffff     call sym.encoding_base64.__Encoding_.DecodeString
|      ||   0x08100493      8b442414       mov eax, dword [local_14h]  ; [0x14:4]=-1 ; 20
|      ||   0x08100497      8b4c2410       mov ecx, dword [local_10h]  ; [0x10:4]=-1 ; 16
[0x08094250]> px 64 @ 0x813de90
- offset -   0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
0x0813de90  7741 3635 5875 5165 4b75 7772 684f 6c4c  wA65XuQeKuwrhOlL
0x0813dea0  5745 6937 4e73 4c6a 5161 6962 4945 3772  WEi7NsLjQaibIE7r
0x0813deb0  7435 3148 6846 6364 3879 4252 6b77 7053  t51HhFcd8yBRkwpS
0x0813dec0  5758 3874 3955 7268 5439 4368 7768 7171  WX8t9UrhT9Chwhqq
```
From this disasm we can see that `sym.main.win` function uses sha256 hash of some input as AES key to decrypt hardcoded value named `obj.main.boy`.
It looks like encrypted flag. We also can see that this function is called by `sym.main.move`

So let's check `sym.main.move`:
```asm
[0x08094250]> pd 98 @ sym.main.move
/ (fcn) sym.main.move 1653
|   sym.main.move (int arg_60h, int arg_64h, int arg_68h, int arg_6ch, int arg_70h);
|           ; var int local_4h @ esp+0x4
|           ; var int local_8h @ esp+0x8
|           ; var int local_ch @ esp+0xc
|           ; var int local_10h @ esp+0x10
|           ; var int local_14h @ esp+0x14
|           ; var int local_18h @ esp+0x18
|           ; var int local_1ch @ esp+0x1c
|           ; var int local_20h @ esp+0x20
|           ; var int local_24h @ esp+0x24
|           ; var int local_28h @ esp+0x28
|           ; var int local_2ch @ esp+0x2c
|           ; var int local_30h @ esp+0x30
|           ; var int local_34h @ esp+0x34
|           ; var int local_38h @ esp+0x38
|           ; var int local_3ch @ esp+0x3c
|           ; var int local_40h @ esp+0x40
|           ; var int local_44h @ esp+0x44
|           ; var int local_48h @ esp+0x48
|           ; var int local_4ch @ esp+0x4c
|           ; var int local_50h @ esp+0x50
|           ; var int local_54h @ esp+0x54
|           ; var int local_58h @ esp+0x58
|           ; arg int arg_60h @ esp+0x60
|           ; arg int arg_64h @ esp+0x64
|           ; arg int arg_68h @ esp+0x68
|           ; arg int arg_6ch @ esp+0x6c
|           ; arg int arg_70h @ esp+0x70
|           ; CALL XREF from sym.main.options (0x8100855)
|           ; CODE XREF from sym.main.move (0x81014b0)
|           0x08100e40      658b0d000000.  mov ecx, dword gs:[0]
|           0x08100e47      8b89fcffffff   mov ecx, dword [ecx - 4]
|           0x08100e4d      3b6108         cmp esp, dword [ecx + 8]    ; [0x8:4]=-1 ; 8
|       ,=< 0x08100e50      0f8655060000   jbe 0x81014ab
|       |   0x08100e56      83ec5c         sub esp, 0x5c               ; '\'
|       |   0x08100e59      31c0           xor eax, eax
|      ,==< 0x08100e5b      eb04           jmp 0x8100e61
|      ||   ; XREFS: CODE 0x08100f0a  CODE 0x08100f33  CODE 0x08100f6f  CODE 0x0810100b  CODE 0x081010ba  CODE 0x0810116c  
|      ||   ; XREFS: CODE 0x08101214  CODE 0x081012c6  CODE 0x08101315  
|   ...---> 0x08100e5d      8b44242c       mov eax, dword [local_2ch]  ; [0x2c:4]=-1 ; ',' ; 44
|   :::||   ; CODE XREF from sym.main.move (0x8100e5b)
|   :::`--> 0x08100e61      8b4c2468       mov ecx, dword [arg_68h]    ; [0x68:4]=-1 ; 'h' ; 104
|   ::: |   0x08100e65      39c8           cmp eax, ecx
|   :::,==< 0x08100e67      0f8d2a060000   jge 0x8101497
|   :::||   0x08100e6d      8b542464       mov edx, dword [arg_64h]    ; [0x64:4]=-1 ; 'd' ; 100
|   :::||   0x08100e71      0fb61c02       movzx ebx, byte [edx + eax]
|   :::||   0x08100e75      81fb80000000   cmp ebx, 0x80               ; 128
|  ,======< 0x08100e7b      0f8df1050000   jge 0x8101472
|  |:::||   0x08100e81      8d6801         lea ebp, [eax + 1]          ; 1
|  |:::||   ; CODE XREF from sym.main.move (0x8101492)
|  |:::||   0x08100e84      8b35145e1d08   mov esi, dword [obj.main.count] ; [0x81d5e14:4]=0
|  |:::||   0x08100e8a      8b3d185e1d08   mov edi, dword [obj.main.exhausted] ; [0x81d5e18:4]=0
|  |:::||   0x08100e90      39fe           cmp esi, edi
| ,=======< 0x08100e92      0f8fc2050000   jg 0x810145a
| ||:::||   0x08100e98      8d7b9f         lea edi, [ebx - 0x61]
| ||:::||   0x08100e9b      83ff19         cmp edi, 0x19               ; 25
| ========< 0x08100e9e      0f8680050000   jbe 0x8101424
| ||:::||   0x08100ea4      89d8           mov eax, ebx
| ||:::||   ; CODE XREFS from sym.main.move (0x810144c, 0x8101455)
| ||:::||   0x08100ea6      8b3508441c08   mov esi, dword [obj.main.p] ; [0x81c4408:4]=0
| ||:::||   0x08100eac      83fb6a         cmp ebx, 0x6a               ; 'j' ; 106
| ========< 0x08100eaf      0f8f30050000   jg 0x81013e5
| ||:::||   0x08100eb5      83fb69         cmp ebx, 0x69               ; 'i' ; 105
| ========< 0x08100eb8      0f8500050000   jne 0x81013be
| ||:::||   0x08100ebe      8b1e           mov ebx, dword [esi]
| ||:::||   0x08100ec0      4b             dec ebx
| ||:::||   0x08100ec1      891e           mov dword [esi], ebx
| ||:::||   0x08100ec3      85db           test ebx, ebx
| ========< 0x08100ec5      7d06           jge 0x8100ecd
| ||:::||   0x08100ec7      c70600000000   mov dword [esi], 0
| ||:::||   ; XREFS: CODE 0x08100ec5  CODE 0x081013cc  CODE 0x081013d9  CODE 0x081013f2  CODE 0x081013fe  CODE 0x08101412  
| ||:::||   ; XREFS: CODE 0x0810141f  
| --------> 0x08100ecd      bb01000000     mov ebx, 1
| ||:::||   ; CODE XREF from sym.main.move (0x81013e0)
| ||:::||   0x08100ed2      896c242c       mov dword [local_2ch], ebp
| ||:::||   0x08100ed6      84db           test bl, bl
| ========< 0x08100ed8      0f853c040000   jne 0x810131a
| ||:::||   ; CODE XREFS from sym.main.move (0x81013a2, 0x81013b9)
| ||:::||   0x08100ede      8b0504441c08   mov eax, dword [obj.main.m] ; [0x81c4404:4]=0
| ||:::||   0x08100ee4      890424         mov dword [esp], eax
| ||:::||   0x08100ee7      8b0508441c08   mov eax, dword [obj.main.p] ; [0x81c4408:4]=0
| ||:::||   0x08100eed      89442404       mov dword [local_4h], eax
| ||:::||   0x08100ef1      e8caf2ffff     call sym.main.__dungeon_.status
| ||:::||   0x08100ef6      8b442408       mov eax, dword [local_8h]   ; [0x8:4]=-1 ; 8
| ||:::||   0x08100efa      83f804         cmp eax, 4                  ; 4
| ========< 0x08100efd      0f8fe0000000   jg 0x8100fe3
| ||:::||   0x08100f03      83f801         cmp eax, 1                  ; 1
| ========< 0x08100f06      7f44           jg 0x8100f4c
| ||:::||   0x08100f08      85c0           test eax, eax
| ||`=====< 0x08100f0a      0f844dffffff   je 0x8100e5d
| || ::||   0x08100f10      83f801         cmp eax, 1                  ; 1
| ||,=====< 0x08100f13      7523           jne 0x8100f38
| |||::||   0x08100f15      8b442460       mov eax, dword [arg_60h]    ; [0x60:4]=-1 ; '`' ; 96
| |||::||   0x08100f19      890424         mov dword [esp], eax
| |||::||   0x08100f1c      8d0d28761308   lea ecx, [0x8137628]        ; "[*] Level Up!bad map statebad pagesize exchange fullfatal error: invalid UTF-8invalid base invalid pointlevel 3 resetload64 fai"
| |||::||   0x08100f22      894c2404       mov dword [local_4h], ecx
| |||::||   0x08100f26      c74424080d00.  mov dword [local_8h], 0xd   ; [0xd:4]=-1 ; 13
| |||::||   0x08100f2e      e8edfcffff     call sym.main.print
| |||`====< 0x08100f33      e925ffffff     jmp 0x8100e5d
| ||| :||   ; CODE XREFS from sym.main.move (0x8100f13, 0x810103d)
| ||`-----> 0x08100f38      c744246c0000.  mov dword [arg_6ch], 0
| ||  :||   0x08100f40      c74424700000.  mov dword [arg_70h], 0
| ||  :||   0x08100f48      83c45c         add esp, 0x5c               ; '\'
| ||  :||   0x08100f4b      c3             ret
| --------> 0x08100f4c      83f802         cmp eax, 2                  ; 2
| || ,====< 0x08100f4f      7523           jne 0x8100f74
| || |:||   0x08100f51      8b442460       mov eax, dword [arg_60h]    ; [0x60:4]=-1 ; '`' ; 96
| || |:||   0x08100f55      890424         mov dword [esp], eax
| || |:||   0x08100f58      8d0da57a1308   lea ecx, [0x8137aa5]        ; "[*] Level Down!]\n\tmorebuf={pc:acquirep: p->m=advertise errorbad select sizebad system callforce gc (idle)invalid paddingkey has"
| || |:||   0x08100f5e      894c2404       mov dword [local_4h], ecx
| || |:||   0x08100f62      c74424080f00.  mov dword [local_8h], 0xf   ; [0xf:4]=-1 ; 15
| || |:||   0x08100f6a      e8b1fcffff     call sym.main.print
| || |`===< 0x08100f6f      e9e9feffff     jmp 0x8100e5d
| || | ||   ; CODE XREF from sym.main.move (0x8100f4f)
| || `----> 0x08100f74      83f803         cmp eax, 3                  ; 3
| ||  ,===< 0x08100f77      7518           jne 0x8100f91
| ||  |||   0x08100f79      8b05e4441c08   mov eax, dword [0x81c44e4]  ; [0x81c44e4:4]=0
| ||  |||   0x08100f7f      8b0de0441c08   mov ecx, dword [obj.github.com_jroimartin_gocui.ErrQuit] ; [0x81c44e0:4]=0
| ||  |||   0x08100f85      894c246c       mov dword [arg_6ch], ecx
| ||  |||   0x08100f89      89442470       mov dword [arg_70h], eax
| ||  |||   0x08100f8d      83c45c         add esp, 0x5c               ; '\'
| ||  |||   0x08100f90      c3             ret
| ||  |||   ; CODE XREF from sym.main.move (0x8100f77)
| ||  `---> 0x08100f91      8b442460       mov eax, dword [arg_60h]    ; [0x60:4]=-1 ; '`' ; 96
| ||   ||   0x08100f95      890424         mov dword [esp], eax
| ||   ||   0x08100f98      8d0d986a1308   lea ecx, [0x8136a98]        ; "[*] Good![:^word:][:alnum:][:alpha:][:ascii:][:blank:][:cntrl:][:digit:][:graph:][:lower:][:print:][:punct:][:space:][:upper:]a"
| ||   ||   0x08100f9e      894c2404       mov dword [local_4h], ecx
| ||   ||   0x08100fa2      c74424080900.  mov dword [local_8h], 9
| ||   ||   0x08100faa      e871fcffff     call sym.main.print
| ||   ||   0x08100faf      8b442460       mov eax, dword [arg_60h]    ; [0x60:4]=-1 ; '`' ; 96
| ||   ||   0x08100fb3      890424         mov dword [esp], eax
| ||   ||   0x08100fb6      8b0574451c08   mov eax, dword [0x81c4574]  ; [0x81c4574:4]=0
| ||   ||   0x08100fbc      8b0d70451c08   mov ecx, dword [obj.main.sol] ; [0x81c4570:4]=0
| ||   ||   0x08100fc2      894c2404       mov dword [local_4h], ecx
| ||   ||   0x08100fc6      89442408       mov dword [local_8h], eax
| ||   ||   0x08100fca      e8e1f3ffff     call sym.main.win
| ||   ||   0x08100fcf      c744246c0000.  mov dword [arg_6ch], 0
| ||   ||   0x08100fd7      c74424700000.  mov dword [arg_70h], 0
| ||   ||   0x08100fdf      83c45c         add esp, 0x5c               ; '\'
| ||   ||   0x08100fe2      c3             ret
```
The most important stuff here is that the `sym.main.__dungeon_.status` function must return 4, then we will get `sym.main.win` executed.
In addition to that we can see that there are checks for `j` and `i` characters. If it is a dungeon challenge then `i-j-k-l` key should be used for player's movements.

By typing command `move i` we can confirm that indeed the player starts to move - application prints `[+] Move one step`.

Now we can analyze `sym.main.__dungeon_.status` functions. It takes two important arguments: `obj.main.m` and `obj.main.p`:
```asm
[0x08094250]> pd @ sym.main.__dungeon_.status
/ (fcn) sym.main.__dungeon_.status 348
|   sym.main.__dungeon_.status (int arg_104h, int arg_108h, int arg_10ch);
|           ; arg int arg_104h @ esp+0x104
|           ; arg int arg_108h @ esp+0x108
|           ; arg int arg_10ch @ esp+0x10c
|           ; CODE XREF from sym.main.__dungeon_.status (0x8100317)
|           ; CALL XREF from sym.main.move (0x8100ef1)
|           0x081001c0      658b0d000000.  mov ecx, dword gs:[0]
|           0x081001c7      8b89fcffffff   mov ecx, dword [ecx - 4]
|           0x081001cd      8d442480       lea eax, [esp - 0x80]
|           0x081001d1      3b4108         cmp eax, dword [ecx + 8]    ; [0x8:4]=-1 ; 8
|       ,=< 0x081001d4      0f8638010000   jbe 0x8100312
|       |   0x081001da      81ec00010000   sub esp, 0x100
|       |   0x081001e0      8b8424040100.  mov eax, dword [arg_104h]   ; [0x104:4]=-1 ; 260
|       |   0x081001e7      8400           test byte [eax], al         ; [0x2:1]=255 ; 2
|       |   0x081001e9      8b8c24080100.  mov ecx, dword [arg_108h]   ; [0x108:4]=-1 ; 264
|       |   0x081001f0      8b5108         mov edx, dword [ecx + 8]    ; [0x8:4]=-1 ; 8
|       |   0x081001f3      83fa02         cmp edx, 2                  ; 2
|      ,==< 0x081001f6      0f830f010000   jae 0x810030b
|      ||   0x081001fc      c1e208         shl edx, 8
|      ||   0x081001ff      8d3410         lea esi, [eax + edx]
|      ||   0x08100202      8d3c24         lea edi, [esp]
|      ||   0x08100205      e8363af9ff     call fcn.08093c40
|      ||   0x0810020a      8b8424080100.  mov eax, dword [arg_108h]   ; [0x108:4]=-1 ; 264
|      ||   0x08100211      8b08           mov ecx, dword [eax]
|      ||   0x08100213      83f910         cmp ecx, 0x10               ; 16
|     ,===< 0x08100216      0f83e8000000   jae 0x8100304
|     |||   0x0810021c      8b5004         mov edx, dword [eax + 4]    ; [0x4:4]=-1 ; 4
|     |||   0x0810021f      83fa10         cmp edx, 0x10               ; 16
|    ,====< 0x08100222      0f83dc000000   jae 0x8100304
|    ||||   0x08100228      c1e104         shl ecx, 4
|    ||||   0x0810022b      8d0c0c         lea ecx, [esp + ecx]
|    ||||   0x0810022e      0fb60c11       movzx ecx, byte [ecx + edx]
|    ||||   0x08100232      80f964         cmp cl, 0x64                ; 'd' ; 100
|   ,=====< 0x08100235      775b           ja 0x8100292
|   |||||   0x08100237      80f923         cmp cl, 0x23                ; '#' ; 35
|  ,======< 0x0810023a      7728           ja 0x8100264
|  ||||||   0x0810023c      80f920         cmp cl, 0x20                ; 32
| ,=======< 0x0810023f      7510           jne 0x8100251
| |||||||   0x08100241      31c0           xor eax, eax
| |||||||   ; XREFS: CODE 0x0810025b  CODE 0x08100262  CODE 0x08100272  CODE 0x08100290  CODE 0x081002a1  CODE 0x081002b7  
| |||||||   ; XREFS: CODE 0x081002be  CODE 0x081002d9  
| --------> 0x08100243      8984240c0100.  mov dword [arg_10ch], eax
| |||||||   0x0810024a      81c400010000   add esp, 0x100
| |||||||   0x08100250      c3             ret
| |||||||   ; CODE XREF from sym.main.__dungeon_.status (0x810023f)
| `-------> 0x08100251      80f923         cmp cl, 0x23                ; '#' ; 35
| ,=======< 0x08100254      7507           jne 0x810025d
| |||||||   0x08100256      b803000000     mov eax, 3
| ========< 0x0810025b      ebe6           jmp 0x8100243
| |||||||   ; CODE XREFS from sym.main.__dungeon_.status (0x8100254, 0x8100277, 0x81002a6, 0x81002e1)
| `-------> 0x0810025d      b803000000     mov eax, 3
| ========< 0x08100262      ebdf           jmp 0x8100243
|  ||||||   ; CODE XREF from sym.main.__dungeon_.status (0x810023a)
|  `------> 0x08100264      80f94d         cmp cl, 0x4d                ; 'M' ; 77
|  ,======< 0x08100267      750b           jne 0x8100274
|  ||||||   0x08100269      c6401101       mov byte [eax + 0x11], 1
|  ||||||   0x0810026d      b809000000     mov eax, 9
| ========< 0x08100272      ebcf           jmp 0x8100243
|  ||||||   ; CODE XREF from sym.main.__dungeon_.status (0x8100267)
|  `------> 0x08100274      80f964         cmp cl, 0x64                ; 'd' ; 100
| ========< 0x08100277      75e4           jne 0x810025d
|   |||||   0x08100279      8b4808         mov ecx, dword [eax + 8]    ; [0x8:4]=-1 ; 8
|   |||||   0x0810027c      49             dec ecx
|   |||||   0x0810027d      894808         mov dword [eax + 8], ecx
|   |||||   0x08100280      85c9           test ecx, ecx
|  ,======< 0x08100282      7d07           jge 0x810028b
|  ||||||   0x08100284      c74008000000.  mov dword [eax + 8], 0
|  ||||||   ; CODE XREF from sym.main.__dungeon_.status (0x8100282)
|  `------> 0x0810028b      b802000000     mov eax, 2
| ========< 0x08100290      ebb1           jmp 0x8100243
|   |||||   ; CODE XREF from sym.main.__dungeon_.status (0x8100235)
|   `-----> 0x08100292      80f96d         cmp cl, 0x6d                ; 'm' ; 109
|   ,=====< 0x08100295      7734           ja 0x81002cb
|   |||||   0x08100297      80f966         cmp cl, 0x66                ; 'f' ; 102
|  ,======< 0x0810029a      7507           jne 0x81002a3
|  ||||||   0x0810029c      b804000000     mov eax, 4
| ========< 0x081002a1      eba0           jmp 0x8100243
|  ||||||   ; CODE XREF from sym.main.__dungeon_.status (0x810029a)
|  `------> 0x081002a3      80f96d         cmp cl, 0x6d                ; 'm' ; 109
```

For now it does not make much sense, but at least we can see that in order to return 4 some internal value must be equal to `f` character.

Let's check how `obj.main.m` and `obj.main.p` are used.

```asm
[0x08094250]> axF obj.main.m 
Finding references of flags matching 'obj.main.m'...
[0x081d7fd0-0x081d80a8] sym.main.show_map 0x8100942 [DATA] movzx ecx, byte [obj.main.map_shown]
sym.main.show_map 0x8100aff [DATA] mov byte [obj.main.map_shown], 0
sym.main.show_map 0x8100b22 [DATA] mov byte [obj.main.map_shown], 1
sym.main.main 0x810197a [DATA] mov byte [obj.main.map_shown], 0
[0x081d7fd0-0x081d80a8] entry0 0x8094250 [DATA] jmp sym._rt0_386
sym.main.show_map 0x81009da [DATA] mov ebp, dword [obj.main.m]
sym.main.move 0x8100ede [DATA] mov eax, dword [obj.main.m]
sym.main.main 0x8101932 [DATA] mov dword [obj.main.m], ecx
sym.main.main 0x8101d73 [DATA] lea eax, [obj.main.m]
[0x08094250]> axF obj.main.p
Finding references of flags matching 'obj.main.p'...
[0x081d7fd0-0x081d80a8] sym.main.show_map 0x81008e9 [DATA] mov eax, dword [obj.main.p]
sym.main.move 0x8100ea6 [DATA] mov esi, dword [obj.main.p]
sym.main.move 0x8100ee7 [DATA] mov eax, dword [obj.main.p]
sym.main.main 0x8101916 [DATA] mov dword [obj.main.p], eax
sym.main.main 0x8101d8a [DATA] lea ecx, [obj.main.p]
```
Both `obj.main.m` and `obj.main.p` are initialized in `sym.main.main` and later used in `sym.main.move` (function that handles 'move' command).

Here is how `sym.main.main` looks:

```asm
[0x08094250]> pd 24 @ sym.main.main
/ (fcn) sym.main.main 1243
|   sym.main.main ();
|           ; var int local_4h @ esp+0x4
|           ; var int local_8h @ esp+0x8
|           ; var int local_ch @ esp+0xc
|           ; var char *local_10h @ esp+0x10
|           ; var int local_14h @ esp+0x14
|           ; var int local_18h @ esp+0x18
|           ; var int local_1ch @ esp+0x1c
|           ; var int local_20h @ esp+0x20
|           ; var int local_24h @ esp+0x24
|           ; var int local_28h @ esp+0x28
|           ; var int local_2ch @ esp+0x2c
|           ; var int local_30h @ esp+0x30
|           ; var int local_34h @ esp+0x34
|           ; var int local_38h @ esp+0x38
|           ; var int local_3ch @ esp+0x3c
|           ; var int local_40h @ esp+0x40
|           ; var int local_44h @ esp+0x44
|           ; var int local_48h @ esp+0x48
|           ; var int local_4ch @ esp+0x4c
|           ; var int local_50h @ esp+0x50
|           ; var int local_54h @ esp+0x54
|           ; var int local_58h @ esp+0x58
|           ; var int local_5ch @ esp+0x5c
|           ; var int local_60h @ esp+0x60
|           ; var int local_64h @ esp+0x64
|           ; CODE XREF from sym.main.main (0x8101da6)
|           0x081018d0      658b0d000000.  mov ecx, dword gs:[0]
|           0x081018d7      8b89fcffffff   mov ecx, dword [ecx - 4]
|           0x081018dd      3b6108         cmp esp, dword [ecx + 8]    ; [0x8:4]=-1 ; 8
|       ,=< 0x081018e0      0f86bb040000   jbe 0x8101da1
|       |   0x081018e6      83ec68         sub esp, 0x68               ; 'h'
|       |   0x081018e9      8d05e0a21208   lea eax, [0x812a2e0]
|       |   0x081018ef      890424         mov dword [esp], eax
|       |   0x081018f2      e88932f5ff     call sym.runtime.newobject
|       |   0x081018f7      8b442404       mov eax, dword [local_4h]   ; [0x4:4]=-1 ; 4
|       |   0x081018fb      c7400c030000.  mov dword [eax + 0xc], 3
|       |   0x08101902      66c740100000   mov word [eax + 0x10], 0
|       |   0x08101908      8b0d805f1d08   mov ecx, dword [obj.runtime.writeBarrier] ; [0x81d5f80:4]=0
|       |   0x0810190e      85c9           test ecx, ecx
|      ,==< 0x08101910      0f8574040000   jne 0x8101d8a
|      ||   0x08101916      890508441c08   mov dword [obj.main.p], eax ; [0x81c4408:4]=0
|      ||   ; CODE XREF from sym.main.main (0x8101d9c)
|      ||   0x0810191c      e88f040000     call sym.main.generateDungeon
|      ||   0x08101921      8b05805f1d08   mov eax, dword [obj.runtime.writeBarrier] ; [0x81d5f80:4]=0
|      ||   0x08101927      8b0c24         mov ecx, dword [esp]
|      ||   0x0810192a      85c0           test eax, eax
|     ,===< 0x0810192c      0f8541040000   jne 0x8101d73
|     |||   0x08101932      890d04441c08   mov dword [obj.main.m], ecx ; [0x81c4404:4]=0
|     |||   ; CODE XREF from sym.main.main (0x8101d85)
|     |||   0x08101938      c7051c5e1d08.  mov dword [obj.main.fk], 0  ; [0x81d5e1c:4]=0
|     |||   0x08101942      c705145e1d08.  mov dword [obj.main.count], 0 ; [0x81d5e14:4]=0
|     |||   0x0810194c      c705185e1d08.  mov dword [obj.main.exhausted], 0x58 ; 'X' ; [0x81d5e18:4]=0
```
Both `obj.main.p` and `obj.main.m` looks like complex objects, so now I would prefer to check its value in debugger.

```asm
$ radare2 -d ./zoltan_46adb5be98c671a73229cfdbdc055eea
[0x08094250]> db 0x08101916
[0x08094250]> dc
[0x08101916]> px 32 @ eax
- offset -   0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
0x19210380  0000 0000 0000 0000 0000 0000 0300 0000  ................
0x19210390  0000 0000 0000 0000 0000 0000 0000 0000  ................
[0x08101932]> db 0x08101932
[0x08101932]> dc
[0x08101932]> px 512 @ ecx
- offset -   0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
0x19286000  2023 4d20 2023 2323 2323 2323 7523 2323   #M  #######u###
0x19286010  2023 2323 2023 2320 2020 6d23 2023 2323   ### ##   m# ###
0x19286020  2023 2323 2023 2320 2320 2323 2023 2323   ### ## # ## ###
0x19286030  2023 2323 2023 2320 2320 2323 206d 6d23   ### ## # ## mm#
0x19286040  2023 2323 2023 2373 2320 2323 2023 2023   ### ##s# ## # #
0x19286050  2023 6d20 2023 2323 2320 2323 2023 2023   #m  #### ## # #
0x19286060  2023 2323 2020 6d23 2320 2323 2023 2023   ###  m## ## # #
0x19286070  2020 2020 2023 2323 2320 2323 2023 2023       #### ## # #
0x19286080  2023 2323 2023 2323 2320 2323 2023 2023   ### #### ## # #
0x19286090  2023 2323 2023 2323 2320 2323 2023 2023   ### #### ## # #
0x192860a0  2023 2323 2023 2323 2320 2323 2023 2023   ### #### ## # #
0x192860b0  2023 6d20 2020 2020 2020 206d 2020 206d   #m        m   m
0x192860c0  2023 2323 2023 2323 2323 2323 2323 2323   ### ###########
0x192860d0  2023 2323 2023 6623 2323 2323 2323 2323   ### #f#########
0x192860e0  6d23 2323 2023 2023 2323 2323 2323 2323  m### # #########
0x192860f0  2323 2323 6d23 6d6d 6d20 2323 2323 2323  ####m#mmm ######
0x19286100  2323 2323 2323 2323 2323 2323 2023 2323  ############ ###
0x19286110  6d23 236d 2323 2323 2323 2323 2023 2323  m##m######## ###
0x19286120  2023 2320 2323 2020 2020 2020 2023 2323   ## ##       ###
0x19286130  2023 2320 2323 2023 2323 2323 2023 2323   ## ## ##### ###
0x19286140  2023 2320 2323 2023 206d 2023 2023 2323   ## ## # m # ###
0x19286150  2023 2320 2323 2023 2020 6d23 2023 2323   ## ## #  m# ###
0x19286160  2023 2320 2020 2023 6d20 2023 2023 2323   ##    #m  # ###
0x19286170  2023 2320 2323 2323 2020 2023 2023 2323   ## ####   # ###
0x19286180  2020 2320 2323 2323 2020 6d23 2023 2323    # ####  m# ###
0x19286190  2320 2020 2020 2020 2020 2023 2023 2323  #          # ###
0x192861a0  2320 2323 2323 2023 2020 2023 2023 2323  # #### #   # ###
0x192861b0  2320 2323 6d23 2023 206d 2023 2023 2323  # ##m# # m # ###
0x192861c0  2320 2323 2023 2023 2020 6d23 2023 2323  # ## # #  m# ###
0x192861d0  2320 2323 2023 2023 2020 2023 2023 2323  # ## # #   # ###
0x192861e0  2320 2323 2023 2323 2320 2323 2023 2323  # ## #### ## ###
0x192861f0  236d 2020 2020 206d 2364 2020 2023 2323  #m     m#d   ###
```
The `obj.main.m` contains generated dungeon (or maze). It's important to note that this maze is *always* the same, even if app is executed several times.

We don't know what kind of object `obj.main.p` is, but we know that it contains zeros.
Since it is  passed to `sym.main.__dungeon_.status` we can assume it contains a current player's position.

Because `sym.main.__dungeon_.status` has checks for values that `obj.main.p` contains.

```asm
0x0810020a      8b8424080100.  mov eax, dword [arg_108h]   ; [0x108:4]=-1 ; 264
0x08100211      8b08           mov ecx, dword [eax]
0x08100213      83f910         cmp ecx, 0x10               ; 16
0x08100216      0f83e8000000   jae 0x8100304
0x0810021c      8b5004         mov edx, dword [eax + 4]    ; [0x4:4]=-1 ; 4
0x0810021f      83fa10         cmp edx, 0x10               ; 16
0x08100222      0f83dc000000   jae 0x8100304
```

Based on this dissasembly, we can assume that `obj.main.p` contains X,Y position. And the size of maze is 16x16.

If we divide memory into two 16x16 mazes then makes a bit more sense:
```asm
  #M  #######u###
  ### ##   m# ###
  ### ## # ## ###
  ### ## # ## mm#
  ### ##s# ## # #
  #m  #### ## # #
  ###  m## ## # #
      #### ## # #
  ### #### ## # #
  ### #### ## # #
  ### #### ## # #
  #m        m   m
  ### ###########
  ### #f#########
 m### # #########
 ####m#mmm ######
-----------------
 ############ ###
 m##m######## ###
  ## ##       ###
  ## ## ##### ###
  ## ## # m # ###
  ## ## #  m# ###
  ##    #m  # ###
  ## ####   # ###
   # ####  m# ###
 #          # ###
 # #### #   # ###
 # ##m# # m # ###
 # ## # #  m# ###
 # ## # #   # ###
 # ## #### ## ###
 #m     m#d   ###
```

Since there are two mazes, we can assume that at some point the player can be transfered from one maze to another.
In addition to that, we have seen strings like `[*] Level Up!` and `[*] Level Down!`, which confirms our assumption.

At this point I though I can solve this challenge manually.
But if we would enter `move k` command 4 times to move player's position down then app won't print `[+] Move one step` anymore.

Figuring out why the player is not moving after few commands was the most hardest task in this challenge.

By playing directly with app, I figured out that after first moves suddenly the app accepts different than `i-j-k-l` keys.
For example we can kill the player, by entering following command:
```
move k
move m
```

Since apps can accept different than `i-j-k-l` keys. Let's check how they are checked in `sym.main.move` function:

```asm
|   :::||   0x08100e6d      8b542464       mov edx, dword [arg_64h]    ; [0x64:4]=-1 ; 'd' ; 100
|   :::||   0x08100e71      0fb61c02       movzx ebx, byte [edx + eax]   << OUR INPUT IS HERE
|   :::||   0x08100e75      81fb80000000   cmp ebx, 0x80               ; 128
|  ,======< 0x08100e7b      0f8df1050000   jge 0x8101472
|  |:::||   0x08100e81      8d6801         lea ebp, [eax + 1]          ; 1
|  |:::||   ; CODE XREF from sym.main.move (0x8101492)
|  |:::||   0x08100e84      8b35145e1d08   mov esi, dword [obj.main.count] ; [0x81d5e14:4]=0
|  |:::||   0x08100e8a      8b3d185e1d08   mov edi, dword [obj.main.exhausted] ; [0x81d5e18:4]=0
|  |:::||   0x08100e90      39fe           cmp esi, edi
| ,=======< 0x08100e92      0f8fc2050000   jg 0x810145a
| ||:::||   0x08100e98      8d7b9f         lea edi, [ebx - 0x61]
| ||:::||   0x08100e9b      83ff19         cmp edi, 0x19               ; 25
| ========< 0x08100e9e      0f8680050000   jbe 0x8101424
```

After this block we are going to `0x8101424` address. Register `esi` contains `obj.main.count` value and register `edi` contains our input minus 0x61 (`a`).

Let's check block at `0x8101424`:
```asm
[0x08094250]> pd 17 @ 0x8101424
|       :   ; CODE XREF from sym.main.move (0x8100e9e)
|       :   0x08101424      b84fecc44e     mov eax, 0x4ec4ec4f
|       :   0x08101429      89d7           mov edi, edx
|       :   0x0810142b      f7ee           imul esi
|       :   0x0810142d      c1fa03         sar edx, 3
|       :   0x08101430      89f0           mov eax, esi
|       :   0x08101432      c1fe1f         sar esi, 0x1f
|       :   0x08101435      29f2           sub edx, esi
|       :   0x08101437      6bd21a         imul edx, edx, 0x1a
|       :   0x0810143a      29d0           sub eax, edx
|       :   0x0810143c      89da           mov edx, ebx
|       :   0x0810143e      29c3           sub ebx, eax
|       :   0x08101440      83fb61         cmp ebx, 0x61               ; 'a' ; 97
|      ,==< 0x08101443      7d0c           jge 0x8101451
|      |:   0x08101445      83c31a         add ebx, 0x1a
|      |:   0x08101448      89d0           mov eax, edx
|      |:   0x0810144a      89fa           mov edx, edi
|      |`=< 0x0810144c      e955faffff     jmp 0x8100ea6
```

This block a bit hard to understand, but the purspose is to make sure that entered value contains printable character between `0x61` and `0x7a`.
Since `obj.main.count` is involved I made assumption that it calculates something like `VALUE + (obj.main.count % 0x1A)`.

It's important to note that `sym.main.move` has the following check:
```asm
0x08100e84      8b35145e1d08   mov esi, dword [obj.main.count] ; [0x81d5e14:4]=0
0x08100e8a      8b3d185e1d08   mov edi, dword [obj.main.exhausted] ; [0x81d5e18:4]=0
0x08100e90      39fe           cmp esi, edi
0x08100e92      0f8fc2050000   jg 0x810145a
```
The `obj.main.exhausted` is initializated with `0x58` in `sym.main.main` function.
So we have limited number of moves, specifically it should be less than 88.

Since our input is probably used as input for `sym.main.win` function, we can assume that there is only one right solution.

Our goal is to move player from 0,0 to f. We can use `d` and `u` cells in order to be transfered between mazes.
Here is our path:
```asm
  #M  #######u###
 ↓### ##↔↔↔m#↑###
 ↓### ##↕#↕##↑###
 ↓### ##↕#↕##↑mm#
 ↓### ##s#↕##↑# #
 ↓#m  ####↕##↑# #
 ↓###  m##↕##↑# #
 ↓→→→↓####↕##↑# #
  ###↓####↕##↑# #
  ###↓####↕##↑# #
  ###↓####↕##↑# #
  #m ↓→→→→→→→→  m
  ### ###########
  ### #f#########
 m### #↑#########
 ####m#←←← ######
-----------------
 ############ ###
 m##m########↓###
  ## ##      ↓###
  ## ## #####↓###
  ## ## # m #↓###
  ## ## #  m#↓###
  ##    #m  #↓###
  ## ####   #↓###
   # ####  m#↓###
 #          #↓###
 # #### #   #↓###
 # ##m# # m #↓###
 # ## # #  m#↓###
 # ## # #   #↓###
 # ## #### ##↓###
 #m     m#d←←←###
```

Note that at first we must move player to cell `s` which represents `sword`, because otherwise we can't pass monsters (`m` cells).

Since putting all moves manually is PITA. I decided to do a small python script, that enters all commands automatically.
The python script uses [xdotool](https://github.com/jordansissel/xdotool) in order to enter commands to the application.

We have to open two terminals, find the ID of termnial window with app by running following command: `xdotool search -name TITLE`.
Then we can change hardcoded ID of window in the python script.

Here is the script:
```python
import time
import os

answer = 'kkkkkkkllllkkkkllllliiiiiiiiiijjkkkiiillkkkkkkkkkkllliiiiiiiiiiikkkkkkkkkkkkkkkjjjjjjii' 
for i in range(len(answer)):

    char = ord(answer[i]) + (i % 0x1A)
    if char > 0x7A:
        char -= 0x1A
    print(answer[i] + ' -> ' + chr(char))
    os.system('xdotool windowfocus 37751905;xdotool type "move {0:s}";xdotool key Return;xdotool key Return;'.format(chr(char)))
    time.sleep(0.3)
```

Once script is finished we have our well deserved flag: `r2con{_y0U_G0T_d4_Tr34sur3=FooKud0s!}`


