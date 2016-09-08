#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main (int argc, char **argv) {
	if (argc < 2) {
		puts ("Usage echo <string>");
		return 0;
	}
	int len = strlen (argv[1]);
	char *str = malloc (len);
	strncpy (str, argv[1], len);
	printf ("%s\n", str);
	free (str);
	return 0;
}
