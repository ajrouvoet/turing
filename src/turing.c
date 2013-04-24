#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "turing.h"
#include "turingparser.h"

// disable debug mode
#define NDEBUG
#include "errors.h"

static int verbose = 1;

/* Transition constructor
 *
 * @return {Transition*} valid Transition pointer or NULL on error
 */
Transition* Transition_create( State *start, State *next, Direction move, char input, char write )
{
	// make sure the input is valid
	assert( start );
	assert( next );

	// allocate memory
	Transition *trans = malloc( sizeof( Transition ));
	if( ! trans ) {
		log_err( "Memory error" );
		return NULL;
	}

	trans->start = start;
	trans->next = next;
	trans->move = move;
	trans->input = input;
	trans->write = write;

	assert( trans );

	return trans;
}

/* Transition destructor
 */
void Transition_destroy( Transition* trans )
{
	free( trans );
}

/* Transition print
 * Or prints 'Transition{ NULL }' on NULL pointer
 */
void Transition_print( Transition *trans )
{
	if( ! trans ) printf( "Transition{ NULL }" );
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

/* State constructor
 *
 * @param name {const char*} Name of the state
 * @param accept {bool} Whether this is an accept state
 * @param reject {bool} Whether this is a reject state
 * @return {State*} or NULL on fail
 */
State* State_create( const char* name, bool accept, bool reject )
{
	assert( name );

	// allocate mem
	State *state = malloc( sizeof( State ));
	if( ! state ) {
		log_err( "Memory error" );
		return NULL;
	}

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

/* Adds a transition to a state
 *
 * @param state {State*}
 * @param trans {Transition*}
 * @return {bool} False on failure, true otherwise
 */
bool State_add_transition( State *state, Transition *trans )
{
	assert( state );
	assert( trans );

	// check if we can still add another transition
	if( state->trans_count == MAX_TRANSITIONS ) {
		log_err( "State %s already has the maximum amount of transitions.", state->name );
		return false;
	}

	// add the transition
	state->transitions[ state->trans_count ] = trans;
	state->trans_count = state->trans_count + 1;

	return true;
}

/* State printer
 * Prints 'State{ NULL }' if <state> is a NULL pointer
 */
void State_print( State *state )
{
	if( ! state ) printf( "State{ NULL }" );
	else {
		printf( "State{\n\tname: %s\n\taccept: %d\n\treject: %d\n}\n",
			state->name,
			state->accept,
			state->reject
		);
	}
}

/* State destructor
 * Also destroys all connected transitions
 *
 * Logs errors to stderr, but won't exit
 */
void State_destroy( State *state )
{
	assert( state );

	int i = 0;

	// loop over its transitions
	for( i = 0; i < state->trans_count; i++ ) {
		Transition *trans = state->transitions[ i ];
		if( !trans ) {
			log_err( "Problem destroying state transition." );
			continue;
		}

		Transition_destroy( trans );
	}

	free( state );
}

/* Turing machine constructor
 *
 * @return {Turing*} or NULL on failure
 */
Turing* Turing_create()
{
	// allocate mem
	Turing *machine = malloc( sizeof( Turing ));
	if( !machine ) {
		log_err( "Memory error." );
		return NULL;
	}

	// init
	machine->state_count = 0;
	machine->current = NULL;
	machine->head = 0;

	// create an accept and reject state
	machine->accept = State_create( "accept", true, false );
	machine->reject = State_create( "reject", false, true );
	Turing_add_state( machine, machine->accept );
	Turing_add_state( machine, machine->reject );

	assert( machine );

	return machine;
}

/* Turing destructor
 * Chain destroys all connected state.
 *
 * Will log on error, but won't exit
 *
 * @param machine {Turing*}
 */
void Turing_destroy( Turing *machine )
{
	assert( machine );

	int i = 0;

	// loop over it's states
	for( i = 0; i < machine->state_count; i++ ) {
		State *state = machine->states[ i ];
		if( !state ) {
			log_err( "Error while destroying machine state." );
			continue;
		}

		State_destroy( state );
	}

	free( machine );
}

/* Adds a state to a turing machine
 *
 * @param machine {Turing*}
 * @param state {State*}
 * @return {bool} Returns false on error
 */
bool Turing_add_state( Turing *machine, State *state )
{
	assert( machine );
	assert( state );

	if( machine->state_count == MAX_STATES ) {
		log_err( "The turing machine already has the maximum amount of states" );
		return false;
	}

	// add the state
	machine->states[ machine->state_count++ ] = state;

	return true;
}

/* Turing printer
 *
 * @param machine
 * @param tape {char*} input tape or NULL
 * @param tape_len {int} length of input tape
 */
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
	if( !current ) {
		log_err( "Failed to retrieve machines current state." );
		return;
	}

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
		if( !trans ) {
			log_err( "error while trying to get state transition." );
			continue;
		}

		// check if this is a transition in the given char input
		if( trans->input == input ) {
			State *next = trans->next;
			if( !next ) {
				log_err( "Transitions to NULL state" );
				goto error;
			}

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
					log_err( "Machine walked of tape on right side" );
					goto error;
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
	// we reject per default
	return machine->reject;

error:
	return NULL;
}

void Turing_run( Turing *machine, char *tape, int tapelen )
{
	assert( machine );
	assert( tape );

	// check if the start state is configured properly
	if( !machine->current ) {
		log_err( "Turing machine has now start state" );
		goto error;
	}

	// loop until we reach a decision state
	while( true ) {
		// step to the next state
		State* state = Turing_step( machine, tape, tapelen );

		// Turing_step returns NULL on error
		// and does it's own error reporting
		if( !state ) goto error;

		// check if we reache a decision state
		if( state->accept ) {
			printf( "\n> Done!\n\n> Input accepted in state: %s\n", state->name );
			break;
		} else if( state->reject ) {
			printf( "\n> Done!\n\n> Input rejected in state: %s\n", state->name );
			break;
		// print the current state of the machine if not
		} else {
			if( verbose > 0 ) {
				Turing_print( machine, tape, tapelen );
			}
		}
	}

error:
	return;
}

int main( int argc, char* argv[] )
{
	int c;
	extern int optind, optopt;

	while(( c = getopt( argc, argv, ":s" )) != -1 ) {
		switch( c ) {
			case 's':
				verbose = 0;
				break;
			case '?':
				printf( "unknown option: -%c\n", optopt );
				break;
		}
	}

	if( argc < 4 ) {
		die( "USAGE: $ turing <FILE> <tape length> <tape init>" );
	}

	// parse params
	char* fname = argv[optind++];
	int tapelength = atoi( argv[optind++] );

	// save copy of tape initial value
	char tape[ tapelength + 1 ];
	strncpy( tape, argv[optind++], tapelength );
	tape[ tapelength ] = '\0';

	// open file
	printf( fname );
	FILE *fh = fopen( fname, "r" );
	if( !fh ) {
		die( "Could not open file for reading..." );
	}

	printf( "> Parsing file %s ... \n\n", fname );

	// parse the file
	Turing *machine = Turing_parse( fh );
	if( !machine ) {
		die( "Failed parsing the machine, exiting" );
	}

	printf( "> Starting simulation... \n\n" );

	// print the starting machine state
	if( verbose > 0 ) {
		Turing_print( machine, tape, tapelength );
	}

	// start the simulation
	Turing_run( machine, tape, tapelength );

	// clean up
	fclose( fh );
	Turing_destroy( machine );

	return 0;
}
