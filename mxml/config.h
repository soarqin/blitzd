/*
 * "$Id: config.h 387 2009-04-18 17:05:52Z mike $"
 *
 * Configuration file for Mini-XML, a small XML-like file parsing library.
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
 */

/*
 * Beginning with VC2005, Microsoft breaks ISO C and POSIX conformance
 * by deprecating a number of functions in the name of security, even
 * when many of the affected functions are otherwise completely secure.
 * The _CRT_SECURE_NO_DEPRECATE definition ensures that we won't get
 * warnings from their use...
 *
 * Then Microsoft decided that they should ignore this in VC2008 and use
 * yet another define (_CRT_SECURE_NO_WARNINGS) instead.  Bastards.
 */

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS


/*
 * Include necessary headers...
 */

#ifdef WIN32

#include <windows.h>

#define mxml_strtol _mxml_strtol
#define mxml_strcmp lstrcmpA
#define mxml_strcpy lstrcpyA
#define mxml_strlen lstrlenA
extern long _mxml_strtol(const char *, char **, int);

#define mxml_sprintf wsprintfA

#else

#define mxml_strtol strtol
#define mxml_strcmp strcmp
#define mxml_strcpy strcpy
#define mxml_strlen strlen
#define mxml_sprintf sprintf
#include <stdio.h>
#include <stdlib.h>

#endif

/*
 * Version number...
 */

#define MXML_VERSION "Mini-XML v2.6"


/*
 * Inline function support...
 */

#define inline _inline


/*
 * Long long support...
 */

#define HAVE_LONG_LONG 1


/*
 * Do we have the strXXX() functions?
 */

//#define HAVE_STRDUP 1


/*
 * Define prototypes for string functions as needed...
 */

#  ifndef HAVE_STRDUP
extern char	*_mxml_strdup(const char *);
#    define mxml_strdup _mxml_strdup
#  else
#    define mxml_strdup strdup
#  endif /* !HAVE_STRDUP */

/*
 * End of "$Id: config.h 387 2009-04-18 17:05:52Z mike $".
 */
