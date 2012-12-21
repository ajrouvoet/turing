#include <errno.h>
#include <string.h>
#include <stdio.h>

#ifndef _debug_h_
#define _debug_h_

#ifdef NDEBUG
#define debug( M, ... )
#define debug_( C )
#else
#define debug( M, ... ) fprintf( stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__ )
#define debug_( C ) C
#endif

#define clean_errno() ( errno == 0 ? "None" : strerror( errno ) )

#define log_err( M, ... ) fprintf( stderr, "[ERROR] ( %s:%d: errno: %s ) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__ )

#define log_warn( M, ... ) fprintf( stderr, "[WARN] ( %s:%d: errno: %s ) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__ )

#define log_info( M, ... ) fprintf( stderr, "[INFO] ( %s:%d ) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__ )

#define check( A, M, ... ) if( !( A ) ) { log_err( M, ##__VA_ARGS__ ); errno=0; goto error; }

#define sentinel( M, ... )  { log_err( M, ##__VA_ARGS__ ); errno=0; goto error; }

#define check_mem( A ) check( ( A ), "Out of memory." )

#define check_debug( A, M, ... ) if( !( A ) ) { debug( M, ##__VA_ARGS__ ); errno=0; goto error; }

// function for stopping the program and displaying the
// error message passed to it
void die(  char *message  );

#endif
