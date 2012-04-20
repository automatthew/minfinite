#include <stdio.h>

// General functions    
inline void fire_event(int event, char c);

typedef void (*fsm_action)(char c);

void illegal_event(char c) {
  printf("illegal event\n");
};

struct fsm_trans;
struct fsm_trans create_transition(fsm_action a, int next);

#define FSM_STATES(name, args...) \
  enum fsm_states_##name { args, LastState }; \
  struct fsm_trans { fsm_action action; enum fsm_states_##name next_state; }; \
  struct fsm_trans create_transition(fsm_action a, int next) { \
    struct fsm_trans trans = { a, next}; \
    return trans; \
  }
  

#define FSM_EVENTS(name, args...) enum fsm_events_##name { args, LastEvent }

#define FSM_SETUP(name) \
  struct fsm_trans _trans_table[LastState][LastEvent]; \
  int currentState = 0; \
  void fire_event(int event, char c) \
  { \
    struct fsm_trans transition = _trans_table[currentState][event]; \
    currentState = transition.next_state; \
    transition.action(c); \
  } \
  void init_trans_table(void) { \
    int i, j; \
    for( i = 0; i < LastState; ++i) { \
      for(j = 0; j < LastEvent; ++j) { \
        struct fsm_trans trans = { illegal_event, i}; \
        _trans_table[i][j] = trans; \
      } \
    } \
  }


#define FSM_TRANSITION(state, event, action, next_state) \
  _trans_table[state][event] = create_transition(action, next_state)


