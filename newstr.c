/*
 *  Library function to create new copies of strings.  Creates
 *  exactly the same size offered.
 *
 *  $RCSfile: newstr.c,v $	$Revision: 1.3 $
 *
 *  $Author: scs $	$Date: 1989/11/12 22:01:31 $
 *
 *  $State: Production $	$Locker:  $
 *
 *  $Log: newstr.c,v $
 *  Revision 1.3  1989/11/12 22:01:31  scs
 *  First production release.  Stripped all useless history and side-alleys.
 *
 */

#ifndef	lint
#ifndef	lib
static char	rcsid[] = "$Id: newstr.c,v 1.3 1989/11/12 22:01:31 scs Production $" ;
#endif
#endif

# include	<stdio.h>
# include	<stdc.h>

extern VOIDSTAR	malloc( PROTO_1( int ) ) ;
extern char*	strcpy( PROTO_2( char*, char* ) ) ;
extern int	strlen( PROTO_1( char* ) ) ;

/*
 *  Create a new string.  Return a pointer to it.  Return null
 *  if malloc fails, or if the incoming string is a null pointer.
 *  A zero-length string (ie, '\0' only) is a valid string, so
 *  we'll duplicate it.
 */

#ifdef	__STDC__
char*	NewString( register char* string )
#else
char*	NewString( string )
register char*	string ;
#endif
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
