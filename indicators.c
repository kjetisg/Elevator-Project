

#include "elevator_state.h"
#include "indicators.h"
#include "elev.h"
#include "io.h"



void initialize_button_indicators(system_state_t* system_state){
	for (int i = 0; i < N_FLOORS-1; ++i){
		elev_set_button_lamp(BUTTON_CALL_UP, i, 0);
		elev_set_button_lamp(BUTTON_COMMAND, i, 0);
	}
	
	for (int i = 1; i < N_FLOORS; ++i){
		elev_set_button_lamp(BUTTON_CALL_DOWN, i, 0);
		elev_set_button_lamp(BUTTON_COMMAND, i, 0);
	}
}


void reset_button_indicators(system_state_t* system_state){
	for (int i = 0; i < N_FLOORS-1; ++i){
		if (i == system_state->current_floor && elev_get_floor_sensor_signal >= 0){
			elev_set_button_lamp(BUTTON_CALL_UP, i, 0);
			elev_set_button_lamp(BUTTON_COMMAND, i, 0);
		}
	}
	
	for (int i = 1; i < N_FLOORS; ++i){
		if (i == system_state->current_floor && elev_get_floor_sensor_signal >= 0){
			elev_set_button_lamp(BUTTON_CALL_DOWN, i, 0);
			elev_set_button_lamp(BUTTON_COMMAND, i, 0);
		}
	}
}
