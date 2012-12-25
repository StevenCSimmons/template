/*
 *  Module to do the list (-l, -L) switches.
 */

#ifndef lint
# ifndef  lib
static char gitid[] = "$Id$";
#pragma unused(gitid)
# endif /* if ifndef lib */
#endif  /* if ifndef lint */

#ifdef  TEST
#define MAIN
#endif  /* if ifdef TEST */

# include    "template.h"
# include    <sys/param.h>

extern char    *Template_List[];


/*
 *  Main driver for this module.  One by one look up the
 *  directory names and print them.  If needed, print the
 *  full contents of each directory.
 */

void ListTemplateDirs( const boolean List_Contents ) {
    char**   dir_list = Template_List;
    char     buf[ MAXPATHLEN + 10 ];

    (void) printf( "The current template directories " );
    if ( List_Contents )
        (void) printf( "and their contents " );
    (void) printf( "are:\n" );
    while ( *dir_list != NULL ) {
        if ( List_Contents ) {
	   (void) printf( "\n%s:\n", *dir_list );
	   (void) sprintf( buf, "ls -a \"%s\"", *dir_list );
	   (void) system( buf );
        } else
	   (void) printf( "  %s\n", *dir_list );
        dir_list++;
    }
    return;
}


#ifdef TEST

char* Template_List[] = {
    "/usr/lib/Templates", 
    "/usr/local/lib/Templates", 
    "./Templates",
    ".",
    "an extremely stupid template name",
    NULL
};

int main( int argc, char** argv ) {
    char** lister;

    ProgramName = argv[ 0 ];
    if ( argc != 1 )
        Warning( "This test does not use switches.\n" );
    (void) printf( "The current template list is:\n" );
    for ( lister = Template_List ; *lister != NULL ; lister++ ) {
        (void) printf( "  `%s'\n", *lister );
    }
    (void) printf( "\nListing by list only:\n\n" );
    ListTemplateDirs( FALSE );
    (void) printf( "\nListing by contents:\n\n" );
    ListTemplateDirs( TRUE );
    (void) printf( "\nTest completed.\n" );
}
#endif  /* of ifdef TEST */
