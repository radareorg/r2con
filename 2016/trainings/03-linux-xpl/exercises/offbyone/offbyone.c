/*
 * This binary requires stack-boundary to 2 to keep stack aligned to 2^n
 *
 * $ gcc -mpreferred-stack-boundary=2 offbyone.c -m32 -o offbyone
 */

#include <stdio.h>
#include <string.h>

void trick(void)
{
   __asm__("jmp *%esp");
}

void dummy(char *arg)
{
   char buffer[256];

   memset(buffer, 0, sizeof(buffer));
   strncat(buffer, arg, sizeof(buffer));
   printf("El argumento es: %s\n", buffer);
}


int main(int argc, char *argv[])
{
   char *res;

   /* Check args */
   if (argc != 2) {
      printf("Usage: %s argumento\n", argv[0]);
      return 1;
   }
   
   printf("Ola k ase\n");
   dummy(argv[1]);

   return 0;
}

