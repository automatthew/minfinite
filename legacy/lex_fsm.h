#include <stdio.h>

// General functions    
inline void fire_event(int event, char c);

typedef void (*fsm_action)(char c);

#define FSM_STATES(name, args...)\
  typedef enum { args, LastState } fsm_states_##name; \
  typedef struct { fsm_action action; fsm_states_##name next_state; } fsm_trans; \
  fsm_trans create_transition(fsm_action a, int next) { \
    fsm_trans trans = { a, next}; \
    return trans; \
  }
  

#define FSM_EVENTS(name, args...) typedef enum { args, LastEvent } fsm_events_##name

#define FSM_ALLOC_TABLE(name) fsm_trans trans_table[LastState][LastEvent];

#define FSM_TRANSITION(name, state, event, action, next_state) \
  trans_table[state][event] = create_transition(action, next_state)




/* FSM definitions  */


FSM_STATES(lex,
  REST,
  COMMENT,
  WORD,
  STRING,
  DELIMITER_WAIT,
  HARD_STRING
);

FSM_EVENTS(lex,
  SPACE,
  QUOTE,
  BACKSLASH,
  CHARACTER,
  PAREN_OPEN,
  PAREN_CLOSE,
  END_STR
);

FSM_ALLOC_TABLE(lex);

int currentState = 0;

void fire_event(int event, char c)
{
  fsm_trans transition = trans_table[currentState][event];
  currentState = transition.next_state;
  transition.action(c);
}



//app-specific
typedef enum {
  UNKNOWN,
  NO_TOK,
  WORD_TOK,
  STR_TOK,
  INVALID_TOK
} token_type;

typedef union {
  int integer;
  char *string;
} token_value;

typedef struct {
  token_type type;
  token_value value;
} token;

token return_token = { UNKNOWN, {0} };

char token_buffer[128];
char *tok_scanp = token_buffer;
// end app-specific



// Declare action functions
void comment(char e);    
void add_to_buffer(char e);    
void no_op(char e);
void no_token(char e);
void end_token(char e);
void set_delimiter(char e);

void illegal_event(char c) {
  printf("illegal event\n");
}

void init_trans_table(fsm_trans table[][LastEvent]) {
  int i, j;
  for( i = 0; i < LastState; ++i) {
    for(j = 0; j < LastEvent; ++j) {
      fsm_trans trans = { illegal_event, i};
      table[i][j] = trans;
    }
  }
}

void define_trans_table(fsm_trans table[][LastEvent]) {
  init_trans_table(table);

  table[REST][SPACE] = 				create_transition(no_op, REST);
  table[REST][CHARACTER] = 			create_transition(add_to_buffer, WORD_TOK);
  table[REST][PAREN_OPEN] = 			create_transition(comment, COMMENT);
  table[REST][END_STR] = 				create_transition(no_token, REST);
  table[REST][QUOTE] = 				create_transition(no_op, STRING);
  table[REST][BACKSLASH] = 			create_transition(no_op, DELIMITER_WAIT);

  //table[WORD_TOK][CHARACTER] = 		create_transition(add_to_buffer, WORD_TOK);
  FSM_TRANSITION(lex, WORD_TOK, CHARACTER, add_to_buffer, WORD_TOK);
  FSM_TRANSITION(lex, WORD_TOK, SPACE, end_token, REST);
  //table[WORD_TOK][SPACE] = 			create_transition(end_token, REST);
  table[WORD_TOK][END_STR] = 			create_transition(end_token, REST);

  table[COMMENT][PAREN_CLOSE] = 		create_transition(no_op, REST);
  table[COMMENT][END_STR] = 			create_transition(no_op, COMMENT);

  table[STRING][QUOTE] = 				create_transition(end_token, REST);
  table[DELIMITER_WAIT][CHARACTER] = 	create_transition(set_delimiter, HARD_STRING);
}



void lex(char **cp) {
  char *scanp = *cp;

  // reset return_token
  return_token.type = UNKNOWN;
  tok_scanp = token_buffer;

  while (return_token.type == UNKNOWN) {
    switch (*scanp) {
      case ' ':	fire_event(SPACE, *scanp);
                break;
      case '(':	fire_event(PAREN_OPEN, *scanp);
                break;
      case ')':	fire_event(PAREN_CLOSE, *scanp);
                break;
      case '\0':	fire_event(END_STR, *scanp);
                  break;
      default:	fire_event(CHARACTER, *scanp);
                break;
    }
    scanp++;
  }

  *cp = scanp;
}




// action definitions
void comment(char c) {

}

void set_delimiter(char c) {

}

void no_token(char c) {
  return_token.type = NO_TOK;
}

void add_to_buffer(char c) {
  *tok_scanp++ = c;
}
void no_op(char c) {

}
void end_token(char c) {
  return_token.type = WORD;
}

