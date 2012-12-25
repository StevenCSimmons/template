/*
 *  Library function to create a new string.
 */

#ifndef    lint
#ifndef    lib
static char    gitid[] = "$Id$";
#pragma unused(gitid)
#endif
#endif

# include    <stdio.h>
# include    <string.h>
# include    <stdlib.h>

extern void*    malloc( size_t );

/*
 *  Create a new string of a given length.  Copy the incoming string
 *  (if any) into it, up to the maximum length.  Return a NULL if 
 *  malloc fails.  Zero-length strings (ie, '\0' only) are permitted.
 */

char*    NewNString( register char* string, register unsigned len ) {
    register char*    new_string;

    if ( NULL == ( new_string = malloc( len + 1 ) ) )
        return NULL;
    strncpy( new_string, (const char*) string, (size_t) len );
    return new_string;
}
