#include "push_button.h"
#include "controller.h"
#include "disp.h"
#include "ssd.h"
#include "FreeRTOS.h"
#include "task.h"
#include "peripheral_clock.h"
#include "gpio.h"

void vApplicationIdleHook(void);

int main(void)
{

	peripheral_clock_init();
	pb_init();
	digital_clock_init();
	disp_init();
	ssd_init();
	xTaskCreate(&pb_update ,NULL , PB_TASK_STACK_SIZE , NULL , PB_TASK_PRIORITY , NULL);
	xTaskCreate(&digital_clock_update , NULL , DIGITAL_CLOCK_TASK_STACK_SIZE , NULL ,DIGITAL_CLOCK_TASK_PRIORITY ,NULL);
	xTaskCreate(&disp_update , NULL ,DISP_TASK_STACK_SIZE , NULL , DISP_TASK_PRIORITY,NULL) ;
	xTaskCreate(&ssd_update , NULL , SSD_TASK_STACK_SIZE , NULL , SSD_TASK_PRIORITY , NULL );
	vTaskStartScheduler();
	return 0;
}


void vApplicationIdleHook(void)
{
	/*Sleep and wait for interrupt*/
	__asm("WFI");
}

