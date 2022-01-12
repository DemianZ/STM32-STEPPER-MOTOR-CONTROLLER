#ifndef __gpio_H
#define __gpio_H


#define TWOLEDSMASK 0x0300  //PB8+PB9 Mask
#define PB9LEDMASK 0x0200  //PB9 Mask
#define PB8LEDMASK 0x0100  //PB8 Mask


int GPIO_LED_Init();
int GPIO_CH1_Fullstep_Init();
int GPIO_CH2_Fullstep_Init();
void GPIO_CH1_Driver_Init(uint8_t microstep);
void GPIO_CH2_Driver_Init(uint8_t microstep);

#endif