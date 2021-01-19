#include "FreeRTOS.h"
#include "task.h"
#include "disp.h"
#include "controller.h"
#include "ssd.h"

#define DISP_BLINK_UPDATE_PERIOD_MS    (1000)

static uint16_t disp_blink_counter_ms  ;


void disp_init(void)
{
	
	disp_blink_counter_ms =0;
	
}


void disp_update(void *param)
{
	digital_clock_time_t clock_time ; 
	digital_clock_state_t clock_state ; 
	
	vTaskDelay(DISP_TASK_PHASE_MS /  portTICK_PERIOD_MS );
	
	for(;;)
	{
		clock_state = digital_clock_get_state();
		digital_clock_get_time(&clock_time);
		
		disp_blink_counter_ms += DISP_TASK_PERIOD_MS ;
		
		switch(clock_state)
		{
			case DIGITAL_CLOCK_NORMAL :
			ssd_set_symbol(SSD_HOUR_TENS , clock_time.hours/10);
			ssd_set_symbol(SSD_HOUR_UNITS , clock_time.hours%10);
			ssd_set_symbol(SSD_MINTUE_TENS ,clock_time.mintues / 10);
			ssd_set_symbol(SSD_MINTUE_UNITS ,clock_time.mintues % 10);
			
			if(disp_blink_counter_ms == DISP_BLINK_UPDATE_PERIOD_MS)
			{
				disp_set_blinkcounter(0);
				if(ssd_dot_get_state() == SSD_DOT_OFF)
				{
					ssd_dot_set_state(SSD_DOT_ON);
				}
				else if (ssd_dot_get_state() == SSD_DOT_ON)
				{
					ssd_dot_set_state(SSD_DOT_OFF);
				}
				else 
				{
					/*Should not be here*/
				}
			}
			break ;
			
			case DIGITAL_CLOCK_SET_HOURS :
			ssd_dot_set_state(SSD_DOT_OFF);
			if(disp_blink_counter_ms == DISP_BLINK_UPDATE_PERIOD_MS)
			{
				disp_set_blinkcounter(0);
				if((ssd_get_symbol(SSD_HOUR_TENS) == SSD_NULL) && (ssd_get_symbol(SSD_HOUR_UNITS) == SSD_NULL) )
				{
					ssd_set_symbol(SSD_HOUR_TENS , clock_time.hours/10);
					ssd_set_symbol(SSD_HOUR_UNITS , clock_time.hours%10);
				}
       	else
				{
					ssd_set_symbol(SSD_HOUR_TENS,SSD_NULL);
					ssd_set_symbol(SSD_HOUR_UNITS ,SSD_NULL);
				}
			}
			ssd_set_symbol(SSD_MINTUE_TENS ,clock_time.mintues / 10);
			ssd_set_symbol(SSD_MINTUE_UNITS ,clock_time.mintues % 10);
			break ;
			
			case DIGITAL_CLOCK_SET_MINTUES :
			ssd_dot_set_state(SSD_DOT_OFF);
			
			if(disp_blink_counter_ms == DISP_BLINK_UPDATE_PERIOD_MS)
			{
				disp_set_blinkcounter(0); 
				if((ssd_get_symbol(SSD_MINTUE_TENS)==SSD_NULL )&&(ssd_get_symbol(SSD_MINTUE_UNITS)==SSD_NULL ))
				{
					ssd_set_symbol(SSD_MINTUE_TENS , clock_time.mintues/10);
					ssd_set_symbol(SSD_MINTUE_UNITS ,clock_time.mintues % 10);
				}
				else 
				{
					ssd_set_symbol(SSD_MINTUE_TENS ,SSD_NULL);
					ssd_set_symbol(SSD_MINTUE_UNITS ,SSD_NULL);
				}
			}
			ssd_set_symbol(SSD_HOUR_TENS ,clock_time.hours / 10);
			ssd_set_symbol(SSD_HOUR_UNITS ,clock_time.hours % 10);
			break ;
			
			default :
				/*Should not be here*/
			break;
		}
		vTaskDelay(DISP_TASK_PERIOD_MS /  portTICK_PERIOD_MS );
	}
}

void disp_set_blinkcounter(uint16_t value)
{
	disp_blink_counter_ms = value ;
}