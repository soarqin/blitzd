/*
 * "$Id: mxml-string.c 387 2009-04-18 17:05:52Z mike $"
 *
 * String functions for Mini-XML, a small XML-like file parsing library.
 *
 * Copyright 2003-2009 by Michael Sweet.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Contents:
 *
 *   _mxml_strdup()    - Duplicate a string.
 *   _mxml_strtol() - Convert a string into integer.
 */

/*
 * Include necessary headers...
 */
#include "mxml.h"
#include "config.h"


/*
 * '_mxml_strdup()' - Duplicate a string.
 */

#ifndef HAVE_STRDUP
char 	*				/* O - New string pointer */
_mxml_strdup(const char *s)		/* I - String to duplicate */
{
  char	*t;				/* New string pointer */


  if (s == NULL)
    return (NULL);

  if ((t = mxml_malloc(mxml_strlen(s) + 1)) == NULL)
    return (NULL);

  return (mxml_strcpy(t, s));
}
#endif /* !HAVE_STRDUP */

#ifdef WIN32
long _mxml_strtol(
   const char *nptr,
   char **endptr,
   int base 
)
{
	long result;
	const char * p;
	if(base > 16 || base < 2)
		return 0;
	p = nptr;
	while(*p != 0  && (*endptr == NULL || p < *endptr))
	{
		if(*p != ' ' && *p != '\r' && *p != '\n')
			break;
		++ p;
	}
	if(base == 0)
	{
		if(p[0] == '0')
		{
			if(p[1] == 'x' || p[1] == 'X')
			{
				base = 16;
				p += 2;
			}
			else
			{
				base = 8;
				++ p;
			}
		}
	}
	result = 0;
	while(*p != 0  && (*endptr == NULL || p < *endptr))
	{
		int digit;
		if(*p >= '0' && *p <= '9')
		{
			digit = *p - '0';
		}
		else if(*p >= 'A' && *p <= 'F')
		{
			digit = *p - 'A' + 10;
		}
		else if(*p >= 'a' && *p <= 'f')
		{
			digit = *p - 'a' + 10;
		}
		else
		{
			break;
		}
		if(digit >= base)
		{
			break;
		}
		result = result * base + digit;
	}
	return result;
}
#endif

/*
 * End of "$Id: mxml-string.c 387 2009-04-18 17:05:52Z mike $".
 */
