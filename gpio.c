#include "gpio.h"

void gpio_init_portpin (GPIO_TypeDef* gpiox , uint16_t pin ,gpio_mode_t mode )
{
	GPIO_InitTypeDef gpio_cfgr_pin ;
	gpio_cfgr_pin.GPIO_Pin = pin ;
	gpio_cfgr_pin.GPIO_Speed =GPIO_Speed_10MHz;
	switch(mode)
	{
		case GPIO_Input :
		gpio_cfgr_pin.GPIO_Mode=GPIO_Mode_IPU;
		break ;
		case GPIO_Output :
		gpio_cfgr_pin.GPIO_Mode=GPIO_Mode_Out_PP;
		break ;
		default :
			/*Should not be here*/
		break ;
	}
	GPIO_Init(gpiox, &gpio_cfgr_pin);
}


void gpio_set_portpinstate(GPIO_TypeDef* gpiox ,uint16_t pin , uint8_t state )
{
	switch(state)
	{
		case 1 :
		GPIO_SetBits(gpiox ,pin);
		break ;
		case 0 :
		GPIO_ResetBits(gpiox,pin);
		break;
		default :
			/*Should not be here*/
		break;
	}
}

uint8_t gpio_get_portpinstate(GPIO_TypeDef* gpiox , uint16_t  pin)
{
	uint8_t ret = 0 ;
  ret = GPIO_ReadInputDataBit(gpiox,pin);
  return ret ;	
}