/*
 *  This module reads all the user switches, error-checks them,
 *  and initialize the system appropriately.
 */

#ifdef	TEST
#define	MAIN
#endif

#ifndef	lint
# ifndef	lib
static char	gitid[] = "$Id$";
# endif	/* of ifdef lib */
#endif	/* of ifdef lint */

# include	"template.h"

extern int		OptionParse( int, char**, char const * ) ;
# ifndef	TEST
extern void		GetTemplList( char* ) ;
# endif

extern char*	OptionSwitch ;
extern char*	UserExtension ;

boolean Verbose = FALSE ;	/* -[vV] - be noisy? */
boolean NoAction = FALSE ;	/* -V - be noisy and fake it? */
boolean DirList = FALSE ;	/* -l - list template directories */
boolean DirContents = FALSE ;	/* -L - list template dirs and contents */
boolean UsingStdout = FALSE ;	/* -o - send output to stdout */
boolean ForceExtension = FALSE;	/* -e - use user-supplied extension */

static char* template_list = NULL ;	/* for -T<list> */

static char const		switch_list[] = "lLvVoT:e:" ;
static char const* const	usage_msg[] = {
	"Proper usage for extracting templates is:",
	"  template [-lLvVo] [ -T <template list> ] [ -e <extension> ] [ file(s) ]",
	NULL
} ;


/*
 *  Print an error message, tell about proper usage, and die.
 */

static void	usage( const char* const msg )
{
	/* char **	umsg = usage_msg ; */
	int i;

	(void) fprintf( stderr, "%s usage: %s\n", ProgramName, msg ) ;
	/* while ( *umsg != NULL ) */
	for ( i = 0; usage_msg[i] != (char) NULL; i++ ) {
		(void) fprintf( stderr, "%s\n", *usage_msg[i] ) ;
	}
	exit( 1 ) ;
}


/*
 *  If an optional arg exists, create a new copy of it and
 *  return it to the caller.  Otherwise print an error message,
 *  and exit with the usage message.
 *
 *  Incoming parameters:
 *
 *  Returns:
 *	Pointer to new string
 */

static const char	need_option[] = "The -%c option requires a %s name." ;

static char*	get_arg( const char option, const char* const type )
{
	char*	opt ;
	char	msgbuf[ BUFSIZ ] ;

	if ( NULL == OptionSwitch )
	{
		(void) sprintf( msgbuf, need_option, option, type ) ;
		usage( msgbuf ) ;
	}
	else if ( NULL == ( opt = NewString( OptionSwitch ) ) )
		Fatal( "Ran out of memory in get_arg!" ) ;
	return opt ;
}


void	ProcessSwitches( int argc, char** argv, const char** file_list )
{
	int			option ;
	char			msgbuf[ BUFSIZ ] ;
	/* char const* const*	user_files = file_list ; */
	char const**	user_files = file_list ;

	while ( EOF != ( option = OptionParse( argc, argv, switch_list ) ) )
		switch( option )
		{
		  case 'o':
			UsingStdout = TRUE ;
			break ;
		  case 'l':
			DirList = TRUE ;
			DirContents = FALSE ;
			break ;
		  case 'L':
			DirList = DirContents = TRUE ;
			break ;
		  case 'v':
			Verbose = TRUE ;
			break ;
		  case 'V':
			NoAction = Verbose = TRUE ;
			break ;
		  case 'e':
			ForceExtension = TRUE ;
			UserExtension = NewString( OptionSwitch ) ;
			break ;
		  case 'T':
			template_list = get_arg( option, "directory" ) ;
			break ;
		  case 0:
			*user_files = NewString( OptionSwitch ) ;
			user_files++ ;
			*user_files = NULL ;
			break ;
		  case '?':
			option = *OptionSwitch ;
		  default:
			(void) sprintf( msgbuf, "Illegal option `%c'", option ) ;
			usage( msgbuf ) ;
			break ;
		}
	
	/* Cross-check the combinations */

	if ( ( *file_list == NULL ) && ( ( !UsingStdout ) && ( !DirList ) && ( !DirContents ) ) )
		usage( "Sorry, you must specify some files." ) ;

	if ( UsingStdout && *file_list == NULL && !ForceExtension )
		usage( "Sorry, you must specify a name or an extension with -o." ) ;

	/* Switch combinations are legal.  Finish setting up. */

	GetTemplList( template_list ) ;
}

#ifdef	TEST

#define	MAX_FILES	1024

char*	UserExtension ;


static const char*	file_list[ MAX_FILES ] ;

void		GetTemplList( const char* const list )
{
	return ;	/* a dummy */
}

static void	print_string( const char* const msg, const char* const value )
{
	if ( msg == NULL )
		(void) printf( "%s: NULL\n", msg ) ;
	else
		(void) printf( "%s: `%s'\n", msg, value ) ;
}

static 	void	print_bool( const char* const msg, const int value )
{
	(void) printf( "%s: %s (0x%x)\n", msg,
		value ? "TRUE" : "FALSE",
		value ) ;
}

int	main( const int argc, const char* const* const argv )
{
	register int	i ;

	ProgramName = argv[ 0 ] ;
	for ( i = 0 ; i < MAX_FILES ; i++ )
		file_list[ i ] = NULL ;
	ProcessSwitches( argc, argv, file_list ) ;
	print_bool( "Verbose switch (-v) is" , Verbose ) ;
	print_bool( "Action switch (-V) is" , NoAction ) ;
	print_bool( "Template directory list switch (-l) is" , DirList ) ;
	print_bool( "Template directory contents switch (-L) is" , DirList ) ;
	print_bool( "Using standard out switch (-o) is" , UsingStdout ) ;
	print_bool( "Forced extension switch (-e) is", ForceExtension  ) ;
	if ( ForceExtension )
		(void) printf( "Forced extension name is (-e) is `%s'\n" , UserExtension ) ;
	(void) printf( "List of files is:\n" ) ;
	for ( i = 0 ; file_list[ i ] != NULL && i < MAX_FILES ; i++ )
		(void) printf( "  `%s'\n", file_list[ i ] ) ;
	return 0 ;
}
#endif
