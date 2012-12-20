#ifndef __turing_h__
#define __turing_h__

#define MAX_TRANSITIONS 5
#define MAX_STATES 25
#define MAX_STATE_NAME_LEN 15

// forward declare structs
struct State;
struct Transition;

typedef enum {
	LEFT, RIGHT
} Direction;

typedef enum {
	FALSE, TRUE
} Bool;

struct Transition {
	struct State *next;
	Direction move;
	char input;
	char write;
};

typedef struct Transition Transition;

struct State {
	char name[ MAX_STATE_NAME_LEN ];
	struct Transition* transitions[ MAX_TRANSITIONS ];
	int trans_count;
	Bool accept;
	Bool reject;
};

typedef struct State State;

struct Turing {
	State* states[ MAX_STATES ];
	State* current;
	int state_count;
	int head;
};

typedef struct Turing Turing;

#endif
