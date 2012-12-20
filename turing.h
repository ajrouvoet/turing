#ifndef __turing_h__
#define __turing_h__

#define MAX_TRANSITIONS 5
#define MAX_STATES 25

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
	char input;
	char write;
	Direction move;
	struct State *next;
};

typedef struct Transition Transition;

struct State {
	int id;
	int trans_count;
	struct Transition* transitions[ MAX_TRANSITIONS ];
	Bool accept;
	Bool reject;
};

typedef struct State State;

struct Turing {
	int state_count;
	State* states[ MAX_STATES ];
	State* current;
	int head;
};

typedef struct Turing Turing;

#endif
