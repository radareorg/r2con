#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main (int argc, char **argv) {
	if (argc < 2) {
		puts ("Usage echo <string>");
		return 0;
	}
	int i;
	for (i = 1; i < argc; i++) {
		int len = strlen (argv[i]);
		char *str = malloc (len);
		strncpy (str, argv[i], len);
		printf ("%s ", str);
		free (str);
	}
	printf("\b\n");
	return 0;
}
