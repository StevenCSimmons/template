/*
 *  Definitions file for template command.
 *
 *  $RCSfile: template.h,v $	$Revision: 0.6 $
 *
 *  $Author: scs $	$Date: 1989/11/12 22:01:53 $
 *
 *  $State: Production $	$Locker:  $
 *
 *  $Log: template.h,v $
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

extern char*	NewString( PROTO_1( char* ) ) ;
extern char*	NewNString( PROTO_2( char*, unsigned ) ) ;
extern void	Warning( PROTO_1( char* ) ) ;
extern void	Error( PROTO_1( char* ) ) ;
extern void	Fatal( PROTO_1( char* ) ) ;

#endif	/* of ifdef TEMPLATE_H */
