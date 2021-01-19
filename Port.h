#ifndef __PORT_H__
#define __PORT_H__

#include "Main.h"

#define MODULE_PB                 1
#define MODULE_SSD                1

#if MODULE_PB 
#define PB_SEL_PIN                GPIO_Pin_13
#define PB_SEL_PORT               GPIOC     
#define PB_PLUS_PIN               GPIO_Pin_14
#define PB_PLUS_PORT              GPIOC
#define PB_MINUS_PIN              GPIO_Pin_15
#define PB_MINUS_PORT             GPIOC
#endif /*MODULE_PB*/

#if MODULE_SSD
#define SSD_DATA_PORT                GPIOA 
#define SSD_HOUR_TENS_PIN            GPIO_Pin_0
#define SSD_HOUR_TENS_PORT           GPIOB
#define SSD_HOUR_UNITS_PIN           GPIO_Pin_1
#define SSD_HOUR_UNITS_PORT          GPIOB	
#define SSD_MINTUE_TENS_PIN          GPIO_Pin_10
#define SSD_MINTUE_TENS_PORT         GPIOB
#define SSD_MINTUE_UNITS_PIN         GPIO_Pin_11
#define SSD_MINTUE_UNITS_PORT        GPIOB
#define SSD_DOT_PIN                  GPIO_Pin_7              
#define SSD_DOT_PORT                 GPIOA
#endif /*MODULE_SSD*/

#endif /*__PORT_H__*/