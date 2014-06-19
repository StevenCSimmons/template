/*
 * Definitions file for template command.
 *
 * git id: $Id$
 */

#ifndef	TEMPLATE_H // prevent double includes
#define	TEMPLATE_H

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include	<errno.h>
#include	<sys/stat.h>
#include	<sys/param.h>
#include	<ctype.h>

typedef char	boolean;

#define	FALSE	(0)
#define	TRUE	(1)

#ifdef	MAIN
char*		ProgramName = NULL;
#else
extern char*	ProgramName;
#endif // of ifdef MAIN

#define	MAX_FILES	1024

extern char*	NewString( char* );
extern char*	NewNString( char*, size_t );
extern void	Warning( char const* );
extern void	Error( char const* );
extern void	Fatal( char const* );

#ifndef	NULL_CHAR
#define	NULL_CHAR '\0'
#endif

#endif // of ifdef TEMPLATE_H
