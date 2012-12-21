#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "turing.h"
#include "turingparser.h"

// disable debug mode
#define NDEBUG
#include "errors.h"

Transition* Transition_create( State *start, State *next, Direction move, char input, char write )
{
	// make sure the input is valid
	assert( start );
	assert( next );

	// allocate memory
	Transition *trans = malloc( sizeof( Transition ));
	if( ! trans ) die( "Memory error" );

	trans->start = start;
	trans->next = next;
	trans->move = move;
	trans->input = input;
	trans->write = write;

	assert( trans );

	return trans;
}

void Transition_destroy( Transition* trans )
{
	free( trans );
}

void Transition_print( Transition *trans )
{
	if( ! trans ) printf( "NULL" );
	else {
		printf( "Transition{\n\tstate: %s\n\tinput: %c\n\tnext: %s\n\twrite: %c\n\tmove: %d\n}\n",
			trans->start->name,
			trans->input,
			trans->next->name,
			trans->write,
			trans->move
		);
	}
}

State* State_create( const char* name, bool accept, bool reject )
{
	assert( name );

	// allocate mem
	State *state = malloc( sizeof( State ));
	if( ! state ) die( "Memory error" );

	// copy the name and null terminate it
	strncpy( state->name, name, MAX_STATE_NAME_LEN - 1 );
	state->name[ MAX_STATE_NAME_LEN - 1 ] = '\0';

	// set other properties
	state->accept = accept;
	state->reject = reject;
	state->trans_count = 0;

	assert( state );

	return state;
}

void State_add_transition( State *state, Transition *trans )
{
	assert( state );
	assert( trans );

	// check if we can still add another transition
	if( state->trans_count == MAX_TRANSITIONS ) {
		char buffer[ 50 ];
		sprintf( buffer, "State %s already has the maximum amount of transitions.", state->name );

		die( buffer );
	}

	// add the transition
	state->transitions[ state->trans_count ] = trans;
	state->trans_count = state->trans_count + 1;
}

void State_print( State *state )
{
	if( ! state ) printf( "NULL" );
	else {
		printf( "State{\n\tname: %s\n\taccept: %d\n\treject: %d\n}\n",
			state->name,
			state->accept,
			state->reject
		);
	}
}

void State_destroy( State *state )
{
	assert( state );

	int i = 0;

	// loop over its transitions
	for( i = 0; i < state->trans_count; i++ ) {
		Transition *trans = state->transitions[ i ];
		if( !trans ) die( "Could not fetch transition." );

		Transition_destroy( trans );
	}

	free( state );
}

Turing* Turing_create()
{
	// allocate mem
	Turing *machine = malloc( sizeof( Turing ));

	machine->state_count = 0;
	machine->current = NULL;
	machine->head = 0;

	assert( machine );

	return machine;
}

void Turing_destroy( Turing *machine )
{
	assert( machine );

	int i = 0;

	// loop over it's states
	for( i = 0; i < machine->state_count; i++ ) {
		State *state = machine->states[ i ];
		if( !state ) die( "Could not fetch turing state" );

		State_destroy( state );
	}

	free( machine );
}

void Turing_add_state( Turing *machine, State *state )
{
	assert( machine );
	assert( state );

	if( machine->state_count == MAX_STATES ) {
		die( "The turing machine already has the maximum amount of states" );
	}

	// add the state
	machine->states[ machine->state_count++ ] = state;
}

void Turing_print( Turing *machine, char *tape, int tape_len )
{
	assert( machine );
	assert( tape );

	int i = 0;

	// print the start of the tape, upto the head
	for( i = 0; i < machine->head; i++ ) {
		printf( "%c", tape[i] );
	}

	// print the current state at the position of the head
	State *current = machine->current;
	if( !current ) die( "Couldn't fetch current state" );

	printf( " %s ", current->name );

	// print the rest of the tape
	for( i = machine->head; i < tape_len; i++ ) {
		printf( "%c", tape[i] );
	}

	printf( "\n" );
}

State* Turing_step( Turing *machine, char* tape, int tape_len )
{
	assert( machine );
	assert( tape );

	int i = 0;
	char input = tape[ machine->head ];
	State* state = machine->current;

	// look for a transition on the given input
	for( i = 0; i < state->trans_count; i++ ) {
		Transition* trans = state->transitions[ i ];
		if( !trans ) die( "Transition retrieval error" );

		// check if this is a transition in the given char input
		if( trans->input == input ) {
			State *next = trans->next;
			if( !next ) die( "Transitions to NULL state" );

			// write if nescesary
			if( trans->write != '\\' ) {
				tape[ machine->head ] = trans->write;
			}

			// move the head
			if( trans->move == Left ) {
				if( machine->head > 0 ) {
					machine->head--;
				}
			} else {
				if( machine->head + 1 >= tape_len ) {
					die( "Machine walked of tape on right side" );
				}
				
				machine->head++;
			}

			// move the machine to the next state
			machine->current = next;

			assert( next );

			return next;
		}
	}

	// if the above loop did not return a next state
	// no next state is available for the given input
	char buffer[ 50 ];
	sprintf( buffer, "Turing machine blocked: state %s for input %c", state->name, input );

	die( buffer );

	// this point should never be reached
	assert( false );
	return NULL;
}

void Turing_run( Turing *machine, char *tape, int tapelen )
{
	assert( machine );
	assert( tape );

	// check if the start state is configured properly
	if( !machine->current ) die( "Turing machine has now start state" );

	// loop until we reach a decision state
	while( true ) {
		// step to the next state
		State* state = Turing_step( machine, tape, tapelen );

		// check if we reache a decision state
		if( state->accept ) {
			printf( "\n> Done!\n\n> Input accepted in state: %s\n", state->name );
			break;
		} else if( state->reject ) {
			printf( "\n> Done!\n\n> Input rejected in state: %s\n", state->name );
			break;
		// print the current state of the machine if not
		} else {
			Turing_print( machine, tape, tapelen );
		}
	}
}

int main( int argc, char* argv[] )
{
	if( argc < 3 ) {
		die( "USAGE: $ turing <FILE> <tape length> <tape init>" );
	}

	// parse params
	char* fname = argv[1];
	int tapelength = atoi( argv[2] );

	// save copy of tape initial value
	char tape[ tapelength + 1 ];
	strncpy( tape, argv[3], tapelength );
	tape[ tapelength ] = '\0';

	// open file
	printf( fname );
	FILE *fh = fopen( fname, "r" );
	if( !fh ) {
		die( "Could not open file for reading..." );
	}

	printf( "> Parsing file %s ... \n\n", fname );
	
	Turing *machine = Turing_parse( fh );
	if( !machine ) {
		die( "Failed parsing the machine, exiting" );
	}

	printf( "> Starting simulation... \n\n" );
	
	// print the starting machine state
	Turing_print( machine, tape, 50 );

	// start the simulation
	Turing_run( machine, tape, 50 );

	// clean up
	fclose( fh );
	Turing_destroy( machine );

	return 0;
}
