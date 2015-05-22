

#include <unistd.h>
#include "elevator_state.h"
#include "buttons.h"
#include "timer.h"
#include "elev.h"


// returns 0 if pause interrupted by stop button
int fsm_pause_3_sec(system_state_t* system_state){ 

	int t =  0;
	
	while (t <= 3000) { 
		check_cmd_buttons(system_state);
		check_call_buttons(system_state);
		
		if (elev_get_stop_signal())
			return 0;
		
		if (t >= 3000)
			break;
		

		usleep(1000 * 100);
		t += 100;
	}
	return 1;
}



