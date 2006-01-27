/*
 *  Build an actual file from a template and source.  Module has
 *  two entry points -- an initializer InitCopying(), and a service
 *  provider CreateTarget().
 *
 *  $RCSfile: copyfile.c,v $	$Revision: 1.5 $
 *
 *  $Author: scs $	$Date: 2006/01/27 15:30:52 $
 *
 *  $State: Exp $	$Locker:  $
 *
 *  $Log: copyfile.c,v $
 *  Revision 1.5  2006/01/27 15:30:52  scs
 *  Convert to more secure mkstemp.
 *
 *  Revision 1.4  2006/01/27 15:13:39  scs
 *  Removed deprecated sys_errlist in favor of strerror().
 *
 *  Revision 1.3  2006/01/27 14:54:29  scs
 *  Removed a lot of old history and stuff that is now a standard part
 *  of the posix world.
 *
 *  Revision 1.2  2003/04/14 14:48:07  scs
 *  Updated tests to reflect newer switches.
 *
 *  Revision 1.1  2002/08/25 01:50:22  scs
 *  Moved to FreeBSD style tempfiles.
 *
 *  Revision 0.17  2002/04/25 03:45:04  scs
 *  Reset a few things for FreeBSD use and compilation.
 */

#ifdef	TEST
# define	MAIN
#endif

#ifndef	lint
# ifndef	lib
static char	rcsid[] = "$Id: copyfile.c,v 1.5 2006/01/27 15:30:52 scs Exp $" ;
# endif	/* of ifndef lib */
#endif	/* of ifndef lint */

# include	"template.h"
# include	<sys/param.h>
# include	<fcntl.h>

# define	TEMPFILE	"/tmp/template.XXXXXX"
# define	IO_CHUNK	( BUFSIZ * 8 )

extern boolean	Verbose ;
extern boolean	UsingStdout ;

static int	tempfd = -1 ;		/* fd for writing tempfile */
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

static void	module_cleanup()
{
	(void) close( template ) ;
	(void) close( tempfd ) ;
	(void) close( target ) ;
	tempfd = template = target = -1 ;
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

static int	open_file( char* name, int mode, char* detail )
{
	int	fd ;

	if ( -1 == ( fd = open( name, mode, file_mask ) ) )
	{
		(void) sprintf( message,
			"Couldn't open (%s) file `%s': %s",
			detail, name, strerror( errno ) ) ;
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

static boolean	copyfile( int to, int from )
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

void	CreateTarget( char* template_name, char* target_name )
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
		if ( -1 != template )
		{
			if ( ! copyfile( 2, template ) )
				(void) sprintf( message,
					"Error in copying `%s' to standard out: %s",
					template_name, strerror( errno ) ) ;
		}
		if ( target_name != NULL )
		{
			if ( *target_name != (char) NULL )
			{
				target = open_file( target_name, O_RDONLY, "read" ) ;
				if ( ! copyfile( 2, target ) )
				{
					(void) sprintf( message,
						"Error in copying `%s' to stdout: %s",
						target_name, strerror( errno ) ) ;
				}
			}
		}
	}
	else	/* copying to file, not stdout */
	{
		if ( Verbose )
			(void) fprintf( stderr, "Creating/rewriting `%s' with template `%s'.\n",
				target_name, template_name ) ;
		target = open_file( target_name, ( O_RDWR | O_CREAT ), "modify" ) ;
		if ( ! copyfile( tempfd, template ) )
		{
			(void) sprintf( message,
				"Error in copying `%s' to `%s': %s",
				target_name, tempfile_name, strerror( errno ) ) ;
		}
		else if ( ! copyfile( tempfd, target ) )
		{
			(void) sprintf( message,
				"Error in copying `%s' to `%s': %s",
				tempfile_name, target_name, strerror( errno ) ) ;
		}
		else if ( 0 != lseek( tempfd, (off_t) 0, (off_t) 0 ) )
		{
			(void) sprintf( message,
				"Could not reset `%s' to beginning of file: %s",
				tempfile_name, strerror( errno ) ) ;
		}
		else if ( 0 != lseek( target, (off_t) 0, (off_t) 0 ) )
		{
			(void) sprintf( message,
				"Could not reset `%s' to start of file: %s",
				target_name, strerror( errno ) ) ;
		}
		else if ( ! copyfile( target, tempfd ) )
		{
			(void) sprintf( message,
				"Error in copying `%s' to `%s': %s",
				target_name, tempfile_name, strerror( errno ) ) ;
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

void	InitCopying()
{
	int	old_umask = umask( 0 ) ;

	(void) strcpy( tempfile_name, TEMPFILE ) ;
	tempfd = mkstemp( tempfile_name ) ;

	(void) umask( old_umask ) ;
	file_mask = ( ( ~old_umask ) & 0666 ) ;
}

#ifdef	TEST

boolean	Verbose = TRUE ;
boolean UsingStdout = FALSE ;

int main( int argc, char* argv[] )
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
