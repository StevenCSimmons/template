/*
 *  Definitions file for template command.
 *
 *  $RCSfile: template.h,v $	$Revision: 0.7 $
 *
 *  $Author: scs $	$Date: 1996/09/29 01:05:49 $
 *
 *  $State: Exp $	$Locker:  $
 *
 *  $Log: template.h,v $
 *  Revision 0.7  1996/09/29 01:05:49  scs
 *  Removed old stdc stuff, minor bug fixes for stdin-only mode.
 *
 *  Revision 0.6  1989/11/12 22:01:53  scs
 *  First production release.  Stripped all useless history and side-alleys.
 *
 */

#ifndef	TEMPLATE_H
#define	TEMPLATE_H

# include	<stdc.h>
# include	<stdio.h>

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
extern void	Warning( char* ) ;
extern void	Error( char* ) ;
extern void	Fatal( char* ) ;

#endif	/* of ifdef TEMPLATE_H */
