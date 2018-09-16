/*
** config.h - Configuration for Marlais
*/

/***********************************************************************
** Operating System Configuration
*/

/* Flag to distinguish Solaris from SunOS 4. */
#if defined(sun) && defined(__SVR4__)
# define __SunOS_5__
#endif

/* Define if the platform C library doesn't have the strcasecmp(3) function */
/* #define NO_STRCASECMP */

/* Use the readline library to get editing and history on input.
** Comment out if your system doesn't have the readline library.
*/
#define HAS_READLINE

/* Use dlopen() to load foreign function libraries */
#define HAS_DLOPEN

/* This probably depends on the compiler version not the platform */
#if defined(__MACH__) && defined(__APPLE__) /* Darwin or MacOS X */
# define USE_CONST
#endif

/* This is mostly just for gettring rid of warnings */
#ifdef USE_CONST
# define CONST const
#else
# define CONST
#endif

/***********************************************************************
** Interpreter Customization Features
*/

/* XXX This doesn't work currently. */
/* #define BIG_INTEGERS */

/* Try to be tail recursive. Make debugging a little harder. */
/* #define OPTIMIZE_TAIL_CALLS */

/* ??? I'm not sure if this actually works?
** !!! Actually things break if you don't define this.
** !!! I don't know!!! (some interaction w/ LARGE_OBJECTS ????)
*/
/* #define OPTIMIZE_SPECIALIZERS */
/* #define DEBUG_OPTIMIZE_SPECIALIZERS */

/* Cache methods in an attempt to improve generic function invocation speed. */
#define USE_METHOD_CACHING

/* Determine class precedence algorithm to use. Pick on of: */

/* Use L*Loops multiple inheritance precedence */
#define USE_LL_PRECEDENCE

/* Use CLOS multiple inheritance precedence */
/* #define USE_CLOS_PRECEDENCE */

/* The init file uses infix syntax */
/* XXX This should go away unless we revive prefix syntax */
#define INFIX_INIT_FILE

/* Full path to the init file */
#define INIT_FILE		"/usr/local/share/marlais/init.dylan"

/* ??? Not used ??? 
** String to mark output of the interpreter.
*/
#define OUTPUT_MARKER		"=> "

/*
** Define for more debugging features which may sacrifice efficiency.
**
** Currently this records in objects the place in a file from which the object
** was loaded.
*/
#define DEBUG

/* Make objects be much smaller. Make debugging harder. */
/* #define SMALL_OBJECTS */

/* Define to use the non-conservative garbage collector */
#define MGC

/***********************************************************************
** XXX Things which probably shouldn't be in here:
*/

/* The version string */
#define VERSION			"0.6.4-io-beta"
