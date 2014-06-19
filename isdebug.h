/*
 * Definitions file for Inland Sea debugging standard
 *
 * TODOS:
 *   Move the definition of ProgramName here.
 *   See if the newer C standard define FILE and use instead of
 *     MODULE_NAME.
 *   See if the newer C standard define FUNCTION NAME and use
 *     instead of FUNCTION.
 *
 * git id: $Id$
 */

#ifndef	ISDEBUG_H // prevent double includes
#define	ISDEBUG_H

#ifdef DEBUGGING
    extern char* ProgramName;
#define	DEBUGTRACE(FUNCTION)	\
    if ( NULL != ProgramName ) { \
	(void) fprintf( stderr, "%s/", ProgramName ); \
    } \
    (void) fprintf( stderr, "%s/%s\n", MODULE_NAME, FUNCTION );
#else
#define	DEBUGTRACE(FUNCTION)
#endif 

#endif // of ifdef ISDEBUG_H
