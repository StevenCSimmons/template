/*
 *  Library function to create a new string.
 *
 *  $RCSfile: newnstr.c,v $	$Revision: 1.3 $
 *
 *  $Author: scs $	$Date: 1989/11/12 22:01:28 $
 *
 *  $State: Production $	$Locker:  $
 *
 *  $Log: newnstr.c,v $
 *  Revision 1.3  1989/11/12 22:01:28  scs
 *  First production release.  Stripped all useless history and side-alleys.
 *
 */

#ifndef	lint
#ifndef	lib
static char	rcsid[] = "$Id: newnstr.c,v 1.3 1989/11/12 22:01:28 scs Production $" ;
#endif
#endif

# include	<stdio.h>
# include	<stdc.h>

extern VOIDSTAR	malloc( PROTO_1( int ) ) ;
extern char*	strncpy( PROTO_3( char*, char*, int ) ) ;
extern int	strlen( PROTO_1( char* ) ) ;

/*
 *  Create a new string of a given length.  Copy the incoming string
 *  (if any) into it, up to the maximum length.  Return a NULL if 
 *  malloc fails.  Zero-length strings (ie, '\0' only) are permitted.
 */

#ifdef	__STDC__
char*	NewNString( register char* string, register unsigned len )
#else
char*	NewNString( string, len )
register char*		string ;
register unsigned	len ;
#endif
{
	register char*	new_string ;

	if ( NULL == ( new_string = malloc( len + 1 ) ) )
		return NULL ;
	(void) strncpy( new_string, string, (int) len ) ;
	return new_string ;
}
