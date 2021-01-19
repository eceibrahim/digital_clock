#ifndef __PUSH_BUTTON_H__
#define __PUSH_BUTTON_H__


#include "Main.h"

#define PB_PRESSED_TIME_MAX_MS          (200)
#define PB_RELEASED_TIME_MAX_MS         (200)

#define PB_TASK_PERIOD_MS               (20)
#define PB_TASK_PRIORITY                (4)
#define PB_TASK_STACK_SIZE              (30)

typedef enum pb_tag 
{
	PB_SEL =0 ,
	PB_PLUS ,
	PB_MINUS 
}pb_t;

typedef enum pb_state_tag 
{
	PB_RELEASED =0 ,
	PB_PRE_PRESSED ,
	PB_PRESSED ,
	PB_PRE_RELEASED
}pb_state_t ;

void pb_init(void);
void pb_update(void *param);
void pb_set_state(pb_t push_button , pb_state_t tate);
pb_state_t pb_get_state(pb_t push_button);
uint16_t pb_get_pressedtime(pb_t push_button);
uint16_t pb_get_releasedtime(pb_t push_button);

#endif /*__PUSH_BUTTON_H__*/