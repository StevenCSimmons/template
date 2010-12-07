/*
 *  Definitions file for template command.
 */

#ifndef	TEMPLATE_H
#define	TEMPLATE_H

# include	<stdio.h>
# include	<stdlib.h>
# include	<string.h>
# include	<errno.h>

typedef char	boolean ;

# define	FALSE	(0)
# define	TRUE	(1)

#ifdef	MAIN
char*		ProgramName = NULL ;
#else
extern char*	ProgramName ;
#endif

# define	MAX_FILES	1024

extern char*	NewString( char* ) ;
extern char*	NewNString( char*, unsigned ) ;
extern void	Warning( char const* ) ;
extern void	Error( char const* ) ;
extern void	Fatal( char const* ) ;

#endif	/* of ifdef TEMPLATE_H */
