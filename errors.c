/*
 *  Error handling routines
 */

#ifndef    lint
# ifndef lib
static char    gitid[] = "$Id$";
#pragma unused(gitid)
# endif    /* of ifndef lib */
#endif    /* of ifndef lint */

# include    "template.h"

/*
 *  Print an error message on stderr and return.
 */

void    Error( const char* const msg ) {
#ifdef    DEBUGGING
    if ( ProgramName != NULL )
        (void) fprintf( stderr, "%s ", ProgramName );
    if ( DBG_ModuleName != NULL )
        (void) fprintf( stderr, "(module %s) ", DBG_ModuleName );
    if ( DBG_FunctionName != NULL )
        (void) fprintf( stderr, "[function %s] ", DBG_FunctionName );
#endif
    (void) fprintf( stderr, "error: %s\n", msg );
    return;
}


/*
 *  Print an error message on stderr and exit.  Do
 *  cleanups as appropriate.
 */

void    Fatal( const char* const msg ) {
#ifdef    DEBUGGING
    if ( ProgramName != NULL )
        (void) fprintf( stderr, "%s ", ProgramName );
    if ( DBG_ModuleName != NULL )
        (void) fprintf( stderr, "(module %s) ", DBG_ModuleName );
    if ( DBG_FunctionName != NULL )
        (void) fprintf( stderr, "[function %s] ", DBG_FunctionName );
#endif
    (void) fprintf( stderr, "fatal error: %s\n", msg );
    exit( 1 );
    /* NOTREACHED */
    return;
}


/*
 *  Print a warning message on stderr.
 */

void    Warning( const char* const msg ) {
#ifdef    DEBUGGING
    if ( ProgramName != NULL )
        (void) fprintf( stderr, "%s ", ProgramName );
    if ( DBG_ModuleName != NULL )
        (void) fprintf( stderr, "(module %s) ", DBG_ModuleName );
    if ( DBG_FunctionName != NULL )
        (void) fprintf( stderr, "[function %s] ", DBG_FunctionName );
#endif
    (void) fprintf( stderr, "warning: %s\n", msg );
    return;
}
