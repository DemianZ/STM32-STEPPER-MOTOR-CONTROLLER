#ifndef __tim_H
#define __tim_H

#define BSRR_BLINKLED_VAL 0x0300
#define ARR_FULLSTEP_VAL (240-1)
#define ARR_HALFSTEP_VAL (240/2 - 1)

void TIM2_PWM_Init();
void TIM3_PWM_Init();
void TIM6_Init();

void delay (int a);

#endif