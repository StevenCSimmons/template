/*
 * Build an actual file from a template and source. Module has
 * two entry points -- an initializer InitCopying(), and a service
 * provider CreateTarget().
 */

#ifdef	TEST
#define MAIN
#endif

#ifndef	lib
static char gitid[] = "$Id: 61f97e947ba3c629dcc36d5dfe43cec7debab083 $";
#pragma unused(gitid)
#endif // of ifndef lib

#include    "template.h"
#include    <sys/stat.h>
#include    <fcntl.h>
#include    <stdlib.h>

#define     TEMPFILE	"/tmp/template.XXXXXX"
#define     IO_CHUNK    ( BUFSIZ * 8 )

extern boolean  Verbose;
extern boolean  UsingStdout;

static int      tempfd = -1;        // fd for writing tempfile
static int      template = -1;      // fd for reading template
static int      target = -1;        // fd for read/write target
static char     tempfile_name[ sizeof( TEMPFILE ) + 1 ];
static int      file_mask = 0600;   // file creation mask

static char     message[ 512 ];


/*
 * Module-wide cleanup. Close all the fds, delete the
 * tempfile, reset all the variables to initial state.
 * Then check to see if there is an error message pending.
 * If so, print it and die. No parameters, no return value.
 */

static void module_cleanup() {
    (void) close( template );
    (void) close( tempfd );
    (void) close( target );
    tempfd = template = target = -1;
    (void) unlink( tempfile_name );
    if ( message[ 0 ] != '\0' ) {
        Fatal( message );
    }
    return;
}


/*
 * Initialize a file. Try to open it in the mode requested,
 * and return a fd for the opened file. Print nice error
 * messages should something go wrong.
 *
 * Incoming parameters:
 *   name    - path of file to be opened
 *   mode    - how we are to open it
 *   detail  - handy string to use in error messages
 *   xbits   - extra bits to be applied to file creation mode mask
 *
 * Returns
 *   an opened fd if successful, -1 if not
 */

static int open_file( char* name, int mode, char* detail, int xbits ) {
    int fd;

    if ( -1 == ( fd = open( name, mode, file_mask | xbits ) ) ) {
        (void) sprintf( message,
            "Couldn't open (%s) file `%s': %s",
            detail, name, strerror( errno ) );
        module_cleanup();
    }
    return fd;
}


/*
 * Given two open fds, copy the second file to the first.
 * Assume the file position pointer (as returned by lseek)
 * has already been set. Use big buffers -- we should make
 * this as fast as possible. If any error occur, print a
 * detailed message and return false immediately.
 *
 * Incoming parameters:
 *   to   - fd for the file to be copied to
 *   from - fd for the file to be copied from
 *
 * Returns
 *   TRUE/FALSE, if succeeded or failed.
 */

static boolean    copyfile( int to, int from ) {
    char    buffer[ IO_CHUNK ];
    size_t  inlen = IO_CHUNK;
    size_t  outlen;

    for (;;) {
        errno = 0;
        inlen = read( from, buffer, inlen );
        if ( inlen == 0 ) {
            return TRUE;
        } else if ( inlen == -1 ) {
            return FALSE;
	}
        errno = 0;
        outlen = write( to, buffer, (unsigned) inlen );
        if ( ( outlen == -1 ) || ( inlen != outlen ) ) {
            return FALSE; // write error
	}
    }
}


/*
 * Main entry point for module. It is expected this will be
 * called repeatedly, so be neat and clean with initializations.
 *
 * Given the name of a template and a target file, create a copy
 * of the file with the template included. Algorithm is:
 *   Get the mode bits for the template file
 *   Open a tempfile read-write, the target read-write-create
 *     but not truncate, and the template read-only.
 *   Copy the template to the tempfile.
 *   Copy the target to the tempfile, appending.
 *   Reset the target to beginning of file.
 *   Copy the tempfile into the target.
 *   Delete the tempfile.
 * Bitch mightily and die about any errors along the way.
 */

void CreateTarget( char* template_name, char* target_name ) {
    message[ 0 ] = '\0';
    template = open_file( template_name, O_RDONLY, "read", 0 );
    if ( UsingStdout ) {
        if ( Verbose ) {
            (void) fprintf( stderr, "Will copy `%s' and `%s' to stdout.\n",
                template_name, target_name );
        }
        if ( -1 != template ) {
            if ( ! copyfile( 2, template ) ) {
                (void) sprintf( message,
                    "Error in copying `%s' to standard out: %s",
                    template_name, strerror( errno ) );
	    }
        }
        if ( target_name != NULL ) {
            if ( *target_name != NULL_CHAR ) {
                target = open_file( target_name, O_RDONLY, "read", 0 );
                if ( ! copyfile( 2, target ) ) {
                    (void) sprintf( message,
                        "Error in copying `%s' to stdout: %s",
                        target_name, strerror( errno ) );
                }
            }
        }
    } else {
        // copying to file, not stdout
	struct stat statinfo;
	int    xbits;
	fstat( template, &statinfo );
	xbits = ( statinfo.st_mode & 0111 );
        if ( Verbose ) {
            (void) fprintf( stderr, "Creating/rewriting `%s' with template `%s'.\n",
                target_name, template_name );
	}
        target = open_file( target_name, ( O_RDWR | O_CREAT ), "modify", xbits );
        if ( ! copyfile( tempfd, template ) ) {
            (void) sprintf( message,
                "Error in copying `%s' to `%s': %s",
                target_name, tempfile_name, strerror( errno ) );
        } else if ( ! copyfile( tempfd, target ) ) {
            (void) sprintf( message,
                "Error in copying `%s' to `%s': %s",
                tempfile_name, target_name, strerror( errno ) );
        } else if ( 0 != lseek( tempfd, (off_t) 0, (off_t) 0 ) ) {
            (void) sprintf( message,
                "Could not reset `%s' to beginning of file: %s",
                tempfile_name, strerror( errno ) );
        } else if ( 0 != lseek( target, (off_t) 0, (off_t) 0 ) ) {
            (void) sprintf( message,
                "Could not reset `%s' to start of file: %s",
                target_name, strerror( errno ) );
        } else if ( ! copyfile( target, tempfd ) ) {
            (void) sprintf( message,
                "Error in copying `%s' to `%s': %s",
                target_name, tempfile_name, strerror( errno ) );
        }
    }
    module_cleanup();
    return;
}


/*
 * Initialization entry point for module. Sets up variables
 * that there's no point in rebuilding every time the module
 * is entered.
 */

void InitCopying() {
    int old_umask = umask( 0 );

    (void) strcpy( tempfile_name, TEMPFILE );
    tempfd = mkstemp( tempfile_name );

    (void) umask( old_umask );
    file_mask = ( ( ~old_umask ) & 0666 );
}

#ifdef    TEST

boolean Verbose = TRUE;
boolean UsingStdout = FALSE;

int main( int argc, char* argv[] ) {
    ProgramName = argv[ 0 ];
    Verbose = TRUE;
    if ( argc != 1 ) {
        Warning( "This test ignores args." );
    }
    InitCopying();
    CreateTarget( "bar.c", "baz.c" );
    CreateTarget( "/etc/motd", "bar.c" );
}
#endif // of ifdef TEST
