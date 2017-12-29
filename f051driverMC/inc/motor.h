#ifndef __motor_H
#define __motor_H

#define COIL_A_POS 0x1
#define COIL_A_NEG 0x2
#define COIL_B_POS 0x4
#define COIL_B_NEG 0x8

#define PWM_MAX 65534

void m1_Move_To(uint16_t pos);
int m1_Steps_Available(void);
void m1_Run_Fullstep();
void m1_Run_Microstep(uint16_t step_div, float speed);
void Motor_CH1_Init(uint8_t microstep);
#endif