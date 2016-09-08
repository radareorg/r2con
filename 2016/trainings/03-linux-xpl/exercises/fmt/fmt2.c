#include <stdio.h>
#include <string.h>


char *shellargs[] = { "/bin/sh", 0 };


void spawnshell()
{
   execv(shellargs[0], shellargs);
}

int main(int argc, char** argv)
{
    char buffer[4096];

    strncpy(buffer, argv[1], sizeof(buffer));
    printf(buffer);
    printf("\nDone.\n");

    //spawnshell();

    return 0;
}


