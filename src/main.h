#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#ifndef MAIN_H_
#define MAIN_H_

//Defino LEDS que serão utilizados//
const uint16_t LEDS = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
const uint16_t LED[] = {GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};

//Função de inicialização//
void init();

//Função de Loop//
void loop();

#endif /* MAIN_H_ */
