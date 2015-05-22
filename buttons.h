

#ifndef __Heisprosjekt__buttons__
#define __Heisprosjekt__buttons__

#include <stdio.h>
#include "elevator_state.h"


void check_cmd_buttons(system_state_t* system_state);

void check_floor(system_state_t* system_state);

void check_call_buttons(system_state_t* system_state);

void reset_button_indicators(system_state_t* system_state);

void initialize_button_indicators(system_state_t* system_state);


#endif /* defined(__Heisprosjekt__buttons__) */
