#ifndef __DISP_H__
#define __DISP_H__ 

#include "Main.h"

#define DISP_TASK_PHASE_MS     (10)
#define DISP_TASK_PERIOD_MS    (20)
#define DISP_TASK_PRIORITY     (2)
#define DISP_TASK_STACK_SIZE   (70)

void disp_init(void);
void disp_update(void *param);
void disp_set_blinkcounter(uint16_t value);

#endif /*__DISP_H__*/