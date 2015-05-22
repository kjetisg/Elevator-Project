

#include "elevator_state.h"
#include "buttons.h"
#include "queue.h"
#include "elev.h"



void check_cmd_buttons(system_state_t* system_state){ 
    for (int i = 0; i < N_FLOORS; ++i){
        if(elev_get_button_signal(BUTTON_COMMAND, i)){
            system_state->elev_active_floors[i] = 1;
            elev_set_button_lamp(BUTTON_COMMAND, i, 1);
        }
    }
}


void check_call_buttons(system_state_t* system_state){
    for (int i = 1; i < N_FLOORS; ++i){
		if (elev_get_button_signal(BUTTON_CALL_DOWN,i)){
	    	system_state->elev_called_floors_down[i] = 1;
	    	elev_set_button_lamp(BUTTON_CALL_DOWN, i, 1);
        }
    }
    
    for (int i = 0; i < N_FLOORS - 1; ++i){
		if (elev_get_button_signal(BUTTON_CALL_UP,i)){
	    	system_state->elev_called_floors_up[i] = 1;
	    	elev_set_button_lamp(BUTTON_CALL_UP, i, 1);
		}
    }
}


void check_floor(system_state_t* system_state){
    if (elev_get_floor_sensor_signal() >= 0){
        system_state->current_floor = elev_get_floor_sensor_signal();
    }
}



