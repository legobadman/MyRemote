#ifndef __KEY_H__
#define __KEY_H__

#include "sys.h"

#define S81_PIN	GPIO_Pin_1
#define S82_PIN	GPIO_Pin_3

#define KEY_PORT GPIOB

void KEY_init(void);

#endif
