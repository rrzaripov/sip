/*
 * Wrappers around standard functions that use the heap.
 *
 * @BS_LICENSE@
 */


#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>

#include "sip.h"


static void nomem(void);


/*
 * Wrap malloc() and handle any errors.
 */

void *sipMalloc(size_t n)
{
	void *h;

	if ((h = malloc(n)) == NULL)
		nomem();

	return h;
}


/*
 * Wrap strdup() and handle any errors.
 */

char *sipStrdup(char *s)
{
	char *h;

	if ((h = strdup(s)) == NULL)
		nomem();

	return h;
}


/*
 * Return a string on the heap which is the concatonation of all the arguments.
 */

char *concat(char *s,...)
{
	char *sp, *new;
	size_t len;
	va_list ap;

	/* Find the length of the final string. */

	len = 1;
	va_start(ap,s);

	for (sp = s; sp != NULL; sp = va_arg(ap,char *))
		len += strlen(sp);

	va_end(ap);

	/* Create the new string. */

	new = sipMalloc(len);
	*new = '\0';

	va_start(ap,s);

	for (sp = s; sp != NULL; sp = va_arg(ap,char *))
		strcat(new,sp);

	va_end(ap);

	return new;
}


/*
 * Append a string to another that is on the heap.
 */

void append(char **s,char *new)
{
	if ((*s = realloc(*s,strlen(*s) + strlen(new) + 1)) == NULL)
		nomem();

	strcat(*s,new);
}


/*
 * Display a standard error message when the heap is exhausted.
 */

static void nomem(void)
{
	fatal("Unable to allocate memory on the heap\n");
}
