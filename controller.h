#ifndef __CONTROLLER_H__ 
#define __CONTROLLER_H__ 

#include "Main.h"
#include "FreeRTOS.h"
#include "task.h"


#define DIGITAL_CLOCK_TASK_PHASE_MS               (5)
#define DIGITAL_CLOCK_TASK_PERIOD_MS              (20) 
#define DIGITAL_CLOCK_TASK_PRIORITY               (3)
#define DIGITAL_CLOCK_TASK_STACK_SIZE             (40)

typedef enum digital_clock_state_tag
{
	DIGITAL_CLOCK_NORMAL =0 ,
	DIGITAL_CLOCK_SET_HOURS ,
	DIGITAL_CLOCK_SET_MINTUES
}digital_clock_state_t;

typedef struct digital_clock_time_tag 
{
	uint8_t seconds ;
	uint8_t mintues;
	uint8_t hours; 
}digital_clock_time_t ;

void digital_clock_init(void);
void digital_clock_update(void * param);
void digital_clock_set_state(digital_clock_state_t state);
digital_clock_state_t digital_clock_get_state(void);
void digital_clock_get_time(digital_clock_time_t *pt);

#endif /*__CONTROLLER_H__*/