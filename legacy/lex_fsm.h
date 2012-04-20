#include "../include/minfinite.h"

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
static void add_to_buffer(char e);    
static void no_op(char e);
static void no_token(char e);
static void end_token(char e);
static void set_delimiter(char e);

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



void lex(char **cp) {
  char *scanp = *cp;

  // reset return_token
  return_token.type = UNKNOWN;
  tok_scanp = token_buffer;

  while (return_token.type == UNKNOWN) {
    switch (*scanp) {
      case ' ':	fire_event(Space, *scanp);
                break;
      case '(':	fire_event(Paren_open, *scanp);
                break;
      case ')':	fire_event(Paren_close, *scanp);
                break;
      case '\0':	fire_event(EndString, *scanp);
                  break;
      default:	fire_event(Character, *scanp);
                break;
    }
    scanp++;
  }

  *cp = scanp;
}


// action definitions
void set_delimiter(char c) {
  delimiter = c;
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

