
#include "elevator_state.h"
#include "elev.h"
#include "buttons.h"
#include "timer.h"
#include "motor.h"


void elev_set_direction(system_state_t* system_state, int n) {
	
	switch (n){
		case -1:
            system_state->motor_direction = -1;
			elev_set_motor_direction(DIRN_DOWN);
			break;

		case 1:
			system_state->motor_direction = 1;
			elev_set_motor_direction(DIRN_UP);
			break;

		case 0:
            system_state->motor_direction = 0;
			elev_set_motor_direction(DIRN_STOP);
			break;

		default:
			system_state->motor_direction = 0;
			elev_set_motor_direction(DIRN_STOP);
			break;
	}
}








	
