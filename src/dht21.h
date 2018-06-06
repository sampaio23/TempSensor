

#ifndef DHT21_H_
#define DHT21_H_

#include "stm32f4xx.h"

//"FeedBack" - Testar se a comunica��o est� ok! - M�todo encontrado em algum dos infinitos c�digos dispon�veis no GitHub

#define DHT_OK		0	// OK!
#define DHT_NAO_CONECTADO	1	// Sensor n�o conectado!
#define DHT_TIME_OUT	2	// "Time Out"
#define DHT_PARIDADE	3	// Erro na soma - Paridade!

//Auxiliar
#define DHT_C 0
#define DHT_F 1

//Irei utilizar o pino B0 para o protocolo SINGLE BUS do DHT21:
#define DHT21_PIN GPIO_Pin_0
#define DHT21_GPIO GPIOB
#define DHT21_BUS RCC_AHB1Periph_GPIOB

//Declaro a fun��o que inicializa o DHT21
void DHT21_init();

//Fun��o de Leitura
uint8_t DHT21_read(uint8_t s, int16_t *t, uint16_t *h, uint8_t *h1, uint8_t *t1);
volatile uint32_t contador;

//Declara��o de Fun��es do Temporizador
void SysTick_Handler();
void Delay_us(uint32_t mSecs);

#endif /* DHT21_H_ */
