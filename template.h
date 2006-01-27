/*
 *  Definitions file for template command.
 *
 *  $RCSfile: template.h,v $	$Revision: 0.9 $
 *
 *  $Author: scs $	$Date: 2006/01/27 15:17:08 $
 *
 *  $State: Exp $	$Locker:  $
 *
 *  $Log: template.h,v $
 *  Revision 0.9  2006/01/27 15:17:08  scs
 *  Removed useless consts.
 *
 *  Revision 0.8  2006/01/27 15:01:16  scs
 *  Added a few things that are now ubiquitous in posix, tightened up
 *  usage of parameters in some cases.
 *
 *  Revision 0.7  1996/09/29 01:05:49  scs
 *  Removed old stdc stuff, minor bug fixes for stdin-only mode.
 *
 *  Revision 0.6  1989/11/12 22:01:53  scs
 *  First production release.  Stripped all useless history and side-alleys.
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
