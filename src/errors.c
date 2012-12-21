#include <stdio.h>
#include <stdlib.h>
#include "errors.h"

void die( char *message )
{
	if( message )
	{
		log_err( "Error: %s.\n", message );
	}

	// exit unsuccesfully
	exit(1);
}
