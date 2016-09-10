format MS64 COFF

extrn MessageBoxA
extrn ExitProcess

section '.text' code readable executable

 public main

 main:
        mov     r9d,0
        lea     r8,[_caption]
        lea     rdx,[_message]
        mov     rcx,0
        call    MessageBoxA
        mov     ecx,eax
        call    ExitProcess


section '.data' data readable writeable

 _caption db 'Win64 assembly',0
 _message db 'Coffee time!',0

