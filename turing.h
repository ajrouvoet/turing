#include <stdbool.h>

#ifndef _turing_h_
#define _turing_h_

#define MAX_TRANSITIONS 5
#define MAX_STATES 25
#define MAX_STATE_NAME_LEN 15

struct State;
struct Transition;

typedef struct Transition Transition;
typedef struct State State;

typedef enum {
	Left, Right
} Direction;

struct Transition {
	struct State *start;
	struct State *next;
	Direction move;
	char input;
	char write;
};

struct State {
	char name[ MAX_STATE_NAME_LEN ];
	struct Transition* transitions[ MAX_TRANSITIONS ];
	int trans_count;
	bool accept;
	bool reject;
};

struct Turing {
	State* states[ MAX_STATES ];
	State* current;
	int state_count;
	int head;
};

typedef struct Turing Turing;

Transition* Transition_create( State*, State*, Direction, char, char );
void Transition_destroy( Transition* );
void Transition_print ( Transition* );

State* State_create( const char*, bool, bool );
void State_add_transition( State*, Transition* );
void State_print( State* );
void State_destroy( State* );

Turing* Turing_create();
void Turing_destroy( Turing* );
void Turing_add_state( Turing*, State* );
void Turing_print( Turing*, char*, int );
State* Turing_step( Turing*, char* , int );
void Turing_run( Turing*, char*, int );

#endif
