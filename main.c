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
 *
 *  $RCSfile: main.c,v $	$Revision: 0.13 $
 *
 *  $Author: scs $	$Date: 1989/11/12 22:01:18 $
 *
 *  $State: Production $	$Locker:  $
 *
 *  $Log: main.c,v $
 *  Revision 0.13  1989/11/12 22:01:18  scs
 *  First production release.  Stripped all useless history and side-alleys.
 *
 */

#ifndef	lint
#ifndef	lib
static char	rcsid[] = "$Id: main.c,v 0.13 1989/11/12 22:01:18 scs Production $" ;
#endif
#endif

#define	MAIN

# include	"template.h"
# include	"patchlevel.h"

extern void	ProcessSwitches( PROTO_3( int, char**, char** ) ) ;
extern void	ProcessFiles( PROTO_1( char** ) ) ;
extern void	InitCopying( PROTO_0 ) ;
extern void	ListTemplateDirs( PROTO_1( boolean ) ) ;

extern char*	strrchr( PROTO_2( char *a, char b ) ) ;

extern boolean	DirList ;
extern boolean	DirContents ;
extern boolean	Verbose ;

static int	version = 1 ;
static int	release = 0 ;

static const char*	file_list[ MAX_FILES + 1 ] ;

#ifdef	TEST

boolean	DirList = FALSE ;
boolean	DirContents = FALSE ;
boolean	Verbose = TRUE ;

#ifdef	__STDC__
void	ProcessSwitches( int i, char** c, char** l )
#else
void	ProcessSwitches( i, c, l )
int	i ;
char**	c ;
char**	l ;
#endif
{
	(void) printf( "ProcessSwitches called.\n" ) ;
}

#ifdef	__STDC__
void	InitCopying( void )
#else
void	InitCopying()
#endif
{
	(void) printf( "InitCopying called.\n" ) ;
}

#ifdef	__STDC__
void	ListTemplateDirs( boolean b )
#else
void	ListTemplateDirs( b )
boolean b ;
#endif
{
	(void) printf( "ListTemplateDirs called.\n" ) ;
}

#ifdef	__STDC__
void	ProcessFiles( char** f )
#else
void	ProcessFiles( f )
char**	f ;
#endif
{
	(void) printf( "ProcessFiles called.\n" ) ;
}
#endif	/* of ifdef TEST */

/*
 *  The main.  Can we get much simpler?
 */

#ifdef	__STDC__
int	main( int argc, char** argv )
#else
int	main( argc, argv )
int	argc ;
char**	argv ;
#endif
{
	if ( NULL == ( ProgramName = strrchr( argv[ 0 ], '/' ) ) )
		ProgramName = argv[ 0 ] ;
	else
		ProgramName++ ;
	ProcessSwitches( argc, argv, file_list ) ;
	if ( Verbose )
	{
		(void) printf( "%s: Version %d.%d", ProgramName, version, release ) ;
		if ( PATCHLEVEL > 0 )
			(void) printf( "PL%d\n", PATCHLEVEL ) ;
		else
			(void) putchar( '\n' ) ;
	}
	InitCopying() ;
	if ( DirList || DirContents )
		ListTemplateDirs( DirContents ) ;
	else
		ProcessFiles( file_list ) ;
	return 0 ;
}
