

#ifndef __Heisprosjekt__queue__
#define __Heisprosjekt__queue__

#include <stdio.h>
#include "elevator_state.h"
#include "elev.h"



void orders_clearAt(system_state_t* system_state, int floor);

void reset_active_floors(system_state_t* system_state);

void reset_called_floors(system_state_t* system_state);

int should_stop(system_state_t* system_state, int floor);


#endif /* defined(__Heisprosjekt__queue__) */
