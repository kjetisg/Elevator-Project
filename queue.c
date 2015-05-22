

#include "elevator_state.h"
#include "queue.h"
#include "elev.h"



void orders_clearAt(system_state_t* system_state, int floor){

	system_state->elev_active_floors[floor] = 0;
	system_state->elev_called_floors_down[floor] = 0;
	system_state->elev_called_floors_up[floor] = 0;

}


void reset_active_floors(system_state_t* system_state){
    for(int i = 0; i < N_FLOORS; ++i)
        system_state->elev_active_floors[i] = 0;
}


void reset_called_floors(system_state_t* system_state){
	for (int i = 0; i < N_FLOORS; ++i){
		system_state->elev_called_floors_down[i] = 0;
		system_state->elev_called_floors_up[i] = 0;
	}
}


int should_stop(system_state_t* system_state, int floor){
	
	if (system_state->elev_active_floors[floor] == 1 ||
	   (system_state->elev_called_floors_down[floor] == 1 &&
	    system_state->elev_called_floors_up[floor] == 1)){
			return 1;
		}			
		
	else if (system_state->elev_called_floors_down[floor] == 1){
		if (system_state->motor_direction == -1)	
			return 1;
		
		for (int i = floor+1; i < N_FLOORS; ++i){
			if (system_state->elev_active_floors[i] == 1 ||
				system_state->elev_called_floors_down[i] == 1 ||
				system_state->elev_called_floors_up[i] == 1){
				return 0;
			}
		}	
		return 1;
	}
	
	else if (system_state->elev_called_floors_up[floor] == 1){
		if (system_state->motor_direction == 1)		
			return 1;
		
		for (int i = 0; i < floor; ++i){
			if (system_state->elev_active_floors[i] == 1 ||
				system_state->elev_called_floors_down[i] == 1 ||
				system_state->elev_called_floors_up[i] == 1)
				return 0;
		}	
		return 1;
	}	
	return 0;	
}

