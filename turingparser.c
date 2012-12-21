#include "turing.h"
#include "errors.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define MAX_LINE_LEN 80

/* Parses a transition from a file in the following syntax:
 * 	
 * 	s1 input -> s2 write move
 * 		`s1`	string name of start state
 * 		`input`	input on which this transition occurs
 * 		`s2`	string of destination state
 * 		`write`	char to write to tape
 * 		`move`	direction to move tape head (either 'R' or 'L')
 *
 * It looks up the states by name in the given states array and
 * adds them to the transition to the start state.
 *
 * If parsing fails, it will print an error and return NULL
 *
 * @param fh {FILE*} pointer to file stream to read from
 * @param states {State**} pointer to array of State pointers
 * @param states_len {int} length of states
 * @returns {Transition*} Created transition
 */
Transition* Transition_parse( FILE *fh, State** states, int states_len )
{
	State *st1 = NULL;
	State *st2 = NULL;
	int i = 0, parsed = 0;
	char s1[ MAX_STATE_NAME_LEN ], s2[ MAX_STATE_NAME_LEN ];
	char format[ MAX_LINE_LEN ];
	char buffer[ MAX_LINE_LEN ];
	char input, move, write;
	Direction dir;

	// read the line
	fgets( buffer, MAX_LINE_LEN, fh );
	if( feof( fh ) ) return NULL;

	// create a parsing format and
	// parse the line
	sprintf( format, "%%%ds %%c -> %%%ds %%c %%c", 
		( MAX_STATE_NAME_LEN - 1 ),
		( MAX_STATE_NAME_LEN - 1 )
	);
	parsed = sscanf( buffer, format, s1, &input, s2, &write, &move );

	// if there WAS a line, it should be in correct syntax
	if( parsed < 5 ) {
		log_err( "Wrong transition line in file." );
		goto out;
	}

	// find the state pointers based on their name
	for( i = 0; i < states_len; i++ ) {
		State *state = states[i];

		if( strcmp( state->name, s1 ) == 0 ) {
			st1 = state;
		}

		if( strcmp( state->name, s2 ) == 0 ) {
			st2 = state;
		}
	}

	// verify both states where found
	if( !st1 || !st2 ) {
		log_err( "Parse error: could not find the states of one of the transitions specified" );
		goto out;
	}

	// get the move direction
	switch( move ) {
		case 'L':
			dir = Left;
			break;

		case 'R':
			dir = Right;
			break;

		default:
			log_err( "Parse error: could not parse direction of one of the transitions." );
			goto out;
	}

	// now create the transition
	Transition *trans = Transition_create( st1, st2, dir, input, write );

	// add it to the state
	State_add_transition( st1, trans );
	
	return trans;

// on parse errors
out:
	return NULL;
}

/* Parses a state from a file using the following syntax
 * 	
 * 	s1 mode
 *		`s1` 	unique name of the new state
 *		`mode`	char indicating whether it's a decision state ('A' or 'R')
 *
 * If parsing fails, it will print an error and return NULL
 *
 * @param fh {FILE*} pointer to file stream to read from
 * @returns {State*} Created State
 */
State* State_parse( FILE *fh )
{
	int parsed, accept, reject;
	char name[ MAX_STATE_NAME_LEN ];
	char mode = '\0';
	char format[ MAX_LINE_LEN ];
	char buffer[ MAX_LINE_LEN ];

	// read the first line
	fgets( buffer, MAX_LINE_LEN, fh );
	// create the parsing format
	// and parse the line
	sprintf( format, "%%%ds %%c", ( MAX_STATE_NAME_LEN - 1 )), 
	parsed = sscanf( buffer, format, name, &mode );

	// verify that we captured at least a name
	// and possibly a state
	if( parsed < 1 ) {
		log_err( "Could not parse state line." );
		goto out;
	}

	// determine accept and reject parameters based on mode
	switch( mode ) {
		case 'A':
			accept = true;
			reject = false;
			break;

		case 'R':
			accept = false;
			reject = true;
			break;

		default:
			accept = false;
			reject = false;
	}

	// create a new state
	State *state = State_create( name, accept, reject );

	// this method should always return a valid state pointer
	assert( state );

	return state;

// on parse errors
out:
	return NULL;
}

/* Parses a turing machine from a file, first line should contain
 * the amount of states it should read from the file as a single number
 * 	
 * If parsing fails, it will print an error and return NULL
 *
 * @param fh {FILE*} pointer to file stream to read from
 * @returns {Turing*} Created Turing machine
 */
Turing* Turing_parse( FILE *fh )
{
	int num_states, parsed, i;
	char buffer[ MAX_LINE_LEN ];

	// create a new turing machine
	Turing *machine = Turing_create();
	
	// buffer the first line
	fgets( buffer, MAX_LINE_LEN, fh );

	// try to read the amount of states from the input
	parsed = sscanf( buffer, "%d", &num_states );
	if( parsed < 1 ) {
		log_err( "Could not parse state num." );
		goto out;
	}

	// now try to parse <num_states> states
	for( i = 0; i < num_states; i++ ) {
		State* state = State_parse( fh );
		// State_parse will log the error
		if( !state ) goto out;

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
	if( !machine->current ) {
		log_err( "Turing machine should have atleast one state" );
		goto out;
	}

	return machine;

// on parse errors
out:
	Turing_destroy( machine );

	return NULL;
}
