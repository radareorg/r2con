/*
 * This binary requires stack-boundary to 2 to keep stack aligned to 2^n
 *
 * $ gcc -mpreferred-stack-boundary=2 simplebof.c -m32 -o simplebof
 */

#include <stdio.h>
#include <string.h>

void trick(void)
{
   __asm__("jmp *%esp");
}

void dummy(char *arg)
{
   char buffer[64];

   strcpy(buffer, arg);
   printf("The argument is: %s\n", buffer);
}


int main(int argc, char *argv[])
{
   char *res;

   /* Check args */
   if (argc != 2) {
      printf("Usage: %s argument\n", argv[0]);
      return 1;
   }
   
   printf("Hello r2con\n");
   dummy(argv[1]);

   return 0;
}

