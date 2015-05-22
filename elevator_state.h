

#ifndef __Heisprosjekt__elevator_state__
#define __Heisprosjekt__elevator_state__

#include <stdio.h>
#include "elev.h"

typedef enum fsm_states_enum{init, idle, door_open, going_down, going_up, stop} States;

typedef struct system_state_struct {
    int motor_direction;    // Down: -1, Up: 1; Stop: 0
    int elev_active_floors[N_FLOORS];
    int elev_called_floors_down[N_FLOORS];
    int elev_called_floors_up[N_FLOORS];
    int current_floor;
    int prev_floor;
    int next_floor;
    
}system_state_t;


void get_elev_direction(void);

States fsm_init_state(system_state_t* system_state, States fsm_previous_state);

States fsm_idle_state(system_state_t* system_state, States fsm_previous_state);

States fsm_door_open(system_state_t* system_state, States fsm_previous_state);

States fsm_going_down(system_state_t* system_state, States fsm_previous_state);

States fsm_going_up(system_state_t* system_state, States fsm_previous_state);

States fsm_stop_button_pressed(system_state_t* system_state, States fsm_previous_state);


#endif /* defined(__Heisprosjekt__elevator_state__) */
