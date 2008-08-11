/*
 *  Module to do the list (-l, -L) switches.
 *
 *  $RCSfile: list.c,v $	$Revision: 0.6 $
 *
 *  $Author: scs $	$Date: 2008/08/11 21:48:48 $
 *
 *  $State: Exp $	$Locker:  $
 *
 *  $Log: list.c,v $
 *  Revision 0.6  2008/08/11 21:48:48  scs
 *  Updated test to use /usr/lib/Templates.
 *
 *  Revision 0.5  1996/09/29 01:05:49  scs
 *  Removed old stdc stuff, minor bug fixes for stdin-only mode.
 *
 *  Revision 0.4  1989/12/09 15:12:17  scs
 *  Upgraded to new version of ANSI C compatibility macros.
 *
 *  Revision 0.3  89/11/12  22:01:15  scs
 *  First production release.  Stripped all useless history and side-alleys.
 *  
 */

#ifndef	lint
# ifndef	lib
static char	rcsid[] = "$Id: list.c,v 0.6 2008/08/11 21:48:48 scs Exp $" ;
# endif	/* if ifndef lib */
#endif	/* if ifndef lint */

#ifdef	TEST
#define	MAIN
#endif	/* if ifdef TEST */

# include	"template.h"
# include	<sys/param.h>

extern char*	Template_List[] ;


/*
 *  Main driver for this module.  One by one look up the
 *  directory names and print them.  If needed, print the
 *  full contents of each directory.
 */

void ListTemplateDirs( const boolean List_Contents )
{
	char**	dir_list = Template_List ;
	char	buf[ MAXPATHLEN + 10 ] ;

	(void) printf( "The current template directories " ) ;
	if ( List_Contents )
		(void) printf( "and their contents " ) ;
	(void) printf( "are:\n" ) ;
	while ( *dir_list != NULL )
	{
		if ( List_Contents )
		{
			(void) printf( "\n%s:\n", *dir_list ) ;
			(void) sprintf( buf, "ls -a \"%s\"", *dir_list ) ;
			(void) system( buf ) ;
		}
		else
			(void) printf( "  %s\n", *dir_list ) ;
		dir_list++ ;
	}
	return ;
}


#ifdef	TEST

char*	Template_List[] = {
	"/usr/lib/Templates", 
	"/usr/local/lib/Templates", 
	"./Templates",
	".",
	"an extremely stupid template name",
	NULL
	} ;

int main( int argc, char** argv )
{
	char**	lister ;

	ProgramName = argv[ 0 ] ;
	if ( argc != 1 )
		Warning( "This test does not use switches.\n" ) ;
	(void) printf( "The current template list is:\n" ) ;
	for ( lister = Template_List ; *lister != NULL ; lister++ )
	{
		(void) printf( "  `%s'\n", *lister ) ;
	}
	(void) printf( "\nListing by list only:\n\n" ) ;
	ListTemplateDirs( FALSE ) ;
	(void) printf( "\nListing by contents:\n\n" ) ;
	ListTemplateDirs( TRUE ) ;
	(void) printf( "\nTest completed.\n" ) ;
}
#endif	/* of ifdef TEST */
