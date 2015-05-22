

#include "elevator_state.h"
#include "indicators.h"
#include "buttons.h"
#include "motor.h"
#include "queue.h"
#include "timer.h"
#include "elev.h"



States fsm_init_state(system_state_t* system_state, States fsm_previous_state){

    if (system_state->motor_direction == 0){
        elev_set_direction(system_state, -1);
    }
    
    int floor = elev_get_floor_sensor_signal();  
    if (floor >= 0){	
    	elev_set_floor_indicator(floor);
		elev_set_direction(system_state, 0);
        system_state->current_floor = floor;
        elev_set_door_open_lamp(0);
        return idle;                         
    }
	reset_active_floors(system_state);
	reset_called_floors(system_state);
    return init;
}

//-----------------------------------------------------------------------------------

States fsm_idle_state(system_state_t* system_state, States fsm_previous_state){

	int floor = elev_get_floor_sensor_signal();
    system_state->next_floor = system_state->prev_floor + system_state->motor_direction;

    if (elev_get_stop_signal()){
		return stop;
	}	
	
    if (floor == 0){
		if (elev_get_button_signal(BUTTON_COMMAND, floor) ||
        	elev_get_button_signal(BUTTON_CALL_UP, floor)){
			return door_open;
		}
    } 
    
    else if (floor == 3){
		if (elev_get_button_signal(BUTTON_COMMAND, floor) ||
        	elev_get_button_signal(BUTTON_CALL_DOWN, floor)){
			return door_open;
		}
	} 
	
	else{
    	if (elev_get_button_signal(BUTTON_COMMAND, floor) ||
            elev_get_button_signal(BUTTON_CALL_UP, floor) ||
            elev_get_button_signal(BUTTON_CALL_DOWN, floor)){
        	return door_open;
    	}
    }
    
    check_cmd_buttons(system_state);
    
    for (int i = 0; i < N_FLOORS; ++i){
        if (system_state->elev_active_floors[i] == 1){
            if (i < floor){
            	system_state->motor_direction = -1;
                return going_down;
            }           
            else if (i > floor){  
            	system_state->motor_direction = 1;    
                return going_up;
            }
        }
    }
	
	
	check_call_buttons(system_state);

	for (int i=0; i<N_FLOORS;++i){
		if (system_state->elev_called_floors_down[i] || system_state->elev_called_floors_up[i]){
			if (i < floor){
				system_state->motor_direction = -1;
				return going_down;
			}
			else if (i > floor){
				system_state->motor_direction = 1;
				return going_up;
			}
		}
	}


    return idle;
}

//-----------------------------------------------------------------------------------

States fsm_door_open(system_state_t* system_state, States fsm_previous_state){

	elev_set_direction(system_state, 0);
	elev_set_door_open_lamp(1);
	
	if (!fsm_pause_3_sec(system_state)) 
		return stop;
	
	elev_set_door_open_lamp(0);
	check_floor(system_state);
	
    if (fsm_previous_state == idle){
		elev_set_button_lamp (BUTTON_COMMAND, system_state->current_floor,0);
		orders_clearAt(system_state, system_state->current_floor);
			
    	if (system_state->current_floor < 3)
    		elev_set_button_lamp (BUTTON_CALL_UP, system_state->current_floor,0);
		if (system_state->current_floor > 0)
			elev_set_button_lamp (BUTTON_CALL_DOWN, system_state->current_floor,0);
			
        return idle;
    }
        
        
    else if (fsm_previous_state == going_down) {
        for (int i = system_state->current_floor; i >= 0; --i){
	    	if (system_state->elev_active_floors[i] ||
	    	    system_state->elev_called_floors_up[i] ||
	    	    system_state->elev_called_floors_down[i]){
	    		system_state->motor_direction = -1;
	        	return going_down;
	    	}
        }
		return idle;
    }


    else if (fsm_previous_state == going_up){
        for (int i = system_state->current_floor; i < N_FLOORS; ++i){
	    	if (system_state->elev_active_floors[i] ||
	    	    system_state->elev_called_floors_up[i] ||
	    	    system_state->elev_called_floors_down[i]){
	    		system_state->motor_direction = 1;	
	        	return going_up;
	    	}
        }
    }

    return idle;
}

//-----------------------------------------------------------------------------------

States fsm_going_down(system_state_t* system_state, States fsm_previous_state){

	if (elev_get_stop_signal()){
		return stop;
	}
	
	system_state->next_floor = system_state->current_floor + system_state->motor_direction;
    
    elev_set_door_open_lamp(0);
    elev_set_motor_direction(DIRN_DOWN);
    
    check_cmd_buttons(system_state);
    check_call_buttons(system_state);
	
	check_floor(system_state);
    if (elev_get_floor_sensor_signal() >= 0) { 
		elev_set_floor_indicator(system_state->current_floor);
		
		if (should_stop(system_state, system_state->current_floor)){

            orders_clearAt(system_state, system_state->current_floor);
	    	reset_button_indicators(system_state);
			return door_open;
		}
	
    }  
    return going_down;
}

//----------------------------------------------------------------------------------- 

States fsm_going_up(system_state_t* system_state, States fsm_previous_state){

	if (elev_get_stop_signal()){
		return stop;
	}
	
	system_state->next_floor = system_state->current_floor + system_state->motor_direction;
	
    elev_set_door_open_lamp(0);
    elev_set_motor_direction(DIRN_UP);
    
    check_cmd_buttons(system_state);
    check_call_buttons(system_state);

	check_floor(system_state);
    if (elev_get_floor_sensor_signal() >= 0) { 	
		
		elev_set_floor_indicator(system_state->current_floor);
		
		
		if (should_stop(system_state, system_state->current_floor)){
			elev_set_direction(system_state, 0);
            orders_clearAt(system_state, system_state->current_floor);
	    	reset_button_indicators(system_state);     	
			return door_open;
		}
    }  
    return going_up;
}

//-----------------------------------------------------------------------------------

States fsm_stop_button_pressed(system_state_t* system_state, States fsm_previous_state){
    
    system_state->next_floor = system_state->current_floor + system_state->motor_direction;

	elev_set_motor_direction(DIRN_STOP);
	reset_active_floors(system_state);
	reset_called_floors(system_state);
	initialize_button_indicators(system_state);
	
	if (elev_get_floor_sensor_signal() >= 0){
		elev_set_stop_lamp(1);
		elev_set_door_open_lamp(1);
		
		if (!elev_get_stop_signal()){
			elev_set_stop_lamp(0);
			return idle;
		}
		return stop;
	}
	
	else{
		
		while (elev_get_stop_signal())
			elev_set_stop_lamp(1);
			
		if (!elev_get_stop_signal()){
			elev_set_stop_lamp(0);
    		
    		check_cmd_buttons(system_state);
    
   			for (int i = 0; i < N_FLOORS; ++i){
        		if (system_state->elev_active_floors[i] == 1){
            		if (i < system_state->current_floor){
               			return going_down;
            		}           
            		else if (i > system_state->current_floor){      
               			return going_up;
            		}
            		else{ // if i is the floor currently indicated
            			if (system_state->next_floor > system_state->current_floor){
            				return going_down;
            			}
            			else            				
            				return going_up;
            		}
        		}
    		}
	
			check_call_buttons(system_state);
		
			for (int i = 0; i < N_FLOORS; ++i){
				if (system_state->elev_called_floors_down[i] || 
					system_state->elev_called_floors_up[i]){
					if (i < system_state->current_floor){
					
						return going_down;
					}
					else if (i > system_state->current_floor){
						return going_up;
					}
            		else{ // if i is the floor currently indicated0
            			if (system_state->next_floor > system_state->current_floor)
            				return going_down;
            			else 
            				return going_up;
            		}					
				}	
			}	
		}
	}

	return stop;
}
	



