/*
 *  Module to handle the template list.  Offers one entry
 *  point and two public variables.  The entry point is
 *  GetTemplList, which initializes the two public variables.
 *  The public variables are a list of template directories
 *  and a count of how many are in the list.
 *
 *  $RCSfile: templist.c,v $	$Revision: 0.14 $
 *
 *  $Author: scs $	$Date: 2001/11/30 18:23:18 $
 *
 *  $State: Exp $	$Locker:  $
 *
 *  $Log: templist.c,v $
 *  Revision 0.14  2001/11/30 18:23:18  scs
 *  Added bdcs home.
 *
 *  Revision 0.13  1996/09/29 01:05:49  scs
 *  Removed old stdc stuff, minor bug fixes for stdin-only mode.
 *
 *  Revision 0.12  1992/06/06 15:55:38  scs
 *  Made the HOME environment variable honored.
 *
 *  Revision 0.11  1990/10/14  11:21:05  scs
 *  Corrected usage of PARAM_0
 *
 *  Revision 0.10  89/12/09  15:12:11  scs
 *  Upgraded to new version of ANSI C compatibility macros.
 *  
 *  Revision 0.9  89/11/12  22:01:44  scs
 *  First production release.  Stripped all useless history and side-alleys.
 *  
 */

#ifdef	TEST
# define	MAIN
#endif	/* of ifdef TEST */

#ifndef	lint
# ifndef	lib
static char	rcsid[] = "$Id: templist.c,v 0.14 2001/11/30 18:23:18 scs Exp $" ;
# endif	/* of ifdef lib */
#endif	/* of ifdef lint */

# include	<stdio.h>
# include	<sys/param.h>
# include	<pwd.h>
# include	"template.h"

# define	MAX_TEMPLATE_DIRS	(64)

extern char*	strpbrk( char*, char* ) ;
extern char*	getenv( char* ) ;
extern int	access( char*, int ) ;
extern voidptr	malloc( unsigned ) ;
extern char*	getlogin() ;

extern int	errno ;
extern char*	sys_errlist[] ;

extern boolean	Verbose ;

static boolean			default_seen = FALSE ;
static const char* const	default_list = 
	"./.Templates:$HOME/.Templates:/afs/ans.net/group/bdcs/lib/Templates:/usr/local/lib/Templates" ;

/*	The public variables	*/

char*	Template_List[ MAX_TEMPLATE_DIRS + 1 ] ;
int	Max_Template_Dir = 0 ;


/*
 *  Dump the template list.  A debugging and error message aid.
 *  No parameters, no return value.
 */

static void	dump_template_list()
{
	register int	i, max ;

	(void) fprintf( stderr, "There are %d template directories in the list:\n", Max_Template_Dir ) ;
	for ( i = 0, max = Max_Template_Dir ; i < max ; i++ )
		(void) fprintf( stderr, " Dir #%2d:\t%s\n", i, Template_List[ i ] ) ;
}


/*
 *  Find the home directory of the given user.  If the user is
 *  NULL, assume it's the current user.  If you cannot determine
 *  the requested home, print an error message and bomb.  If you
 *  can, return a pointer to the home path.
 */

static char*	get_home( char* user )
{
	struct passwd*	pw_entry ;

	if ( user == NULL )
	{
		if ( NULL == ( user = getlogin() ) )
			Fatal( "Can't find your login id!" ) ;
	}
	else if ( *user == '\0' )
	{
		if ( NULL == ( user = getlogin() ) )
			Fatal( "Can't find your home directory!" ) ;
	}
	if ( NULL == ( pw_entry = getpwnam( user ) ) )
	{
		char	msgbuf[ 256 ] ;

		(void) sprintf( msgbuf, "Can't find home directory of user `%s'\n", user ) ;
		Fatal( msgbuf ) ;
	}
	return pw_entry->pw_dir ;
}


/*
 *  Interpret special stuff inside a string.  We allow three
 *  kinds of specials:
 *	$HOME	user's home directory
 *	  ~	user's home directory
 *      ~user   home directory of 'user'
 *
 *  Once all the stuff is copied, return a pointer to the newly
 *  interpreted string.
 */

static char*	interpret_specials( char* string )
{
	static char	dir_path[ MAXPATHLEN ] ;

	if ( NULL == string )
		Fatal( "Internal error -- null string in interpret_specials" ) ;
	if ( ( 0 == strncmp( string, "$HOME/", 6 ) ) ||
	     ( 0 == strcmp( string, "$HOME" ) ) )
	{
		(void) strcpy( dir_path, getenv( "HOME" ) ) ;
		(void) strcat( dir_path, string + 5 ) ;
		return dir_path ;
	}
	else if ( '~' == *string )
	{
		char	user_name_buf[ 24 ] ;
		char*	user_name = user_name_buf ;
		char*	sptr = string + 1 ;

		while ( ( *sptr != '/' ) && ( *sptr != '\0' ) )
			*user_name++ = *sptr++ ;
		*user_name = '\0' ;
		(void) strcpy( dir_path, get_home( user_name_buf ) ) ;
		(void) strcat( dir_path, sptr ) ;
		return dir_path ;
	}
	return string ;
}


/*
 *  Check the indicated directory for existance and accessibility.
 */

static boolean	valid_template_dir( char* dir )
{
	char*	path ;
	char	msgbuf[ BUFSIZ ] ;

	if ( dir == NULL )
		Fatal( "internal error in valid_template_dir - null path" ) ;
	if ( *dir == NULL )
		Fatal( "internal error in valid_template_dir - empty path" ) ;
	if ( NULL == ( path = NewNString( dir, (unsigned) ( strlen( dir ) + 2 ) ) ) )
		Fatal( "out of memory in valid_template_dir" ) ;
	(void) strcat( path, "/." ) ;
	if ( 0 == access( path, 04 + 01 ) )	/* read-execute mode */
		return TRUE ;
	if ( Verbose )
	{
		(void) sprintf( msgbuf, "can't access `%s' (%s)",
			dir, sys_errlist[ errno ] ) ;
		Warning( msgbuf ) ;
	}
	return FALSE ;
}


/*
 *  Take a pointer to a string and pick off a piece of it.  The
 *  piece should either be terminated by end-of-string or by a
 *  colon.  Allocate a new string, copy the newfound one into
 *  it.  Adjust the incoming pointer to skip over the string we
 *  return.
 *
 *  Parameters:
 *    instr:	Address of pointer to string.
 *
 *  Return:
 *    Address of newly allocated string.
 */

static char*	get_next_colon( char** instr )
{
	char*		oldstr ;
	char*		end ;
	unsigned	len ;
	static char	strbuf[ MAXPATHLEN ] ;

	if ( instr == NULL )
		return NULL ;
	if ( *instr == NULL )
		return NULL ;
	end = strpbrk( *instr, ":" ) ;
	/* Was this the last entry? */
	if ( end == NULL )
	{
		oldstr = *instr ;
		*instr = end ;
		return oldstr ;
	}
	if ( *end == NULL )
	{
		oldstr = *instr ;
		*instr = end ;
		return oldstr ;
	}
	/* OK, we must have hit a colon */
	len = end - *instr ;
	(void) strncpy( strbuf, *instr, (int) len ) ;
	strbuf[ len ] = '\0' ;
	*instr = end + 1 ;
	return strbuf ;
}


/*
 *  Process a directory list string.  Take a list of directories, which
 *  come in a colon-separated strings.  Check each one for existance,
 *  readability, and being a directory.  If OK, put it in the list.
 *  If not OK, print an error message.  Special case: if the directory
 *  is named "DEFAULT", insert the system list at that point.  Note
 *  that each directory should be checked for '$HOME', for simple '~',
 *  and for '~name' usage.
 *
 *  Incoming parameters:
 *	templ_list:	colon list of template directories
 *
 *  Returns
 *	if default was processed or not.
 */

static void	process_list( char* templ_list )
{
	char*	dir ;
	char*	def_list = default_list ;
	char*	tmp_list = templ_list ;

	if ( templ_list == NULL )
		return ;
	if ( *templ_list == NULL )
		return ;
	while ( NULL != ( dir = get_next_colon( &tmp_list ) ) )
	{
		if ( 0 == strcmp( "DEFAULT", dir ) )
		{
			if ( default_seen )
			{
				char buf[ BUFSIZ ] ;
				(void) sprintf( buf, "Attempt to double-include DEFAULT from %s.", templ_list ) ;
				Warning( buf ) ;
			}
			else
			{
				(void) process_list( def_list ) ;
				default_seen = TRUE ;
			}
		}
		else if ( Max_Template_Dir == MAX_TEMPLATE_DIRS )
		{
			dump_template_list() ;
			Fatal( "Too many template dirs to search!" ) ;
		}
		else
		{
			dir = interpret_specials( dir ) ;
			if ( valid_template_dir( dir ) )
				Template_List[ Max_Template_Dir++ ] = NewString( dir ) ;
		}
	}
	return ;
}


/*
 *  Create the template list.  Grab data from the switches,
 *  from the environment, and from the default.  Use them to
 *  build an array of template directory names.  Do some
 *  basic error checking and warnings.  As you read the list,
 *  interpret $HOME and ~ references.
 *
 *  Parameters:
 *     user_list: list of user-supplied directories.  If NULL,
 *		means user specified none.
 *
 *  Returns:
 *     void
 */

void	GetTemplList( const char* const user_list )
{
	char*	environment_list = getenv( "TEMPLATES" ) ;

	if ( user_list == NULL )
	{
		if ( environment_list == NULL )
			(void) process_list( default_list ) ;
		else
			(void) process_list( environment_list ) ;
	}
	else
		(void) process_list( user_list ) ;
	if ( Max_Template_Dir == 0 )
		Fatal( "Could not find any template directories!" ) ;
}

#ifdef	TEST

static void	testit( char* list, char* comment )
{
	Max_Template_Dir = 0 ;
	default_seen = FALSE ;
	(void) printf( "Using list " ) ;
	if ( list == NULL )
		(void) printf( "NULL\n" ) ;
	else
		(void) printf( "`%s'\n", list ) ;
	(void) printf( "%s\n", comment ) ;
	GetTemplList( list ) ;
	dump_template_list() ;
}


boolean	Verbose = TRUE ;

int main( int argc, char** argv )
{
	ProgramName = "test_templist" ;
	(void) printf( "Before startup.\n" ) ;
	dump_template_list() ;

	testit( NULL, "Expect $TEMPLATE if not null, otherwise default" ) ;
	testit( ".,bar,baz", "Expect ., bar, and baz" ) ;
	testit( ".,DEFAULT,bar,baz", "Expect ., DEFAULT, bar, and baz" ) ;
	testit( ".,DEFAULT,bar,DEFAULT,baz", "Expect ., DEFAULT, bar, and baz, plus a warning" ) ;
	testit( "", "Forcing empty list -- expect fatal error" ) ;
}
#endif
