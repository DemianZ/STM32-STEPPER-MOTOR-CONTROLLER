#include "stm32f0xx.h"
#include "tim.h"

volatile uint16_t steps_gl = 0;
volatile uint16_t microsteps_gl = 0;

//Таймер для подсчета шагов
void TIM6_Init(void) {          
  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;       //Включить тактирование таймера
  TIM6->PSC = 47;             //1 МГц - частота таймера APBCLK   
  TIM6->ARR = 1000; //Частота срабатывания прерывания: 100.000/100 = 200 Гц
  TIM6->DIER |= TIM_DIER_UIE;             //Разрешаем прерывание от таймера
  TIM6->CR1 |= TIM_CR1_ARPE;
  NVIC_EnableIRQ(TIM6_DAC_IRQn);
  NVIC_SetPriority(TIM6_DAC_IRQn, 2);
  TIM6->CR1 = (TIM_CR1_CEN);//Включить счетчик!
}
//обработчик прерываний таймера подсчета шагов
void TIM6_DAC_IRQHandler(void) { 
  
  steps_gl++;        //Глобальная переменная для счетчика в режиме полного шага
  if(steps_gl >= 65535)
  {
    steps_gl = 0;
  }
  microsteps_gl++;  //Глобальная переменная для счетчика в режиме микрошага
  if(microsteps_gl >= 65535)
  {
    microsteps_gl = 0;
  }
  TIM6->SR &= ~TIM_SR_UIF; 
}

void TIM2_PWM_Init()    //ШИМ для канала 1 (PA0..PA3)
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //Включить тактирование таймера
  
  TIM2->PSC = 1;    //APBCLK=48 MHz
  TIM2->ARR = 65535; //Период срабатывания прерывания:f=48000000/4800=10 кГц
  TIM2->CCR1=0;   //Изменить заполнение ШИМ канала 1
  TIM2->CCR2=0;   //Изменить заполнение ШИМ канала 2
  TIM2->CCR3=0;   //Изменить заполнение ШИМ канала 3
  TIM2->CCR4=0;   //Изменить заполнение ШИМ канала 3
  
  TIM2->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE;  //Выбор режима ШИМ для 1 канала (110-прямой ШИМ) + разрешить "предзагрузку" значений
  TIM2->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE;  //Выбор режима ШИМ для 2 канала (110)        
  TIM2->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE;  //Выбор режима ШИМ для 3 канала (110) 
  TIM2->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4PE;  //Выбор режима ШИМ для 3 канала (110) 
  
  TIM2->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E |  TIM_CCER_CC4E;  //Разрешить ШИМ использовать выходную ножку
  TIM2->BDTR |= TIM_BDTR_MOE;                   //break and dead-time register - Main output enable
  TIM2->CR1 |= TIM_CR1_CMS_0 | TIM_CR1_CEN;     //Edge-aligned mode | Counter enable
  TIM2->EGR |= TIM_EGR_UG;                      //Инициализация
}


void TIM3_PWM_Init()    //ШИМ для канала 2 
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; //Включить тактирование таймера
  
  TIM3->PSC = 1;    //APBCLK=1 MHz
  TIM3->ARR = 65535; //Период срабатывания прерывания:f=48000000/4800=10 кГц
  TIM3->CCR1=0;   //Изменить заполнение ШИМ канала 1
  TIM3->CCR2=0;   //Изменить заполнение ШИМ канала 2
  TIM3->CCR3=0;   //Изменить заполнение ШИМ канала 3
  TIM3->CCR4=0;   //Изменить заполнение ШИМ канала 3
  
  TIM3->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE;  //Выбор режима ШИМ для 1 канала (110-прямой ШИМ) + разрешить "предзагрузку" значений
  TIM3->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE;  //Выбор режима ШИМ для 2 канала (110)        
  TIM3->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE;  //Выбор режима ШИМ для 3 канала (110) 
  TIM3->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4PE;  //Выбор режима ШИМ для 3 канала (110) 
  
  TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E |  TIM_CCER_CC4E;  //Разрешить ШИМ использовать выходную ножку
  TIM3->BDTR |= TIM_BDTR_MOE;                   //break and dead-time register - Main output enable
  TIM3->CR1 |= TIM_CR1_CMS_0 | TIM_CR1_CEN;     //Edge-aligned mode | Counter enable
  TIM3->EGR |= TIM_EGR_UG;                      //Инициализация
}

void SysTick_Handler(void)
{ 
 /* microsteps_gl++;
  if(microsteps_gl == 65534)
  {
    microsteps_gl = 0;
  }*/
} 
void delay (int a)
{
  volatile int i,j,k;                     //volatile denies optimisation
  for (i=0 ; i < a ; i++)
  {
    for (j=0 ; j < a ; j++)
    {
      k++;
    }
    k++;
  }
}