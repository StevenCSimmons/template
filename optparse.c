/*
 *  OptionParse() -- get command line switches according to
 *    a user-defined script
 *
 *  Purpose
 *
 *  This is a suite of functions which, taken together, gives
 *  the user very tight control over command line parsing.
 *  It consists of three entry points and a global variable:
 *
 *    OptionParse() -- get the "next" arguement
 *    OptionInit() -- can be called to "rewind" the arguement list
 *    OptionReset() -- allows the user to redefine the switches
 *    OptionSwitch -- extended arguement associated with a switch
 *
 *  See the individual routines below for detailed descriptions.
 */

#ifdef        TEST
# define    MAIN
# ifndef    DEBUGGING
#  define    DEBUGGING
# endif    // of ifdef DEBUGGING
#endif    // of ifdef TEST

#ifndef    lib
static char    gitid[] = "$Id$";
#pragma unused(gitid)
#endif    /* if ifndef lib */

#include    <stdio.h>
#include    <stdlib.h>
#include    <ctype.h>

/*    Functions from standard system libraries    */

extern char*    strchr() ;    /* scan a string for a character */

/*    Module-wide variables    */

static int    argnum = 1 ;    /* current location in argv */
static int    argchar = 1 ;    /* which character in arg we're looking at */
static char    swflag = '-' ;    /* what character delimits a switch */
static char    extarg = ':' ;    /* flag for extended argument wanted */
static char    errflg = '?' ;    /* what to return on a non-existant switch */
static char    dummy[ 2 ] ;    /* buffer to build error returns */

/*    Global variables    */

const char*    OptionSwitch = NULL ;    /* pointer to return an extended arguement */

/*
 *  OptionParse -- scan the arg list and return next arguement
 *
 *  parameters
 *    argc -- maximum number of arguements
 *    argv -- array of argc pointers to arguements
 *    opts -- list of valid switches
 *
 *  return value (int)
 *    null character if no switch is on the arguement, and
 *        OptionSwitch points to the arg;
 *    valid character if found behind switch, and OptionSwitch
 *        points to extended if requested (NULL if not or
 *        if there is none);
 *    errflg if illegal character found behind switch and
 *        OptionSwitch points to illegal character;
 *    EOF if no args to process.
 *
 *  This function scans the arguement list passed in, starting
 *  from an internal pointer into the list.  Each time it is
 *  called, the internal pointer is reset so the next call will
 *  scan the next arg.
 *
 *  An arguement is first checked to see if it begins with the
 *  switch character.  If it does not, it is assumed to be a
 *  non-switched arguement (eg, a file name).  A pointer to is
 *  is copied into OptionSwitch, and we return a null character.
 *
 *  If it is a switched arg, we return check the first character
 *  following the switch against the opts list.  If invalid, we
 *  set OptionSwitch to point to it and return errflag.
 *
 *  If valid, we then check to see if there should be an extended
 *  arguement.  If there should not be, we set OptionSwitch to NULL
 *  and return the switch character.
 *
 *  If there should be an extended arg, we check for it in two
 *  places: first, following the switch character in the same
 *  arguement.  If not found there, we look for the next arg.  If
 *  not found, set OptionSwitch NULL.  Return the switch character.
 *
 *    Format of valid switches string -- all of the valid switches
 *    lumped together into a single string.  Switches which have
 *    extended arguements must be followed by the extended arg
 *    flag, ':' for default (the extended arg flag may be reset by
 *    calling optargs()).  Thus "abc:d" would mean the switches
 *    -a, -b, -c, and -d are all legal.  In addition, the -c
 *    switch may have some sort of trailing parameter.
 */

int    OptionParse( const int argc, const char** const argv, const char* const opts ) {
    register char    c ;    /* General char holder */
    register char*    cp ;    /* Pointer into options list for this switch */

    OptionSwitch = NULL;

    /*
     * If we're just starting to look at this list ( argchar == 1 )
     * we just check two things -- end of arg list, and switch presence
     */
    
    if ( argchar == 1 ) {
        if ( argnum >= argc )    /* At end of arg list? */
            return EOF;
        if ( argv[ argnum ][ 0 ] != swflag ) {
            /* No switch flag on arg */
            OptionSwitch = argv[ argnum++ ];
            return '\0';
        } else if ( argv[ argnum ][ 1 ] == '\0' ) {
            // No arg following switch char
            // Return switch as arg, not switch
            dummy[ 0 ] = argv[ argnum++ ][ 0 ];
            OptionSwitch = (const char*) dummy;
            return '\0';
        }
    }

    /*
     * When we're past the first character, it means we are
     * seeing multiple characters under a single switch, eg, '-TagP'.
     * This is the equivalent of '-T -a -g -P', unless of course
     * extended processing is desired.  Thus, if 'T' was a solo
     * switch but 'a' required extended processing, '-TagP' would
     * be equivalent to '-T -a gP', where 'gP' is the extended arg
     * of 'a'.
     */
    
    c = argv[ argnum ][ argchar ];
    /* Check to see if this is a good switch */
    if ( ( cp = strchr( opts, c ) ) == NULL ) {
        /* Bad switch. */
        if ( argv[ argnum ][ ++argchar ] == '\0' ) {
            /* If end of arg, reset to look at next arg */
            argnum++;
            argchar = 1;
        }
        /* Build the error return and return it */
        dummy[ 0 ] = c;
        OptionSwitch = (const char*) dummy;
        return errflg;
    }

    /*
     * Check for extended arg with this switch.  Check both
     * formats, either '-S extarg' (two args) or '-Sextarg' (one arg)
     */
    
    if ( *++cp == extarg ) {
        if ( argv[ argnum ][ argchar + 1 ] != '\0' )
            OptionSwitch = &argv[ argnum++ ][ argchar + 1 ];
        else if ( ++argnum < argc )    /* Found extended */
            OptionSwitch = argv[ argnum++ ];
        argchar = 1 ;    /* Start at beginning of next arg */
    } else if ( argv[ argnum ][ ++argchar ] == '\0' ) {
        /* Arg complete -- reset for next arg */
        argchar = 1;
        argnum++;
    }
    return c;
}


/*
 *  OptionInit -- reset option scanner to start of arglist
 *
 *  parameters
 *    none
 *
 *  return value
 *    none
 *
 *  This function will reset the various internal counters and
 *  pointer such that the next time OptionParse is called, scanning
 *  will start at the beginning of the list.
 */

void    OptionInit( void )
{
    argchar = argnum = 1;
    OptionSwitch = NULL;
}



/*
 *  OptionReset -- redefine the various options flags and reinit OptionParse
 *
 *  parameters
 *    switchchar - the character that will become the switch
 *    errorflag -- the character that will be returned for errors
 *    externflag -- the character that will indicate extended args
 *
 *  return value
 *    none
 *
 *  Check the incoming parameters.  If any are null, set that
 *  particular flag to the default value.  Force incoming flags
 *  to be ascii.
 */

void    OptionReset( const char switchchar, const char errorflag, const char externflag ) {
    swflag = ( switchchar == '\0' ) ? '-' : toascii( switchchar );
    errflg = ( errorflag == '\0' ) ? '-' : toascii( errorflag );
    extarg = ( externflag == '\0' ) ? '-' : toascii( externflag );
}

#ifdef    TEST
/*
 *  This program allows you to call OptionParse with various
 *  command switches, and find out what OptionParse says the
 *  various switches are.
 */

int    main( int argc, char** argv ) {
    int    c;
    int    i;
    const char* const    opts = "abc:DefF:G";

    i = 1;
    (void) printf( "Option list is \"%s\"\n", opts );
    while ( EOF != ( c = OptionParse( argc, argv, opts ) ) ) {
        if ( c == '\0' ) {
            (void) printf( "Pass %d -- no arg, OptionSwitch is ", i );
            if ( OptionSwitch == NULL )
                (void) printf( "NULL\n" );
            else
                (void) printf( "\"%s\"\n", OptionSwitch );
        } else if ( c == '?' ) {
            (void) printf( "Pass %d -- err arg: ", i );
            if ( OptionSwitch == NULL )
                (void) printf( "NULL\n" );
            else
                (void) printf( "\"%s\"\n", OptionSwitch );
        } else {
            (void) printf( "Pass %d -- Got option '%c',", i, c );
            (void) printf( " got OptionSwitch arg " );
            if ( OptionSwitch == NULL )
                (void) printf( "NULL\n" );
            else
                (void) printf( "\"%s\"\n", OptionSwitch );
        }
        i++;
    }
    return 0;
}
#endif    /* of ifdef TEST */
