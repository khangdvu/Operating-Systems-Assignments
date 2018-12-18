#include "mystring.h"
#include <stdio.h>
#include <stdlib.h>
/*
 *   Implement the following functions: 
 * 
 *   You are NOT allowed to use any standard string functions such as 
 *   strlen, strcpy or strcmp or any other string function.
 */

/*
 *  mystrlen() calculates the length of the string s, 
 *  not including the terminating character '\0'.
 *  Returns: length of s.
 */
int mystrlen (const char *s) 
{
	int len = 0;
	int i = 0;
	while (s[i] != '\0')
	{
		len++;
		i++;
	}
	return len;
}

/*
 *  mystrcpy()  copies the string pointed to by src (including the terminating 
 *  character '\0') to the array pointed to by dst.
 *  Returns: a  pointer to the destination string dst.
 */
char  *mystrcpy (char *dst, const char *src)
{
	int i = mystrlen (src);
	int k = 0;
	while (k != (i))
	{
		dst[k]=src[k];
		k++;
	}
	return dst;
}

/*
 * mystrcmp() compares two strings alphabetically
 * Returns: 
 * 	-1 if s1  < s2
 *  	0 if s1 == s2
 *  	1 if s1 > s2
 */
int mystrcmp(const char *s1, const char *s2)
{
	int i=0;
	while (s1[i] != '\0')
	{
		if (s1[i] < s2[i]){
			return -1;
		}
		else if (s1[i] > s2[i]){
			return 1;
		}
		i++;
	}
	return 0;
}

/*
 * mystrdup() creates a duplicate of the string pointed to by s. 
 * The space for the new string is obtained using malloc.  
 * If the new string can not be created, a null pointer is returned.
 * Returns:  a pointer to a new string (the duplicate) 
 	     or null If the new string could not be created for 
	     any reason such as insufficient memory.
 */
char *mystrdup(const char *s1)
{
	char* dupdup = (char*) malloc(mystrlen(s1)* sizeof(char));
	mystrcpy(dupdup, s1);
	if (mystrcmp(dupdup, s1)==0) {return dupdup;}
	else
		return NULL;
}

