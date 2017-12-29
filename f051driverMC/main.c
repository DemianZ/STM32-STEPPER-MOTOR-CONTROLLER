#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "adc.h"
#include "motor.h"

/*------- Variables ------*/
extern volatile uint16_t RegularConvData_Tab[2];
extern volatile uint16_t steps_gl;
extern volatile uint16_t microsteps_gl;
uint16_t prev_steps = 0;
uint16_t prev_microsteps = 0;

volatile uint16_t v_pos = 0;
volatile uint16_t h_pos = 0;
/*-----------------------*/

int main(void)
{
  ADC_Config();
  DMA_Config();
  TIM6_Init();
  Motor_CH1_Init(1);
  GPIO_LED_Init();
   //SysTick_Config(480000);
  __enable_irq(); 
  uint16_t goal_steps = 50;
  
  m1_Move_To(goal_steps); 
  delay(100);
  
  while(1)
  { 
    v_pos = RegularConvData_Tab[0];
    h_pos = RegularConvData_Tab[1];
    //Go MOTOR!
    if(m1_Steps_Available())
    {
      m1_Run_Microstep(4, 1); 
      //m1_Run_Fullstep();
    }
    if(m1_Steps_Available() == 0)
    {
      LEDBlink(1);
      delay(1000);
      LEDBlink(2);
      delay(1000);
      m1_Move_To(goal_steps);
    }
  }
}

void LEDBlink(uint16_t cur_step){
  uint16_t v1 = cur_step%2;
  
  switch(v1){
  case 0:
    GPIOB->BRR = TWOLEDSMASK;
    break;
  case 1: 
    GPIOB->BSRR = TWOLEDSMASK;
    break;  
  }
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{
  while (1)
  {
  }
}
#endif