/*
 *  Error handling routines
 *
 *  $RCSfile: errors.c,v $	$Revision: 0.7 $
 *
 *  $Author: scs $	$Date: 1989/11/12 22:01:11 $
 *
 *  $State: Production $	$Locker:  $
 *
 *  $Log: errors.c,v $
 *  Revision 0.7  1989/11/12 22:01:11  scs
 *  First production release.  Stripped all useless history and side-alleys.
 *
 */

#ifndef	lint
# ifndef lib
static char	rcsid[] = "$Id: errors.c,v 0.7 1989/11/12 22:01:11 scs Production $" ;
# endif	/* of ifndef lib */
#endif	/* of ifndef lint */

# include	"template.h"

extern void	exit( PROTO_1( int ) ) ;

/*
 *  Print an error message on stderr and return.
 */

#ifdef	__STDC__
void	Error( const char* const msg )
#else
void	Error( msg )
const char* const	msg ;
#endif
{
#ifdef	DEBUGGING
	if ( ProgramName != NULL )
		(void) fprintf( stderr, "%s ", ProgramName ) ;
	if ( DBG_ModuleName != NULL )
		(void) fprintf( stderr, "(module %s) ", DBG_ModuleName ) ;
	if ( DBG_FunctionName != NULL )
		(void) fprintf( stderr, "[function %s] ", DBG_FunctionName ) ;
#endif
	(void) fprintf( stderr, "error: %s\n", msg ) ;
	return ;
}


/*
 *  Print an error message on stderr and exit.  Do
 *  cleanups as appropriate.
 */

#ifdef	__STDC__
void	Fatal( const char* const msg )
#else
void	Fatal( msg )
const char* const	msg ;
#endif
{
#ifdef	DEBUGGING
	if ( ProgramName != NULL )
		(void) fprintf( stderr, "%s ", ProgramName ) ;
	if ( DBG_ModuleName != NULL )
		(void) fprintf( stderr, "(module %s) ", DBG_ModuleName ) ;
	if ( DBG_FunctionName != NULL )
		(void) fprintf( stderr, "[function %s] ", DBG_FunctionName ) ;
#endif
	(void) fprintf( stderr, "fatal error: %s\n", msg ) ;
	exit( 1 ) ;
}


/*
 *  Print a warning message on stderr.
 */

#ifdef	__STDC__
void	Warning( const char* const msg )
#else
void	Warning( msg )
const char* const	msg ;
#endif
{
#ifdef	DEBUGGING
	if ( ProgramName != NULL )
		(void) fprintf( stderr, "%s ", ProgramName ) ;
	if ( DBG_ModuleName != NULL )
		(void) fprintf( stderr, "(module %s) ", DBG_ModuleName ) ;
	if ( DBG_FunctionName != NULL )
		(void) fprintf( stderr, "[function %s] ", DBG_FunctionName ) ;
#endif
	(void) fprintf( stderr, "warning: %s\n", msg ) ;
	return ;
}
