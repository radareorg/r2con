#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char shell[] = "/bin/sh";


void nothing(void)  { __asm__ ("movl $0x000000B, %eax"); }
void nothing2(void) { __asm__ ("xor %eax, %eax"); }
void nothing3(void) { __asm__ ("xor %ecx, %ecx"); }
void nothing4(void) { __asm__ ("xor %edx, %edx"); }
void nothing5(void) { __asm__ ("int $0x80"); }

void aux2(void)
{
    char *pointer;
    pointer = shell;
    pointer++;
}

void nasty(void)
{
    char local[20];
    if (!getenv("INPUT_DATA")) {
       printf("env var INPUT_DATA must be defined\n");
       return;
    }

    strcpy(local, getenv("INPUT_DATA"));
}

void main(void)
{
    nasty();
    printf("Exiting\n");
}


