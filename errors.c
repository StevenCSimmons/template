/*
 *  Error handling routines
 *
 *  $RCSfile: errors.c,v $	$Revision: 0.8 $
 *
 *  $Author: scs $	$Date: 1989/12/09 15:12:02 $
 *
 *  $State: Exp $	$Locker:  $
 *
 *  $Log: errors.c,v $
 *  Revision 0.8  1989/12/09 15:12:02  scs
 *  Upgraded to new version of ANSI C compatibility macros.
 *
 *  Revision 0.7  89/11/12  22:01:11  scs
 *  First production release.  Stripped all useless history and side-alleys.
 *  
 */

#ifndef	lint
# ifndef lib
static char	rcsid[] = "$Id: errors.c,v 0.8 1989/12/09 15:12:02 scs Exp $" ;
# endif	/* of ifndef lib */
#endif	/* of ifndef lint */

# include	"template.h"

extern void	exit( PROTO_1( int ) ) ;

/*
 *  Print an error message on stderr and return.
 */

void	Error PARAM_1( const char* const, msg )
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

void	Fatal PARAM_1( const char* const, msg )
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

void	Warning PARAM_1( const char* const, msg )
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
