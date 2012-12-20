#include <stdio.h>
#include <stdlib.h>

void die( char *message )
{
	if( message )
	{
		printf( "Error: %s.\n", message );
	}

	// exit unsuccesfully
	exit(1);
}
