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
 *  $RCSfile: main.c,v $	$Revision: 0.19 $
 *
 *  $Author: scs $	$Date: 2006/01/27 14:56:00 $
 *
 *  $State: Exp $	$Locker:  $
 *
 *  $Log: main.c,v $
 *  Revision 0.19  2006/01/27 14:56:00  scs
 *  Port to Darwin/MacOS.
 *
 *  Revision 0.18  2006/01/27 13:50:30  scs
 *  Let template.h include string defs.
 *
 *  Revision 0.17  1996/09/29 01:05:49  scs
 *  Removed old stdc stuff, minor bug fixes for stdin-only mode.
 *
 *  Revision 0.16  1990/10/14 11:28:48  scs
 *  Corrected use of PARAM_0/PROTO_0.
 *
 *  Revision 0.15  89/12/09  15:27:58  scs
 *  Upgraded to new ANSI C compatibility macros, minor change to shut lint
 *  up.
 *  
 *  Revision 0.14  89/11/24  14:40:00  scs
 *  Changed patchlevel printout style.
 *  
 *  Revision 0.13  89/11/12  22:01:18  scs
 *  First production release.  Stripped all useless history and side-alleys.
 */

#ifndef	lint
#ifndef	lib
static char	rcsid[] = "$Id: main.c,v 0.19 2006/01/27 14:56:00 scs Exp $" ;
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
