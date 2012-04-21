#include "../include/minfinite.h"

union fsm_action_arg {
  char input_character;
} ;

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
  Space,
  Quote,
  Backslash,
  Character,
  Paren_open,
  Paren_close,
  EndString
);

FSM_SETUP(lex);

// Forward declare action functions
static void add_to_buffer(union fsm_action_arg a);    
static void no_op(union fsm_action_arg a);
static void no_token(union fsm_action_arg a);
static void end_token(union fsm_action_arg a);
static void set_delimiter(union fsm_action_arg a);

void define_trans_table(void) {
  init_trans_table();

  FSM_TRANSITION(REST,            Space,        no_op,          REST);
  FSM_TRANSITION(REST,            Character,    add_to_buffer,  WORD);
  FSM_TRANSITION(REST,            Paren_open,   no_op,        COMMENT);
  FSM_TRANSITION(REST,            EndString,    no_token,       REST);
  FSM_TRANSITION(REST,            Quote,        no_op,          STRING);
  FSM_TRANSITION(REST,            Backslash,    no_op,          DELIMITER_WAIT);

  FSM_TRANSITION(WORD,            Character,    add_to_buffer,  WORD);
  FSM_TRANSITION(WORD,            Space,        end_token,      REST);
  FSM_TRANSITION(WORD,            EndString,    end_token,      REST);

  FSM_TRANSITION(COMMENT,         Character,    no_op,          COMMENT);
  FSM_TRANSITION(COMMENT,         Paren_close,  no_op,          REST);
  FSM_TRANSITION(COMMENT,         EndString,    no_op,          COMMENT);

  FSM_TRANSITION(STRING,          Quote,        end_token,      REST);

  FSM_TRANSITION(DELIMITER_WAIT,  Character,    set_delimiter,  HARD_STRING);
}


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

static token return_token = { UNKNOWN, {0} };
static char delimiter = 0;

char token_buffer[128];
char *tok_scanp = token_buffer;
//char *input_character;


void lex(char **cp) {
  char *input_character = *cp;
  union fsm_action_arg a;

  // reset return_token
  return_token.type = UNKNOWN;
  tok_scanp = token_buffer;

  while (return_token.type == UNKNOWN) {
    a.input_character = *input_character;
    switch (*input_character) {
      case ' ':	fire_event(Space, a);
                break;
      case '(':	fire_event(Paren_open, a);
                break;
      case ')':	fire_event(Paren_close, a);
                break;
      case '\0':	fire_event(EndString, a);
                  break;
      default:	fire_event(Character, a);
                break;
    }
    input_character++;
  }

  *cp = input_character;
}


void fsm_illegal_event(union fsm_action_arg a) {
  printf("illegal event: %c \n", a.input_character);
};

// action definitions
void set_delimiter(union fsm_action_arg a) {
  delimiter = a.input_character;
}

void no_token(union fsm_action_arg a) {
  return_token.type = NO_TOK;
}

void add_to_buffer(union fsm_action_arg a) {
  *tok_scanp++ = a.input_character;
}
void no_op(union fsm_action_arg a) {

}
void end_token(union fsm_action_arg a) {
  return_token.type = WORD;
}

