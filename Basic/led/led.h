#ifndef __LED_H__
#define __LED_H__

#include "sys.h"

#define LED_PORT GPIOB

#define LED82_Pin GPIO_Pin_4
#define LED83_Pin GPIO_Pin_5

void LED_Init(void);

#endif
