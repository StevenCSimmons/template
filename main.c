/*
 *  This program initializes files for source code control.  It is
 *  a spiritual descendant of rcsit, by Michael Cooper of USC, but
 *  contains no code from his original.  It does owe a tremendous
 *  debt of inspiration, however.
 *
 *  The goal is to make it as easy as possible to create files
 *  which will be used for source code control without imposing
 *  any particular usage style (like, for example, mine) on the
 *  writer.
 */

#ifndef	lint
#ifndef	lib
static char	gitid[] = "$Id$";
#endif
#endif

#define	MAIN

# include	"template.h"
# include	"patchlevel.h"

extern void	ProcessSwitches( int, char**, char** ) ;
extern void	ProcessFiles( char** ) ;
extern void	InitCopying( void ) ;
extern void	ListTemplateDirs( boolean ) ;

extern boolean	DirList ;
extern boolean	DirContents ;
extern boolean	Verbose ;

static int	version = 1 ;
static int	release = 0 ;

static char*	file_list[ MAX_FILES + 1 ] ;

#ifdef	TEST

boolean	DirList = FALSE ;
boolean	DirContents = FALSE ;
boolean	Verbose = TRUE ;

void	ProcessSwitches( int i, char** c, char** l )
{
	(void) printf( "ProcessSwitches called.\n" ) ;
}

void	InitCopying()
{
	(void) printf( "InitCopying called.\n" ) ;
}

void	ListTemplateDirs( boolean b )
{
	(void) printf( "ListTemplateDirs called.\n" ) ;
}

void	ProcessFiles( char** f )
{
	(void) printf( "ProcessFiles called.\n" ) ;
}
#endif	/* of ifdef TEST */

/*
 *  The main.  Can we get much simpler?
 */

int	main( int argc, char** argv )
{
	if ( NULL == ( ProgramName = strrchr( argv[ 0 ], '/' ) ) )
		ProgramName = argv[ 0 ] ;
	else
		ProgramName++ ;
	ProcessSwitches( argc, argv, file_list ) ;
	if ( Verbose )
	{
		(void) printf( "%s: Version %d.%d", ProgramName, version, release ) ;
#if	PATCHLEVEL
		(void) printf( ".%d\n", PATCHLEVEL ) ;
#else
		(void) putchar( '\n' ) ;
#endif
	}
	InitCopying() ;
	if ( DirList || DirContents )
		ListTemplateDirs( DirContents ) ;
	else
		ProcessFiles( file_list ) ;
	return 0 ;
}
