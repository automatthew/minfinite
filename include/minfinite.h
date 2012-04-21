#include <stdio.h>

// General functions    
inline void fire_event(int event, char c);

// Function pointer convenience
typedef void (*fsm_action)(char c);

// FSM user must define
static void fsm_illegal_event(char c);

struct fsm_trans;

// Defined in FSM_STATES macro
static struct fsm_trans create_transition(fsm_action a, int next);

#define FSM_STATES(name, args...) \
  enum fsm_states { args, LastState }; \
  struct fsm_trans { fsm_action action; enum fsm_states next_state; }; \
  static struct fsm_trans create_transition(fsm_action a, int next) { \
    struct fsm_trans trans = { a, next}; \
    return trans; \
  }
  

#define FSM_EVENTS(name, args...) \
  enum fsm_events { args, LastEvent }

#define FSM_SETUP(name) \
  static struct fsm_trans _trans_table[LastState][LastEvent]; \
  static int fsm_cur_state = 0; \
  void init_trans_table(void) { \
    int i, j; \
    for( i = 0; i < LastState; ++i) { \
      for(j = 0; j < LastEvent; ++j) { \
        struct fsm_trans trans = { fsm_illegal_event, i}; \
        _trans_table[i][j] = trans; \
      } \
    } \
  } \
  void fire_event(int event, char c) \
  { \
    struct fsm_trans transition = _trans_table[fsm_cur_state][event]; \
    fsm_cur_state = transition.next_state; \
    transition.action(c); \
  } 


#define FSM_TRANSITION(state, event, action, next_state) \
  _trans_table[state][event] = create_transition(action, next_state)


