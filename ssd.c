#include "FreeRTOS.h"
#include "task.h"
#include "ssd.h"
#include "Port.h" 
#include "gpio.h"

#define SSD_MAX_N            (4)

ssd_symbol_t ssd_symbol[SSD_MAX_N];

static ssd_dot_state_t ssd_dot_state ;

const uint8_t ssd_data[] =
{
    0b00111111,
    0b00000110,
    0b01011011,
    0b01001111,
    0b01100110,
    0b01101101,
    0b01111101,
    0b00000111,
    0b01111111,
    0b01101111,
    0b00000000
};

void ssd_init(void)
{
	
	ssd_t current_ssd ;
	uint16_t current_pin =0x0001; 
	uint8_t loopi; 
	
	/*Set data port pins as output*/
	for (loopi =0 ;loopi < 8 ;++loopi)
	{
		gpio_init_portpin(SSD_DATA_PORT,current_pin ,GPIO_Output);
		current_pin <<= 0x01;
	}
	
	/*Initialze data port*/
	ssd_set_data(ssd_data[SSD_NULL]);
		
	for (current_ssd = SSD_HOUR_TENS  ; current_ssd < SSD_MAX_N  ; ++current_ssd)
	{
		/*Set ssd control port pins as output*/
		switch(current_ssd)
		{
			case SSD_HOUR_TENS :
			gpio_init_portpin(SSD_HOUR_TENS_PORT ,SSD_HOUR_TENS_PIN ,GPIO_Output);
			break;
			case SSD_HOUR_UNITS :
			gpio_init_portpin(SSD_HOUR_UNITS_PORT ,SSD_HOUR_UNITS_PIN ,GPIO_Output);
			break;
			case SSD_MINTUE_TENS :
			gpio_init_portpin(SSD_MINTUE_TENS_PORT ,SSD_MINTUE_TENS_PIN ,GPIO_Output);
			break ;
			case SSD_MINTUE_UNITS :
			gpio_init_portpin(SSD_MINTUE_UNITS_PORT ,SSD_MINTUE_UNITS_PIN ,GPIO_Output);
			break ;
			default :
				/*Should not be here*/
			break ;
		}
		/*intialize ssd_symbol with SSD_NULL*/
		ssd_set_symbol(current_ssd ,SSD_NULL);
		
		/*intialize SSDs state with SSD_OFF*/
		ssd_set_state(current_ssd,SSD_OFF);
	}
	ssd_dot_set_state(SSD_DOT_OFF);
	
}

void ssd_update(void *param)
{
	static ssd_t current_ssd ;
	
	ssd_t ssd_index ;
	
	/*Delay for SSD_TASK_PHASE_MS*/
	vTaskDelay(SSD_TASK_PHASE_MS / portTICK_PERIOD_MS); 
	
	for(;;)
	{
		
		/*Turn off all ssds*/
		for(ssd_index = SSD_HOUR_TENS ; ssd_index < SSD_MAX_N ; ++ssd_index)
		{
			ssd_set_state(ssd_index , SSD_OFF);
		}
		
		/*Put data*/
		ssd_set_data(ssd_data[ssd_symbol[current_ssd]]);
		
		/*Turn on current ssd*/
		ssd_set_state(current_ssd ,SSD_ON) ;
		
		/*Set the next ssd to current_ssd*/
		if(current_ssd == SSD_MINTUE_UNITS)
		{
			current_ssd =SSD_HOUR_TENS ;
		}
		else 
		{
			current_ssd++;
		}
		
		vTaskDelay(SSD_TASK_PERIOD_MS / portTICK_PERIOD_MS);
		
	}
}

void ssd_set_state(ssd_t ssd, ssd_state_t state)
{
	switch(ssd)
	{
		case SSD_HOUR_TENS :
			/*Set SSD_HOUR_TENS port pin state*/
		gpio_set_portpinstate(SSD_HOUR_TENS_PORT ,SSD_HOUR_TENS_PIN, state);
		break ;
		case SSD_HOUR_UNITS :
			/*Set SSD_HOUR_UNITS port pin state*/
		gpio_set_portpinstate(SSD_HOUR_UNITS_PORT ,SSD_HOUR_UNITS_PIN, state);
		  /*Set ssd dot port pin based on ssd_dot_state */
		gpio_set_portpinstate(SSD_DOT_PORT ,SSD_DOT_PIN , ssd_dot_state);
		
		break;
		case SSD_MINTUE_TENS :
			/*Set SSD_MINTUE_TENS port pin state*/
		gpio_set_portpinstate(SSD_MINTUE_TENS_PORT ,SSD_MINTUE_TENS_PIN, state);
		break;
		case SSD_MINTUE_UNITS :
			/*Set SSD_MINTUE_UNITS port pin state*/
		gpio_set_portpinstate(SSD_MINTUE_UNITS_PORT ,SSD_MINTUE_UNITS_PIN, state);
		break ;
		default :
			/*Should not be here*/
		break;
	}
	
}
ssd_state_t ssd_get_state(ssd_t ssd)
{
	ssd_state_t ret = SSD_OFF ;
	switch(ssd)
	{
		case SSD_HOUR_TENS :
			/*Get SSD_HOUR_TENS port pin state */
		ret = gpio_get_portpinstate(SSD_HOUR_TENS_PORT, SSD_HOUR_TENS_PIN);
		break; 
		case SSD_HOUR_UNITS:
			/*Get SSD_HOUR_UNITS port pin state */
		ret = gpio_get_portpinstate(SSD_HOUR_UNITS_PORT, SSD_HOUR_UNITS_PIN);
		break ;
		case SSD_MINTUE_TENS :
			/*Get SSD_MINTUE_TENS port pin state */
		ret= gpio_get_portpinstate(SSD_MINTUE_TENS_PORT, SSD_MINTUE_TENS_PIN);
		break ;
		case SSD_MINTUE_UNITS :
			/*Get SSD_MINTUE_TENS port pin state */
		ret = gpio_get_portpinstate(SSD_MINTUE_UNITS_PORT, SSD_MINTUE_UNITS_PIN);
		break ;
		default :
			/*Should not be here*/
		break ;
	}
	return ret ; 
}


void ssd_set_symbol(ssd_t ssd , ssd_symbol_t symbol)
{
	ssd_symbol[ssd] = symbol ;
}

ssd_symbol_t  ssd_get_symbol(ssd_t ssd )
{
	ssd_symbol_t ret = SSD_NULL ;
	ret = ssd_symbol[ssd] ;
	return ret ;
}

void ssd_set_data(uint8_t data)
{
	/*Set port pins with data */
	uint8_t tmp ; 
	uint16_t current_pin = 0x0001;
	uint8_t loopi ;
	for (loopi = 0 ; loopi < 8 ; ++loopi)
	{
		tmp  = data & 0x01 ;
		data >>=0x01;
		gpio_set_portpinstate(SSD_DATA_PORT ,current_pin , tmp);
		current_pin <<= 0x01 ;
	}
	
}

void ssd_dot_set_state(ssd_dot_state_t state) 
{
	/*Set dot port pin state*/
	ssd_dot_state =state ; 
}

ssd_dot_state_t ssd_dot_get_state(void)
{
	ssd_dot_state_t ret = SSD_DOT_OFF;
	
	/*Get dot port pin state*/
	ret = ssd_dot_state ; 
	
	return ret ;
}