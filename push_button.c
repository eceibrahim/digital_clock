#include "FreeRTOS.h"
#include "task.h"
#include "push_button.h"
#include "gpio.h" 
#include "Port.h"

#define PB_MAX_N                        (3)
#define PB_SAMPLES_N                    (5)
#define PB_RELEASED_VOLTAGE_LEVEL       (1)
#define PB_PRESSED_VOLTAGE_LEVEL        (0)


typedef struct pb_info_tag
{
	pb_state_t state;
  uint8_t samples[PB_SAMPLES_N];
  uint16_t pressed_time ;
  uint16_t released_time;
}pb_info_t;

static pb_info_t pb_data[PB_MAX_N];

void pb_init(void)
{
	pb_t push_button ;
	
	uint8_t sample_index ;
	
	for (push_button = PB_SEL ; push_button < PB_MAX_N ; ++ push_button)
	{
		switch(push_button)
		{
			case PB_SEL :
				/*Initialize PB_SEL port as input*/
			gpio_init_portpin(PB_SEL_PORT ,PB_SEL_PIN ,GPIO_Input);
			break ;
			case PB_PLUS :
				/*Initialize PB_PLUS port as input */
			gpio_init_portpin(PB_PLUS_PORT ,PB_PLUS_PIN ,GPIO_Input);
			break ;
			case PB_MINUS :
				/*Initialize PB_MINUS port as input*/
			gpio_init_portpin(PB_MINUS_PORT ,PB_MINUS_PIN ,GPIO_Input);
			break ;
			default : 
				/*Should not be here*/
			break;
		}
		
		for (sample_index = 0; sample_index <PB_SAMPLES_N ; ++sample_index)
		{
			pb_data[push_button].samples[sample_index]= PB_RELEASED_VOLTAGE_LEVEL ;
		}
		
		pb_data[push_button].released_time = 0;
		
		pb_data[push_button].pressed_time = 0;
		
		pb_set_state(push_button , PB_RELEASED);
		
	}
}


void pb_update(void *param)
{
	pb_t push_button ;
	uint8_t sample_index ; 
	
	for(;;)
	{
		for (push_button = PB_SEL ; push_button < PB_MAX_N ; ++ push_button)
		{
			/*Update push button samples*/
			for (sample_index = 0; sample_index < PB_SAMPLES_N - 1 ; ++ sample_index)
			{
				pb_data[push_button].samples[sample_index] = pb_data[push_button].samples[sample_index+1];
			}
			switch(push_button) 
			{
				case PB_SEL :
					/*Read PB_SEL port pin*/
				pb_data[push_button].samples[sample_index] = gpio_get_portpinstate(PB_SEL_PORT , PB_SEL_PIN);
				
				break ;
				case PB_PLUS :
					/*Read PB_PLUS port pin*/
				pb_data[push_button].samples[sample_index] = gpio_get_portpinstate(PB_PLUS_PORT , PB_PLUS_PIN);
				break ;
				case PB_MINUS :
					/*Read PB_MINUS port pin*/
				pb_data[push_button].samples[sample_index] = gpio_get_portpinstate(PB_MINUS_PORT , PB_MINUS_PIN);	
				break ;
				default :
					/*Should not be here*/
				break ;
			}
			/*Update push button state*/
			switch(pb_data[push_button].state)
			{
				case PB_RELEASED :
					if((pb_data[push_button].samples[4]==PB_PRESSED_VOLTAGE_LEVEL) &&
						 (pb_data[push_button].samples[3]==PB_PRESSED_VOLTAGE_LEVEL) &&
						 (pb_data[push_button].samples[1]==PB_RELEASED_VOLTAGE_LEVEL)&&
						 (pb_data[push_button].samples[0]==PB_RELEASED_VOLTAGE_LEVEL))
					{
						pb_set_state(push_button , PB_PRE_PRESSED);
						pb_data[push_button].released_time =0;
					}
					else 
					{
						if( pb_data[push_button].released_time < PB_RELEASED_TIME_MAX_MS)
						{
							pb_data[push_button].released_time += PB_TASK_PERIOD_MS ;
						}
					}
					break ;
				case PB_PRE_PRESSED :
					if((pb_data[push_button].samples[4]==PB_PRESSED_VOLTAGE_LEVEL)&&
						 (pb_data[push_button].samples[3]==PB_PRESSED_VOLTAGE_LEVEL)&&
					   (pb_data[push_button].samples[2]==PB_PRESSED_VOLTAGE_LEVEL)) 
					{
						pb_set_state(push_button,PB_PRESSED); 
						
					}
					else 
					{
						/*Should not be here*/
					}
				break ;
				case PB_PRESSED:
					if((pb_data[push_button].samples[4]==PB_RELEASED_VOLTAGE_LEVEL)&&
						 (pb_data[push_button].samples[3]==PB_RELEASED_VOLTAGE_LEVEL)&&
					   (pb_data[push_button].samples[1]==PB_PRESSED_VOLTAGE_LEVEL) &&
					   (pb_data[push_button].samples[0]==PB_PRESSED_VOLTAGE_LEVEL) )
					{
						pb_set_state(push_button,PB_PRE_RELEASED);
						pb_data[push_button].pressed_time=0;
					}
					else 
					{
						if(pb_data[push_button].released_time < PB_PRESSED_TIME_MAX_MS)
						{
							pb_data[push_button].pressed_time += PB_TASK_PERIOD_MS ;
						}
					}
					break ;
				case PB_PRE_RELEASED :
					if ((pb_data[push_button].samples[4]==PB_RELEASED_VOLTAGE_LEVEL)&&
						  (pb_data[push_button].samples[3]==PB_RELEASED_VOLTAGE_LEVEL)&& 
					    (pb_data[push_button].samples[2]==PB_RELEASED_VOLTAGE_LEVEL))
					{
						pb_set_state(push_button , PB_RELEASED);
					}
					else 
					{
						/*Should not be here*/
					}
					break;
				default :
					/*Should not be here*/
				break ;
				
			}
			
		}
		
		/*Wait for PB_TASK_PERIOD_MS */
		vTaskDelay(PB_TASK_PERIOD_MS /  portTICK_PERIOD_MS );
		
	}
	
}


void pb_set_state(pb_t push_button , pb_state_t state)
{
	
	pb_data[push_button].state = state ;
	
}


pb_state_t pb_get_state(pb_t push_button)
{
	pb_state_t ret = PB_RELEASED ;
	
	ret = pb_data[push_button].state ;
	
	return ret ;
}


uint16_t pb_get_pressedtime(pb_t push_button)
{
	uint16_t ret =0;
	
	ret = pb_data[push_button].pressed_time ;
	
	return ret ;
}


uint16_t pb_get_releasedtime(pb_t push_button)
{
	uint16_t ret = 0 ;
	
	ret = pb_data[push_button].released_time ;
	
	return ret ;
}



