#include "stm32f0xx.h"
#include "tim.h"

volatile uint16_t steps_gl = 0;
volatile uint16_t microsteps_gl = 0;

//������ ��� �������� �����
void TIM6_Init(void) {          
  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;       //�������� ������������ �������
  TIM6->PSC = 47;             //1 ��� - ������� ������� APBCLK   
  TIM6->ARR = 1000; //������� ������������ ����������: 100.000/100 = 200 ��
  TIM6->DIER |= TIM_DIER_UIE;             //��������� ���������� �� �������
  TIM6->CR1 |= TIM_CR1_ARPE;
  NVIC_EnableIRQ(TIM6_DAC_IRQn);
  NVIC_SetPriority(TIM6_DAC_IRQn, 2);
  TIM6->CR1 = (TIM_CR1_CEN);//�������� �������!
}
//���������� ���������� ������� �������� �����
void TIM6_DAC_IRQHandler(void) { 
  
  steps_gl++;        //���������� ���������� ��� �������� � ������ ������� ����
  if(steps_gl >= 65535)
  {
    steps_gl = 0;
  }
  microsteps_gl++;  //���������� ���������� ��� �������� � ������ ���������
  if(microsteps_gl >= 65535)
  {
    microsteps_gl = 0;
  }
  TIM6->SR &= ~TIM_SR_UIF; 
}

void TIM2_PWM_Init()    //��� ��� ������ 1 (PA0..PA3)
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //�������� ������������ �������
  
  TIM2->PSC = 1;    //APBCLK=48 MHz
  TIM2->ARR = 65535; //������ ������������ ����������:f=48000000/4800=10 ���
  TIM2->CCR1=0;   //�������� ���������� ��� ������ 1
  TIM2->CCR2=0;   //�������� ���������� ��� ������ 2
  TIM2->CCR3=0;   //�������� ���������� ��� ������ 3
  TIM2->CCR4=0;   //�������� ���������� ��� ������ 3
  
  TIM2->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE;  //����� ������ ��� ��� 1 ������ (110-������ ���) + ��������� "������������" ��������
  TIM2->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE;  //����� ������ ��� ��� 2 ������ (110)        
  TIM2->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE;  //����� ������ ��� ��� 3 ������ (110) 
  TIM2->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4PE;  //����� ������ ��� ��� 3 ������ (110) 
  
  TIM2->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E |  TIM_CCER_CC4E;  //��������� ��� ������������ �������� �����
  TIM2->BDTR |= TIM_BDTR_MOE;                   //break and dead-time register - Main output enable
  TIM2->CR1 |= TIM_CR1_CMS_0 | TIM_CR1_CEN;     //Edge-aligned mode | Counter enable
  TIM2->EGR |= TIM_EGR_UG;                      //�������������
}


void TIM3_PWM_Init()    //��� ��� ������ 2 
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; //�������� ������������ �������
  
  TIM3->PSC = 1;    //APBCLK=1 MHz
  TIM3->ARR = 65535; //������ ������������ ����������:f=48000000/4800=10 ���
  TIM3->CCR1=0;   //�������� ���������� ��� ������ 1
  TIM3->CCR2=0;   //�������� ���������� ��� ������ 2
  TIM3->CCR3=0;   //�������� ���������� ��� ������ 3
  TIM3->CCR4=0;   //�������� ���������� ��� ������ 3
  
  TIM3->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE;  //����� ������ ��� ��� 1 ������ (110-������ ���) + ��������� "������������" ��������
  TIM3->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE;  //����� ������ ��� ��� 2 ������ (110)        
  TIM3->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE;  //����� ������ ��� ��� 3 ������ (110) 
  TIM3->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4PE;  //����� ������ ��� ��� 3 ������ (110) 
  
  TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E |  TIM_CCER_CC4E;  //��������� ��� ������������ �������� �����
  TIM3->BDTR |= TIM_BDTR_MOE;                   //break and dead-time register - Main output enable
  TIM3->CR1 |= TIM_CR1_CMS_0 | TIM_CR1_CEN;     //Edge-aligned mode | Counter enable
  TIM3->EGR |= TIM_EGR_UG;                      //�������������
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