

#include <stdio.h>
#include <stdlib.h>
#include "elevator_state.h"
#include "buttons.h"
#include "elev.h"
#include "io.h"




int main(void){

    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }


    States fsm_state = init;
    States fsm_previous_state = init;
    States fsm_new_state = init;
    
    system_state_t system_state;
    system_state.motor_direction = 0;
    system_state.current_floor = -1;
    
    reset_active_floors(&system_state);
    reset_called_floors(&system_state);
    
    
    while (1){

	    fsm_previous_state = fsm_state;
        fsm_state = fsm_new_state;

        switch (fsm_state){
            case init:
                fsm_new_state = fsm_init_state(&system_state, fsm_previous_state);
                break;
                
            case idle:
                fsm_new_state = fsm_idle_state(&system_state,fsm_previous_state);      
		        break;
                
            case door_open:
                fsm_new_state = fsm_door_open(&system_state, fsm_previous_state);
                break;
            
            case going_down:
                fsm_new_state = fsm_going_down(&system_state, fsm_previous_state);
                break;
            
            case going_up:
                fsm_new_state = fsm_going_up(&system_state, fsm_previous_state);
                break;
                
	        case stop:
		        fsm_new_state = fsm_stop_button_pressed(&system_state, fsm_previous_state);
		        break;

            default:
		        elev_set_stop_lamp(1);
                break;
	    }
    }
}


