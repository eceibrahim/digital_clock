#ifndef __SSD_H__
#define __SSD_H__

#include "Main.h"


#define SSD_TASK_PHASE_MS              (10)
#define SSD_TASK_PERIOD_MS             (5)
#define SSD_TASK_PRIORITY              (1)
#define SSD_TASK_STACK_SIZE            (33)

typedef enum ssd_tag 
{
	SSD_HOUR_TENS  = 0 ,
	SSD_HOUR_UNITS , 
	SSD_MINTUE_TENS , 
	SSD_MINTUE_UNITS
}ssd_t ;

typedef enum ssd_state_tag
{
	SSD_ON = 0 ,
	SSD_OFF
}ssd_state_t ;

typedef enum ssd_dot_state_tag 
{
	SSD_DOT_OFF =0 ,
	SSD_DOT_ON 
}ssd_dot_state_t ;

typedef enum ssd_symbol_tag 
{
	SSD_0 = 0 ,
	SSD_1 ,
	SSD_2 ,
	SSD_3 ,
	SSD_4 ,
	SSD_5 ,
	SSD_6 ,
	SSD_7 ,
	SSD_8 ,
	SSD_9 ,
	SSD_NULL 
}ssd_symbol_t;


void ssd_init(void);
void ssd_update(void *param);
void ssd_set_state(ssd_t ssd, ssd_state_t state);
ssd_state_t ssd_get_state(ssd_t ssd);
void ssd_set_symbol(ssd_t ssd , ssd_symbol_t symbol);
ssd_symbol_t  ssd_get_symbol(ssd_t ssd );
void ssd_set_data(uint8_t data);
void ssd_dot_set_state(ssd_dot_state_t state) ;
ssd_dot_state_t ssd_dot_get_state(void);

#endif /*__SSD_H__*/