/*
 *  This module reads all the user switches, error-checks them,
 *  and initialize the system appropriately.
 *
 *  $RCSfile: switches.c,v $	$Revision: 0.18 $
 *
 *  $Author: scs $	$Date: 1989/11/24 14:40:51 $
 *
 *  $State: Exp $	$Locker:  $
 *
 *  $Log: switches.c,v $
 *  Revision 0.18  1989/11/24 14:40:51  scs
 *  Added permission to use null file names with stdout.
 *
 *  Revision 0.17  89/11/12  22:01:38  scs
 *  First production release.  Stripped all useless history and side-alleys.
 *  
 */

#ifdef	TEST
#define	MAIN
#endif

#ifndef	lint
# ifndef	lib
static char	rcsid[] = "$Id: switches.c,v 0.18 1989/11/24 14:40:51 scs Exp $" ;
# endif	/* of ifdef lib */
#endif	/* of ifdef lint */

# include	"template.h"

extern void	exit( PROTO_1( int ) ) ;

extern int		OptionParse( PROTO_3( int, char**, char* ) ) ;
extern void		GetTemplList( PROTO_1( char* ) ) ;

extern const char*	OptionSwitch ;

extern char*	UserExtension ;

boolean Verbose = FALSE ;	/* -[vV] - be noisy? */
boolean NoAction = FALSE ;	/* -V - be noisy and fake it? */
boolean DirList = FALSE ;	/* -l - list template directories */
boolean DirContents = FALSE ;	/* -L - list template dirs and contents */
boolean UsingStdout = FALSE ;	/* -o - send output to stdout */
boolean ForceExtension = FALSE;	/* -e - use user-supplied extension */

static char* const	template_list = NULL ;	/* for -T<list> */

static const char const		switch_list[] = "lLvVoT:e:" ;
static const char const* const	usage_msg[] = {
	"Proper usage for extracting templates is:",
	"  template [-lLvVo] [ -T <template list> ] [ -e <extension> ] [ file(s) ]",
	NULL
} ;


/*
 *  Print an error message, tell about proper usage, and die.
 */

#ifdef	__STDC__
static void	usage( const char* const msg )
#else
static void	usage( msg )
const char* const	msg ;
#endif
{
	char**	umsg = usage_msg ;

	(void) fprintf( stderr, "%s usage: %s\n", ProgramName, msg ) ;
	while ( *umsg != NULL )
	{
		(void) fprintf( stderr, "%s\n", *umsg ) ;
		umsg++ ;
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

#ifdef	__STDC__
static char*	get_arg( const char option, const char* const type )
#else
static char*	get_arg( option, type )
const char		option ;
const char* const 	type ;
#endif
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


#ifdef	__STDC__
void	ProcessSwitches( const int argc, const char* const* const argv, const char* file_list[] )
#else
void	ProcessSwitches( argc, argv, file_list )
const int			argc ;
const char* const* const	argv ;
const char*			file_list[] ;
#endif
{
	int			option ;
	char			msgbuf[ BUFSIZ ] ;
	char const* const*	user_files = file_list ;

	if ( argc == 1 )
		usage( "What do you want template to do?" ) ;
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

	/* Switch combinations are legal.  Finish setting up. */

	GetTemplList( template_list ) ;
}

#ifdef	TEST

#define	MAX_FILES	1024


static const char*	file_list[ MAX_FILES ] ;

#ifdef	__STDC__
void		GetTemplList( const char* const list )
#else
void		GetTemplList( list )
const char* const	list ;
#endif
{
	return ;	/* a dummy */
}

#ifdef	__STDC__
static void	print_string( const char* const msg, const char* const value )
#else
static void	print_string( msg, value )
const char* const	msg ;
const char* const	value ;
#endif
{
	if ( msg == NULL )
		(void) printf( "%s: NULL\n", msg ) ;
	else
		(void) printf( "%s: `%s'\n", msg, value ) ;
}

#ifdef	__STDC__
static 	void	print_bool( const char* const msg, const int value )
#else
static 	void	print_bool( msg, value )
const char* const	msg ;
const int		value ;
#endif
{
	(void) printf( "%s: %s (0x%x)\n", msg,
		value ? "TRUE" : "FALSE",
		value ) ;
}

#ifdef	__STDC__
int	main( const int argc, const char* const* const argv )
#else
int	main( argc, argv )
const int			argc ;
const char* const* const	argv ;
#endif
{
	register int	i ;

	ProgramName = argv[ 0 ] ;
	for ( i = 0 ; i < MAX_FILES ; i++ )
		file_list[ i ] = NULL ;
	ProcessSwitches( argc, argv, file_list ) ;
	print_bool( "Verbose switch (-v) is" , Verbose ) ;
	print_bool( "Action switch (-V) is" , NoActions ) ;
	print_bool( "Template directory list switch (-l) is" , DirList ) ;
	print_bool( "Template directory contents switch (-L) is" , DirList ) ;
	print_bool( "Using standard out switch (-o) is" , UsingStdout ) ;
	print_bool( "Forced extension switch (-e) is", ForceExtension  ) ;
	if ( ForceExtension )
		(void) printf( "Forced extension name is (-e) is `%s'\n" , UserTemplate ) ;
	(void) printf( "List of files is:\n" ) ;
	for ( i = 0 ; file_list[ i ] != NULL && i < MAX_FILES ; i++ )
		(void) printf( "  `%s'\n", file_list[ i ] ) ;
	return 0 ;
}
#endif
