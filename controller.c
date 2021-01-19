#include "FreeRTOS.h"
#include "task.h"
#include "controller.h"
#include "push_button.h" 
#include "disp.h"



#define DIGITAL_CLOCK_SECOND_MS          (1000)

static digital_clock_time_t  digital_clock_time ;
static digital_clock_state_t digital_clock_state ;
static uint16_t  digital_clock_counter_ms  ;

void digital_clock_init(void)
{
	digital_clock_time.hours =0;
	digital_clock_time.mintues =0;
	digital_clock_time.seconds =0;
	digital_clock_set_state(DIGITAL_CLOCK_NORMAL);
	digital_clock_counter_ms = 0 ;
}

void digital_clock_update(void * param)
{
	digital_clock_state_t state  ;
	
	vTaskDelay(DIGITAL_CLOCK_TASK_PHASE_MS /  portTICK_PERIOD_MS );

	for(;;)
	{
		
		state = digital_clock_get_state();

		switch(state)
		{
			case DIGITAL_CLOCK_NORMAL :
				if((pb_get_state(PB_SEL)==PB_PRESSED)&&(pb_get_pressedtime(PB_SEL) == PB_PRESSED_TIME_MAX_MS))
				{
					disp_set_blinkcounter(0);

					digital_clock_set_state(DIGITAL_CLOCK_SET_HOURS);
					
				}
				else 
				{
					if(digital_clock_counter_ms < DIGITAL_CLOCK_SECOND_MS)
					{
						digital_clock_counter_ms += DIGITAL_CLOCK_TASK_PERIOD_MS ;
						
					}
					else 
					{
						digital_clock_counter_ms =0;
						digital_clock_time.seconds++;
						if(digital_clock_time.seconds == 60)
						{
							digital_clock_time.seconds =0;
							digital_clock_time.mintues ++;
							if(digital_clock_time.mintues == 60)
							{
								digital_clock_time.mintues =0;
								digital_clock_time.hours ++;
								if(digital_clock_time.hours == 24)
								{
									digital_clock_time.hours = 0;
								}
							}
						}
					}
				}
			break ;
			case DIGITAL_CLOCK_SET_HOURS :
				if((pb_get_state(PB_SEL) == PB_PRESSED) && (pb_get_pressedtime(PB_SEL) == PB_PRESSED_TIME_MAX_MS))
				{
					disp_set_blinkcounter(0);
					digital_clock_set_state(DIGITAL_CLOCK_SET_MINTUES);
				}
				else 
				{
					if(pb_get_state(PB_MINUS) == PB_PRE_PRESSED )
					{
						if(digital_clock_time.hours == 0)
						{
							digital_clock_time.hours = 23;
						}
						else 
						{
							digital_clock_time.hours --;
						}
					}
					else if(pb_get_state(PB_PLUS) == PB_PRE_PRESSED)
					{
						digital_clock_time.hours =(digital_clock_time.hours + 1 ) % 24;
					}
					else 
					{
						/*Should not be here*/
					}
				}

			break ;
			case DIGITAL_CLOCK_SET_MINTUES :
				if((pb_get_state(PB_SEL) == PB_PRESSED) && (pb_get_pressedtime(PB_SEL) == PB_PRESSED_TIME_MAX_MS))
				{
					disp_set_blinkcounter(0);
					digital_clock_counter_ms =0;
					digital_clock_time.seconds=0;
					digital_clock_set_state(DIGITAL_CLOCK_NORMAL);
				}
				else 
				{
					if(pb_get_state(PB_MINUS) == PB_PRE_PRESSED)
					{
						if(digital_clock_time.mintues == 0)
						{
							digital_clock_time.mintues = 59 ;
						}
						else 
						{
							digital_clock_time.mintues --;
						}
					}
					else if(pb_get_state(PB_PLUS) == PB_PRE_PRESSED)
					{
						digital_clock_time.mintues=(digital_clock_time.mintues + 1 ) % 60;
					}
					else 
					{
						/*Should not be here*/
					}
				
				}
			
			break ;
			default :
				/*Should not be here*/
			break;
		}
	
		vTaskDelay(DIGITAL_CLOCK_TASK_PERIOD_MS /  portTICK_PERIOD_MS );
		
	}
	
}

void digital_clock_set_state(digital_clock_state_t state)
{
	digital_clock_state = state; 
}

digital_clock_state_t digital_clock_get_state(void)
{
	digital_clock_state_t ret = digital_clock_state ;
	
	return ret ; 
}

void digital_clock_get_time(digital_clock_time_t *pt)
{
	pt->hours = digital_clock_time.hours ;
	
	pt->mintues = digital_clock_time.mintues;
	
	pt->seconds = digital_clock_time.seconds;
}
