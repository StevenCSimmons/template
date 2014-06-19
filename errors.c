/*
 * Error handling routines
 */

#ifndef lint
#ifndef lib
static char     gitid[] = "$Id$";
#pragma unused(gitid)
#endif	// of ifndef lib
#endif	// of ifndef lint

#include    "template.h"
#include    "isdebug.h"

#ifdef DEBUGGING
#define MODULE_NAME "errors"
#endif // of ifdef DEBUGGING

/*
 * Print an error message on stderr and return.
 */

void    Error( const char* const msg ) {
    DEBUGTRACE("Error")
    (void) fprintf( stderr, "error: %s\n", msg );
    return;
}


/*
 * Print an error message on stderr and exit. Do
 * cleanups as appropriate.
 */

void    Fatal( const char* const msg ) {
    DEBUGTRACE("Fatal")
    (void) fprintf( stderr, "fatal error: %s\n", msg );
    exit( 1 );
    /* NOTREACHED */
    return;
}


/*
 * Print a warning message on stderr.
 */

void    Warning( const char* const msg ) {
    DEBUGTRACE("Warning")
    (void) fprintf( stderr, "warning: %s\n", msg );
    return;
}
