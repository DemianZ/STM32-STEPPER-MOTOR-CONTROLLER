#include "stm32f0xx.h"
#include "gpio.h"

void GPIO_CH1_Driver_Init(uint8_t microstep)
{ 
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;  //Включить тактирование порта GPIOA
  /*--------------------------------------------------------------------------*/
  if(microstep == 1)
  {
    /*------------PA0..PA3 - AF(PWM, TIM3-CH1,CH2,CH3,CH4)------------*/
    GPIOA->MODER &= ( ~GPIO_MODER_MODER0_0 | ~GPIO_MODER_MODER1_0 | ~GPIO_MODER_MODER2_0 | ~GPIO_MODER_MODER3_0 ); 
    GPIOA->MODER |= (GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1 |GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1 ); 
    GPIOA->AFR[0] |= 0x2222;
  }
  if(microstep == 0)
  {
   /*------------PB4,PB5,PB0,PB1 - OUTPUT(PWM, TIM3-CH1,CH2,CH3,CH4)------------*/
    GPIOA->MODER |= (GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0);   
  }
  GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0 | GPIO_PUPDR_PUPDR1 | GPIO_PUPDR_PUPDR2 | GPIO_PUPDR_PUPDR3);  //отключить подтяжку
}


void GPIO_CH2_Driver_Init(uint8_t microstep)
{ 
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;  //Включить тактирование порта GPIOA
  /*--------------------------------------------------------------------------*/
  if(microstep == 1)
  {
    /*------------PB4,PB5,PB0,PB1 - AF(PWM, TIM3-CH1,CH2,CH3,CH4)------------*/
    GPIOB->MODER |= (GPIO_MODER_MODER4_1 | GPIO_MODER_MODER5_1 |GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1 ); 
    GPIOB->AFR[0] |= 0x110011;
  }
  if(microstep == 0)
  {
   /*------------PB4,PB5,PB0,PB1 - OUTPUT(PWM, TIM3-CH1,CH2,CH3,CH4)------------*/
    GPIOB->MODER |= (GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0);   
  }
  GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR0 | GPIO_PUPDR_PUPDR1 | GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR5);  //отключить подтяжку
}


int GPIO_LED_Init()     //Init LEDs on board
{ 
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;  //Включить тактирование порта GPIOB  
  GPIOB->MODER |= (GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0);  //LEDs pins as output 
  GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);  //отключить подтяжку
  return 1;
}

int GPIO_CH1_Fullstep_Init(void)                    //Init Motor 1 Fullstep mode
{                                                   //PA0..PA3
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_InitTypeDef        GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  return 1;
}

int GPIO_CH2_Fullstep_Init(void)         //Init Motor 2 Fullstep mode
{                                        //PB4,PB5,PB0,PB1 -> A+,A-,B+,B-
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  GPIO_InitTypeDef        GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  return 1;
}
