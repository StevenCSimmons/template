/*
 *  This module creates the requested template.
 *
 *  $RCSfile: mktempl.c,v $	$Revision: 0.20 $
 *
 *  $Author: scs $	$Date: 2003/04/14 14:48:07 $
 *
 *  $State: Exp $	$Locker:  $
 *
 *  $Log: mktempl.c,v $
 *  Revision 0.20  2003/04/14 14:48:07  scs
 *  Updated tests to reflect newer switches.
 *
 *  Revision 0.19  1996/09/28 23:23:55  scs
 *  Removed all PROTO/PARAM items, first pass towards fixing core dump
 *  when stdout used without name but with forced extension.
 *
 *  Revision 0.18  1992/06/06 15:54:32  scs
 *  Added handling for stdout.
 *
 *  Revision 0.17  90/10/30  11:00:34  scs
 *  Added check for empty as well as null file names.
 *  
 *  Revision 0.16  90/10/14  11:35:48  scs
 *  Corrected usage of PARAM_0/PROTO_0
 *  
 *  Revision 0.15  89/12/09  15:30:51  scs
 *  Cahnges to use ANSI C compatability macros, first pass at fileless output.
 *  
 *  Revision 0.14  89/11/12  22:01:22  scs
 *  First production release.  Stripped all useless history and side-alleys.
 */

#ifdef	TEST
# define	MAIN
#endif	/* of ifdef TEST */

#ifndef	lint
# ifndef	lib
static char	rcsid[] = "$Id: mktempl.c,v 0.20 2003/04/14 14:48:07 scs Exp $" ;
# endif	/* of ifndef lib */
#endif	/* of ifndef lint */

# include	"template.h"
# include	<sys/param.h>

# define	MAX_NAMES	(3)

extern char*	strrchr( char*, char ) ;
extern int	access( char*, int ) ;
extern void	free( char* ) ;

#ifndef	BSD
extern int	sprintf( char*, char* ) ;
#endif

extern int	errno ;

extern void	CreateTarget( char*, char* ) ;
extern char*	strcat( char*, char* ) ;
extern char*	strncpy( char*, char*, int ) ;
extern void	exit( int ) ;

extern char*	Template_List[] ;

static char*	possibilities[ MAX_NAMES ] = { NULL, NULL, NULL } ;

extern boolean	Verbose ;
extern boolean	NoAction ;
extern boolean	UsingStdout ;
extern boolean	ForceExtension ;

char*	UserExtension ;

/*
 *  Check to see if there is a readable file with the name
 *  "dir/file".  Return null if not, otherwise return a pointer
 *  to the full path.
 */

static char*	check_for_file( char* dir, char* file )
{
	static char	template_path[ MAXPATHLEN ] ;

	if ( dir == NULL )
		Fatal( "internal error in check_for_file - null path" ) ;
	if ( *dir == NULL )
		Fatal( "internal error in check_for_file - empty path" ) ;
	if ( file == NULL )
		Fatal( "internal error in check_for_file - null file" ) ;
	if ( *file == NULL )
		Fatal( "internal error in check_for_file - empty file" ) ;
	(void) sprintf( template_path, "%s/%s", dir, file ) ;
	errno = 0 ;
	if ( 0 == access( template_path, 04 ) )
		return template_path ;
	errno = 0 ;
	return NULL ;
}


/*
 *  Use the list of possible template directories with the
 *  list of possible file names and find the first match.
 */

static char*	get_template_file()
{
	char**	name ;
	char**	dir ;
	char*	file ;
	register int	i ;

	for ( name = possibilities, i = 0 ; i < MAX_NAMES ; name++, i++ )
	{
		for ( dir = Template_List ; *dir != NULL ; dir++ )
		{
			if ( name != NULL )
			{
				if ( *name != NULL )
				{
					file = check_for_file( *dir, *name ) ;
					if ( file != NULL )
						return file ;
				}
			}
		}
	}
	return NULL ;
}


/*
 *  Given an incoming string, generate a list of possible
 *  template file names.  There two sets of rules, depending
 *  on if the user forced an extension (-e) or not.
 *
 *  If the user forced an extension, we use the following
 *  rules to build names:
 *
 *  1	Try the original file name with the extension added.
 *
 *  2	If there is an extension on the file, replace the file
 *	extension with the user-forced one.
 *
 *  3	Use the user-forced extension by itself.
 *
 *  If the user did not force an extension, use the following
 *  rules:
 *
 *  1	Try the full name of the file.
 *
 *  2	If there is an extension on the file, use the extension
 *	by itself.
 *
 *  Incoming parameters
 *	in_name -	The name we're using as a source
 *
 *  Returns:
 *	nothing
 */

static void	generate_names( char* in_name )
{
	char*	suffix ;	/* an extension */
	int	try_number ;
	int	suffix_len ;
	int	len ;
	char	stem[ MAXPATHLEN ] ;

	if ( !ForceExtension )
	{
		if ( in_name == NULL )
			Fatal( "Got a NULL name in generate_names!" ) ;
		else if ( *in_name == NULL )
			Fatal( "Got an empty name in generate_names!" ) ;
	}

	/* Clear out the old names */

	for ( try_number = 0 ; try_number < MAX_NAMES ; try_number++ )
		if ( possibilities[ try_number ] != NULL )
		{
			free( possibilities[ try_number ] ) ;
			possibilities[ try_number ] = NULL ;
		}

	if ( ForceExtension )
	{
		/* Force the extension onto the file name */

		suffix = UserExtension ;
		suffix_len = strlen( UserExtension ) + 1 ;
		len = ( strlen( in_name ) + suffix_len ) ;
		possibilities[ 0 ] = NewNString( in_name, (unsigned) len ) ;
		if ( NULL == possibilities[ 0 ] )
			Fatal( "Ran out of memory in generate_names" ) ;
		(void) strcat( possibilities[ 0 ], "." ) ;
		(void) strcat( possibilities[ 0 ], UserExtension ) ;

		/* Now do extension replacement if possible */

		suffix = strrchr( in_name, '.' ) ;
		if ( NULL != suffix )
		{
			len = suffix - in_name ;
			(void) strncpy( stem, in_name, len ) ;
			possibilities[ 1 ] = NewNString( stem,
				(unsigned) ( len + suffix_len ) ) ;
			if ( NULL == possibilities[ 1 ] )
				Fatal( "Ran out of memory in generate_names" ) ;
			(void) strcat( possibilities[ 1 ], "." ) ;
			(void) strcat( possibilities[ 1 ], UserExtension ) ;
			try_number = 2 ;
		}
		else
			try_number = 1 ;

		/* Now just use the forced extension */

		possibilities[ try_number ] = NewString( UserExtension ) ;
		if ( NULL == possibilities[ try_number ] )
			Fatal( "Ran out of memory in generate_names" ) ;
	}
	else
	{
		/* Start with just the incoming name */

		if ( NULL == ( possibilities[ 0 ] = NewString( in_name ) ) )
			Fatal( "Ran out of memory in generate_names" ) ;
		suffix = strrchr( in_name, '.' ) ;
		if ( NULL != suffix )
		{
			if ( strlen( suffix ) > 1 )
			{
				suffix++ ;
				if ( NULL == ( possibilities[ 1 ] = NewString( suffix ) ) )
					Fatal( "Ran out of memory in generate_names" ) ;
			}
		}
	}
	if ( Verbose )
	{
		(void) fputs( "Possible template names:", stderr ) ;
		for ( try_number = 0 ; ( try_number < MAX_NAMES ) && ( possibilities[ try_number ] != NULL ) ; try_number++ )
		{
			(void) fputs( " `", stderr ) ;
			(void) fputs( possibilities[ try_number ], stderr ) ;
			(void) putc( '\'', stderr ) ;
		}
		(void) putc( '\n', stderr ) ;
	}
	return ;
}


/*
 *  Special module for the purpose of copying stdin to stdout.
 *  Nothing fancy or efficient, just do it.
 */

static void	pass_thru()
{
	int	inchar ;

	while ( EOF != ( inchar = getchar() ) )
		(void) putchar( inchar ) ;
}



/*
 *  Central execution for this module.  If given a filename, do the
 *  following for each file:
 *    Generate the valid template names
 *    Search the template directories for the valid names.
 *    If found, create the template.
 *    Otherwise give a warning error and do nothing with
 *      the request.
 *  If no file name, check to make sure that we've got stdout requested
 *  and the user has forced an extension.
 */

static void	proc_file( char* request )
{
	char*	template_file ;
	char	msgbuf[ BUFSIZ ] ;

	generate_names( request ) ;
	if ( NULL == ( template_file = get_template_file() ) )
	{
		char*	format ;

		if ( possibilities[ 1 ] != NULL )
			format = "No template for `%s'.  Wanted `%s' (full) or `%s' (derived)." ;
		else
			format = "No template for `%s'.  Wanted `%s' (full)." ;
		(void) sprintf( msgbuf, format, request,
			possibilities[ 0 ], possibilities[ 1 ] ) ;
		Warning( msgbuf ) ;
	}
	else
	{
		if ( NoAction )
		{
			(void) fputs( "Would create result from ", stderr ) ;
			(void) fputs( template_file, stderr ) ;
			if ( UsingStdout )
			{
				(void) fputs( " and ", stderr ) ;
				(void) fputs( request, stderr ) ;
				(void) fputs( " on standard out.\n", stderr ) ;
			}
			else
			{
				(void) fputs( " on file ", stderr ) ;
				(void) fputs( request, stderr ) ;
				(void) fputs( ".\n", stderr ) ;
			}
		}
		else
			CreateTarget( template_file, request ) ;
	}
}


/*
 *  Mainline for this module.  Given a list of files, do the
 *  following for each file:
 *    Generate the valid template names
 *    Search the template directories for the valid names.
 *    If found, create the template.
 *    Otherwise give a warning error and do nothing with
 *      the request.
 */

void	ProcessFiles( char** request_list )
{
	char**	request ;

	if ( request_list == NULL )
	{
		(void) fputs( "Internal error in ProcessFiles -- NULL request list.\n", stderr ) ;
		exit( 0 ) ;
	}
	if ( *request_list == NULL )
	{
		if ( ! ForceExtension )
		{
			/*
			 * Really, this is an error.  The user has asked
			 * to read from stdin, write to stdout, and given
			 * no extension or filename.  But since we're in
			 * a pipe, we should be good and pass the data
			 * thru unchanged.
			 */
			pass_thru() ;
		}
		else
			proc_file( NULL ) ;
	}
	else for ( request = request_list ; *request != NULL ; request++ )
		proc_file( *request ) ;
}

#ifdef	TEST

boolean	Verbose = TRUE ;
boolean ForceExtension = FALSE;
boolean NoAction = FALSE ;
boolean UsingStdout = FALSE ;

char*	list[] = { "foo", "bar.c", "baz.", NULL } ;
char*	Template_List[] = {
	"/usr/local/lib/Templates", "./.Templates", ".", NULL } ;

void	CreateTarget( char* template, char* target )
{
	(void) printf( "CreateTarget: would make `%s' from `%s'\n",
		target, template ) ;
}

int main( int argc, char** argv )
{
	ProgramName = argv[ 0 ] ;
	Verbose = TRUE ;
	if ( argc != 1 )
		Warning( "This test ignores args." ) ;
	ProcessFiles( list ) ;
}
#endif /* of ifdef TEST */
