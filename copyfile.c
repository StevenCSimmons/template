/*
 *  Build an actual file from a template and source.  Module has
 *  two entry points -- an initializer InitCopying(), and a service
 *  provider CreateTarget().
 *
 *  $RCSfile: copyfile.c,v $	$Revision: 0.14 $
 *
 *  $Author: scs $	$Date: 1990/10/14 22:36:08 $
 *
 *  $State: Exp $	$Locker:  $
 *
 *  $Log: copyfile.c,v $
 *  Revision 0.14  1990/10/14 22:36:08  scs
 *  Fixed bug with stdout and unmodifiable files.
 *
 *  Revision 0.13  90/10/14  10:51:12  scs
 *  Converted to new parameters format.
 *  
 *  Revision 0.12  90/07/15  17:47:09  scs
 *  Added POSIX umask definition.
 *  
 *  Revision 0.11  89/12/09  15:11:34  scs
 *  Upgraded to new version of ANSI C compatibility macros.
 *  
 *  Revision 0.10  89/11/12  22:00:47  scs
 *  First production release.  Stripped all useless history and side-alleys.
 *  
 */

#ifdef	TEST
# define	MAIN
#endif

#ifndef	lint
# ifndef	lib
static char	rcsid[] = "$Id: copyfile.c,v 0.14 1990/10/14 22:36:08 scs Exp $" ;
# endif	/* of ifndef lib */
#endif	/* of ifndef lint */

# include	"template.h"
# include	<fcntl.h>

# define	TEMPFILE	"/tmp/tmplXXXXXX"
# define	IO_CHUNK	( BUFSIZ * 8 )

extern char*	mktemp( PROTO_1( char* ) ) ;
extern char*	strcpy( PROTO_2( char*, char* ) ) ;
extern long	lseek( PROTO_3( int, long, int ) ) ;
extern int	open( PROTO_3( char*, int, int ) ) ;
extern int	read( PROTO_3( int, char*, unsigned ) ) ;
extern int	write( PROTO_3( int, char*, unsigned ) ) ;
extern int	close( PROTO_1( int ) ) ;
extern int	unlink( PROTO_1( char* ) ) ;

#ifdef	POSIX
extern mode_t	umask( PROTO_1( int ) ) ;
#else
extern int	umask( PROTO_1( int ) ) ;
#endif

#ifndef	BSD
extern int	sprintf( PROTO_2PL( char*, char* ) ) ;
#endif

extern int	errno ;

extern int	errno ;
extern char*	sys_errlist[] ;

extern boolean	Verbose ;
extern boolean	UsingStdout ;

static int	tempfile = -1 ;		/* fd for writing tempfile */
static int	template = -1 ;		/* fd for reading template */
static int	target = -1 ;		/* fd for read/write target */
static char	tempfile_name[ sizeof( TEMPFILE ) + 1 ] ;
static int	file_mask = 0600 ;	/* file creation mask */

static char	message[ 512 ] ;


/*
 *  Module-wide cleanup.  Close all the fds, delete the
 *  tempfile, reset all the variables to initial state.
 *  Then check to see if there is an error message pending.
 *  If so, print it and die.  No parameters, no return value.
 */

static void	module_cleanup PARAM_0
{
	(void) close( template ) ;
	(void) close( tempfile ) ;
	(void) close( target ) ;
	tempfile = template = target = -1 ;
	(void) unlink( tempfile_name ) ;
	if ( message[ 0 ] != '\0' )
		Fatal( message ) ;
	return ;
}


/*
 *  Initialize a file.  Try to open it in the mode requested,
 *  and return a fd for the opened file.  Print nice error
 *  messages should something go wrong.
 *
 *  Incoming parameters:
 *	name	- path of file to be opened
 *	mode	- how we are to open it
 *	detail	- handy string to use in error messages
 *
 *  Returns
 *	an opened fd if successful, -1 if not
 */

static int	open_file PARAM_3( char*, name, int, mode, char*, detail )
{
	int	fd ;

	if ( -1 == ( fd = open( name, mode, file_mask ) ) )
	{
		(void) sprintf( message,
			"Couldn't open (%s) file `%s': %s",
			detail, name, sys_errlist[ errno ] ) ;
		module_cleanup() ;
	}
	return fd ;
}


/*
 *  Given two open fds, copy the second file to the first.
 *  Assume the file position pointer (as returned by lseek)
 *  has already been set.  Use big buffers -- we should make
 *  this as fast as possible.  If any error occur, print a
 *  detailed message and return false immediately.
 *
 *  Incoming parameters:
 *	to   - fd for the file to be copied to
 *	from - fd for the file to be copied from
 *
 *  Returns
 *	TRUE/FALSE, if succeeded or failed.
 */

static boolean	copyfile PARAM_2( int, to, int, from )
{
	char	buffer[ IO_CHUNK ] ;
	int	inlen = IO_CHUNK ;
	int	outlen ;

	for ( ; ; )
	{
		errno = 0 ;
		inlen = read( from, buffer, (unsigned) inlen ) ;
		if ( inlen == 0 )
			return TRUE ;
		else if ( inlen == -1 )
			return FALSE ;
		errno = 0 ;
		outlen = write( to, buffer, (unsigned) inlen ) ;
		if ( ( outlen == -1 ) || ( inlen != outlen ) )
			return FALSE ;	/* write error */
	}
}



/*
 *  Main entry point for module.  It is expected this will be
 *  called repeatedly, so be neat and clean with initializations.
 *
 *  Given the name of a template and a target file, create a copy
 *  of the file with the template included.  Algorithm is:
 *    Open a tempfile read-write, the target read-write-create
 *	but not truncate, and the template read-only.
 *    Copy the template to the tempfile.
 *    Copy the target to the tempfile, appending.
 *    Reset the target to beginning of file.
 *    Copy the tempfile into the target.
 *    Delete the tempfile.
 *  Bitch mightily and die about any errors along the way.
 */

void	CreateTarget PARAM_2( char*, template_name, char*, target_name )
{
	message[ 0 ] = '\0' ;
	template = open_file( template_name, O_RDONLY, "read" ) ;
	if ( UsingStdout )
	{
		if ( Verbose )
		{
			(void) fprintf( stderr, "Will copy `%s' and `%s' to stdout.\n",
				template_name, target_name ) ;
		}
		if ( ! copyfile( 2, template ) )
			(void) sprintf( message,
				"Error in copying `%s' to standard out: %s",
				template_name, sys_errlist[ errno ] ) ;
		if ( target_name != NULL )
		{
			if ( *target_name != NULL )
			{
				target = open_file( target_name, O_RDONLY, "read" ) ;
				if ( ! copyfile( 2, target ) )
				{
					(void) sprintf( message,
						"Error in copying `%s' to stdout: %s",
						target_name, sys_errlist[ errno ] ) ;
				}
			}
		}
	}
	else	/* copying to file, not stdout */
	{
		if ( Verbose )
			(void) fprintf( stderr, "Creating/rewriting `%s' with template `%s'.\n",
				target_name, template_name ) ;
		tempfile = open_file( tempfile_name, ( O_CREAT | O_RDWR | O_EXCL ), "create" ) ;
		target = open_file( target_name, ( O_RDWR | O_CREAT ), "modify" ) ;
		if ( ! copyfile( tempfile, template ) )
		{
			(void) sprintf( message,
				"Error in copying `%s' to `%s': %s",
				target_name, tempfile_name, sys_errlist[ errno ] ) ;
		}
		else if ( ! copyfile( tempfile, target ) )
		{
			(void) sprintf( message,
				"Error in copying `%s' to `%s': %s",
				tempfile_name, target_name, sys_errlist[ errno ] ) ;
		}
		else if ( 0 != lseek( tempfile, 0L, 0 ) )
		{
			(void) sprintf( message,
				"Could not reset `%s' to beginning of file: %s",
				tempfile_name, sys_errlist[ errno ] ) ;
		}
		else if ( 0 != lseek( target, 0L, 0 ) )
		{
			(void) sprintf( message,
				"Could not reset `%s' to beginning of file: %s",
				target_name, sys_errlist[ errno ] ) ;
		}
		else if ( ! copyfile( target, tempfile ) )
		{
			(void) sprintf( message,
				"Error in copying `%s' to `%s': %s",
				target_name, tempfile_name, sys_errlist[ errno ] ) ;
		}
	}
	module_cleanup() ;
	return ;
}


/*
 *  Initialization entry point for module.  Sets up variables
 *  that there's no point in rebuilding every time the module
 *  is entered.
 */

void	InitCopying PARAM_0
{
	int	old_umask = umask( 0 ) ;

	(void) strcpy( tempfile_name, TEMPFILE ) ;
	(void) mktemp( tempfile_name ) ;

	(void) umask( old_umask ) ;
	file_mask = ( ( ~old_umask ) & 0666 ) ;
}

#ifdef	TEST

boolean	Verbose = TRUE ;

int main PARAM_2( int, argc, char*, argc[] )
{
	ProgramName = argv[ 0 ] ;
	Verbose = TRUE ;
	if ( argc != 1 )
		Warning( "This test ignores args." ) ;
	InitCopying() ;
	CreateTarget( "bar.c", "baz.c" ) ;
	CreateTarget( "/etc/motd", "bar.c" ) ;
}
#endif /* of ifdef TEST */
