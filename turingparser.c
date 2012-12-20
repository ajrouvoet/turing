#include "turing.h"
#include "errors.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LEN 80

Transition* Transition_parse( FILE *fh, State** states, int states_len )
{
	int i = 0, parsed;
	char s1[ MAX_STATE_NAME_LEN ], s2[ MAX_STATE_NAME_LEN ];
	char format[ MAX_LINE_LEN ];
	char input, move, write;
	Direction dir;

	// initate a line buffer
	char *buffer = malloc( MAX_LINE_LEN * sizeof( char ));
	if( !buffer ) die( "Memory error" );

	// parse the line
	fgets( buffer, MAX_LINE_LEN, fh );
	sprintf( format, "%%%ds %%c -> %%%ds %%c %%c", 
		( MAX_STATE_NAME_LEN - 1 ),
		( MAX_STATE_NAME_LEN - 1 )
	);
	parsed = sscanf( buffer, format, s1, &input, s2, &write, &move );
	if( parsed == EOF ) return NULL;
	if( parsed < 5 ) die( "Parse error, not a valid transition syntax" );

	// find the state pointers based on their name
	State *st1 = NULL;
	State *st2 = NULL;
	for( i = 0; i < states_len; i++ ) {
		State *state = states[i];
		if( strcmp( state->name, s1 ) == 0 )
			st1 = state;

		if( strcmp( state->name, s2 ) == 0 )
			st2 = state;
	}

	// verify both states where found
	if( !st1 || !st2 ) die( "Parse error: could not find the states of one of the transitions specified" );

	switch( move ) {
		case 'L':
			dir = LEFT;
			break;

		case 'R':
			dir = RIGHT;
			break;

		default:
			die( "Parse error: could not parse direction of one of the transitions." );
	}

	// now create the transition
	Transition *trans = Transition_create( input, write, dir, st2 );

	// add it to the state
	State_add_transition( st2, trans );

	// cleanup
	free( buffer );

	return trans;
}

State* State_parse( FILE *fh )
{
	int parsed, accept, reject;
	char name[ MAX_STATE_NAME_LEN ];
	char error[ MAX_LINE_LEN ];
	char mode = '\0';
	char format[ MAX_LINE_LEN ];

	// initate a line buffer
	char *buffer = malloc( MAX_LINE_LEN * sizeof( char ));
	if( !buffer ) die( "Memory error" );

	// read the first line and try to parse it
	fgets( buffer, MAX_LINE_LEN, fh );
	sprintf( format, "%%%ds %%c", ( MAX_STATE_NAME_LEN - 1 )), 
	parsed = sscanf( buffer, format, name, &mode );
	if( parsed < 1 ) {
		strcpy( error, "Could not parse state syntax: " );
		strcat( error, format );
		goto out;
	}

	// determine accept, reject parameters based on mode
	switch( mode ) {
		case 'A':
			accept = TRUE;
			reject = FALSE;
			break;

		case 'R':
			accept = FALSE;
			reject = TRUE;
			break;

		default:
			accept = FALSE;
			reject = FALSE;
	}

	// create a new state
	State *state = State_create( name, accept, reject );

	// cleanup
	free( buffer );

	return state;

// on parse errors
out:
	State_destroy( state );
	free( buffer );

	die( error );

	return NULL;
}

Turing* Turing_parse( FILE *fh )
{
	int num_states, parsed, i;
	char error[ MAX_LINE_LEN ];

	// create a line buffer
	char *buffer = malloc( MAX_LINE_LEN * sizeof( char ));
	if( !buffer ) die( "Memory error" );

	// create a new turing machine
	Turing *machine = Turing_create();
	
	// buffer the first line
	fgets( buffer, MAX_LINE_LEN, fh );

	// try to read the amount of states from the input
	parsed = sscanf( buffer, "%d", &num_states );
	if( parsed < 1 ) {
		strcpy( error, "Could not parse state num." );
		goto out;
	}

	// now try to parse <num_states> states
	for( i = 0; i < num_states; i++ ) {
		State* state = State_parse( fh );
		if( !state ) {
			strcpy( error, "Could not parse state." );
			goto out;
		}

		// add the state to the machine
		Turing_add_state( machine, state );
	}

	// parse the transitions
	while( 1 ) {
		Transition *trans = Transition_parse( fh, machine->states, machine->state_count );

		// if we didn't get a valid trans back
		// we reached EOL
		if( !trans ) break;
	}

	// set the first state
	machine->current = machine->states[0];
	if( !machine->current ) die( "Turing machine should have atleast one state" );
	
	// cleanup
	free( buffer );

	return machine;

// on parse errors
out:
	Turing_destroy( machine );
	free( buffer );

	die( error );

	return NULL;
}
