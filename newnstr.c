/*
 *  Library function to create a new string.
 *
 *  $RCSfile: newnstr.c,v $	$Revision: 1.4 $
 *
 *  $Author: scs $	$Date: 1989/12/09 15:11:52 $
 *
 *  $State: Exp $	$Locker:  $
 *
 *  $Log: newnstr.c,v $
 *  Revision 1.4  1989/12/09 15:11:52  scs
 *  Upgraded to new version of ANSI C compatibility macros.
 *
 *  Revision 1.3  89/11/12  22:01:28  scs
 *  First production release.  Stripped all useless history and side-alleys.
 *  
 */

#ifndef	lint
#ifndef	lib
static char	rcsid[] = "$Id: newnstr.c,v 1.4 1989/12/09 15:11:52 scs Exp $" ;
#endif
#endif

# include	<stdio.h>
# include	<stdc.h>

extern voidptr	malloc( PROTO_1( int ) ) ;
extern char*	strncpy( PROTO_3( char*, char*, int ) ) ;
extern int	strlen( PROTO_1( char* ) ) ;

/*
 *  Create a new string of a given length.  Copy the incoming string
 *  (if any) into it, up to the maximum length.  Return a NULL if 
 *  malloc fails.  Zero-length strings (ie, '\0' only) are permitted.
 */

char*	NewNString PARAM_2( register char*, string, register unsigned, len )
{
	register char*	new_string ;

	if ( NULL == ( new_string = malloc( len + 1 ) ) )
		return NULL ;
	(void) strncpy( new_string, string, (int) len ) ;
	return new_string ;
}
