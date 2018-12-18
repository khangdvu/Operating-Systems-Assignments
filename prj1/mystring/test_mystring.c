#include "mystring.h"
#include <assert.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * The following are simple tests to help you.
 * You should develop more extensive tests to try and find bugs.
 */
int main()
{
	const int SIZE = 100;
	char buffer[SIZE];

	assert(mystrlen("cmpt 300") == 8);
	assert(mystrlen("Google Chrome") == 13);

	mystrcpy(buffer, "I am testing my string functions!");
	assert(strcmp(buffer, "I am testing my string functions!") == 0);


	assert(mystrcmp ("I love coding", "I love coding") == 0);
	assert(mystrcmp ("I love coding", "I love cats") == 1);
	assert(mystrcmp ("I love coding", "I love cookies") == -1);

	char *dupStr = mystrdup(buffer);
	assert (!strcmp(buffer, dupStr));
	assert (buffer != dupStr);

	if (dupStr != NULL)
		free (dupStr);

	printf ("\nPassed!\n");

	return 0;
}
