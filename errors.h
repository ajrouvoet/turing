#ifndef _debug_h_
#define _debug_h_

// debug macro
#ifndef NDEBUG
#define debug(M, ... ) fprintf( stderr, "DEBUG: %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__ )
#else
#define debug(M, ... )
#endif

// function for stopping the program and displaying the
// error message passed to it
void die( char *message );

#endif
