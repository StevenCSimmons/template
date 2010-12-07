/*
 *  Library function to create new copies of strings.  Creates
 *  exactly the same size offered.
 */

#ifndef	lint
#ifndef	lib
static char	gitid[] = "$Id$";
#endif
#endif

# include	<stdio.h>
# include	<string.h>

extern void*	malloc( int ) ;

/*
 *  Create a new string.  Return a pointer to it.  Return null
 *  if malloc fails, or if the incoming string is a null pointer.
 *  A zero-length string (ie, '\0' only) is a valid string, so
 *  we'll duplicate it.
 */

char*	NewString( register char* string )
{
	register unsigned	len ;
	register char*		new_string ;

	if ( string == NULL )
		return NULL ;
	len = strlen( string ) ;
	if ( NULL == ( new_string = malloc( len + 1 ) ) )
		return NULL ;
	(void) strcpy( new_string, string ) ;
	return new_string ;
}
