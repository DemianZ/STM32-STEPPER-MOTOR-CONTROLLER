#include "stm32f0xx.h"
#include "motor.h"
#include "tim.h"
#include "gpio.h"

static uint16_t cur_pos;
static uint8_t step_mode = 0;

extern  volatile uint16_t steps_gl;
extern  volatile uint16_t microsteps_gl;
static uint16_t prev_steps = 0;
static uint16_t prev_microsteps = 0;

void m1_Move_To(uint16_t pos){
  cur_pos = pos;
}

int m1_Steps_Available(void)
{
  if(cur_pos > 0)
  {
    return 1;
  }
  else
  {
    step_mode = 0;
    return 0;
  }
}
void m1_Run_Fullstep()
{ 
  if(step_mode != 1)
  {
    step_mode = 1;
  }
  
  if (prev_steps != steps_gl)
  {
    prev_steps = steps_gl;
    uint16_t step_num = (steps_gl % 4);
    
    if (step_num == 1) 
    {
      GPIOA->ODR = COIL_A_POS;
      cur_pos--;
      return;
    }
    if (step_num == 2) 
    {
      GPIOA->ODR = COIL_B_POS;
      cur_pos--;
      return;
    }
    if (step_num == 3) 
    {
      GPIOA->ODR = COIL_A_NEG;
      cur_pos--;
      return;
    }
    if (step_num == 0) 
    {
      GPIOA->ODR = COIL_B_NEG;
      cur_pos--;
      return;
    }
  } 
}

void m1_Run_Microstep(uint16_t step_div, float speed)
{
  
  static uint16_t cur_microsteps = 0;
  static uint16_t l_step_num = 0;
  if(step_mode != step_div)
  {
    cur_pos = cur_pos*step_div;
    step_mode = step_div; 
    TIM2->CR1 = TIM_CR1_CEN;   //Включить таймер для ШИМ (TIM2)
  } 
  
  if (cur_microsteps != microsteps_gl)
  {
    
    cur_microsteps = microsteps_gl; 
    static int32_t ch1_val=1;
    static int32_t ch2_val=0;
    static int32_t ch3_val=0;
    static int32_t ch4_val = 65535; 
    uint16_t add_var = (PWM_MAX / step_div);
    if ((l_step_num >= 0)&&(l_step_num < step_div))
    {
      ch2_val = 0;
      ch1_val += add_var;
      ch4_val -= add_var;
    }
    if ((l_step_num >= step_div)&&(l_step_num < step_div*2))
    {
      if (ch1_val > PWM_MAX)
      {
        ch1_val = PWM_MAX;
      }
      ch4_val = 0;
      ch3_val += add_var;
      ch1_val -= add_var;
    }
    if((l_step_num >= step_div*2)&&(l_step_num < step_div*3))
    {
      if (ch3_val > PWM_MAX)
      {
        ch3_val = PWM_MAX;
      }
       ch1_val = 0;
      ch3_val -= add_var;
      ch2_val += add_var;
    }
    
    if((l_step_num >= step_div*3)&&(l_step_num < step_div*4))
    {
      if (ch2_val > PWM_MAX)
      {
        ch2_val = PWM_MAX;
      }
      ch3_val = 0;
      ch2_val -= add_var;
      ch4_val += add_var;
    }       
    TIM2->CCR1 = ch1_val;
    TIM2->CCR2 = ch2_val;
    TIM2->CCR3 = ch3_val;
    TIM2->CCR4 = ch4_val;
    
    if (l_step_num == 4*step_div-1)
    {
      l_step_num=0;
    }
    else 
    {
      l_step_num++;
    }
    
    cur_pos--;
    if (cur_pos == 0)
    {
      ch1_val=1;
      ch2_val=0;
      ch3_val=0;
      ch4_val=65535;
      TIM2->CCR1 = ch1_val;
      TIM2->CCR2 = ch2_val;
      TIM2->CCR3 = ch3_val;
      TIM2->CCR4 = ch4_val;
      step_mode = 0;
      //TIM2->CR1 &= (~TIM_CR1_CEN);
    }
  } 
}


void Motor_CH1_Init(uint8_t microstep)
{
  GPIO_CH1_Driver_Init(microstep);
  if (microstep != 0)
  {
    TIM2_PWM_Init();  //PA0..PA3
  }
}

void m2_Init(uint8_t microstep)
{
  GPIO_CH2_Driver_Init(microstep);
  if (microstep != 0)
  {
    TIM3_PWM_Init();  //PB4,PB5,PB0,PB1
  }
}

