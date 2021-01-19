#ifndef __GPIO_H__
#define __GPIO_H__

#include "stm32f10x_gpio.h"

typedef enum gpio_mode_tag 
{
	GPIO_Input , 
	GPIO_Output 
}gpio_mode_t ;


void gpio_init_portpin (GPIO_TypeDef* gpiox , uint16_t pin ,gpio_mode_t mode );
void gpio_set_portpinstate(GPIO_TypeDef* gpiox ,uint16_t pin , uint8_t state );
uint8_t gpio_get_portpinstate(GPIO_TypeDef* gpiox , uint16_t  pin);
 
#endif /*__GPIO_H__*/