
#include "dht21.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

static void pinModeWrite();
static void pinModeRead();
static int16_t convertCtoF(int16_t c);
static uint8_t read(uint8_t *data);

void SysTick_Handler() {
	if (contador != 0) { contador--; }
}

// Delay de 1us
void Delay_us(uint32_t mSecs) {
	contador = mSecs+1;
	while (contador != 0){
		__WFI();
	}
}

//Inicializa o DHT21
void DHT21_init() {
	RCC_AHB1PeriphClockCmd(DHT21_BUS, ENABLE); // Habilitando o clock.
	pinModeRead(); //Modo leitura
}

//FUN��O DE LEITURA
uint8_t DHT21_read(uint8_t S, uint8_t *h1, uint8_t *t1) {

//	int16_t _t=0; // Vari�vel temperatura
//	uint16_t _h =0; // Vari�vel umidade

	uint8_t data[5]; // Defino o dado a ser lido - Composto de 5 elementos de 8 bits cada - � o
	// protocolo de transmiss�o do  DHT21 - Manda UMIDADE HIGH(8bits) - UMIDADE LOW(8bits)
	//TEMPERATURA HIGH(8bits) - TEMPERATURA LOW(8bits) - PARIDADE(8bits)

	uint8_t err = read(data);
/*	if (err == DHT_OK) { // Se n�o tem erro - Posso coletar os dados!
		_h = data[0]; //
		_h <<= 8;
		_h |= data[1];
		_t = data[2] & 0x7F;
		_t <<= 8;
		_t |= data[3];
		if (data[2] & 0x80) {
			_t = 0 - _t;
		}
		if (S == DHT_F) {
			_t= convertCtoF(_t);
		}
	}
	*t = _t; // J� tenho minha temperatura
	*h = _h; // J� tenho minha umidade
*/
	// no sensor DHT11, esses sao os valores que correspondem a temperatura e humidade
	if (err == DHT_OK){
		*h1 = data[0];
		*t1 = data[2];
	}
	return err;
}

static int16_t convertCtoF(int16_t c) {
	return c * 9 / 5 + 32; // Convers�o(usado na temperatura)
}

static uint8_t read(uint8_t *data) {
	uint8_t laststate = Bit_SET; //Uso para verificar as "quebras" de bit
	uint8_t i = 0, j = 0; // �ndices de vetores

	//CLEAR DO "BANCO DE DADOS"
	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	//Modo escrita - Vou acordar o Sensor
	pinModeWrite();

	// BUS em n�vel baixo ~ 1ms
	GPIO_ResetBits(DHT21_GPIO, DHT21_PIN);
	Delay_us(18000);

	//BUS em n�vel alto por ~ 30us - "Release The Bus" do Protocolo
	GPIO_SetBits(DHT21_GPIO, DHT21_PIN);
	Delay_us(30);
	pinModeRead();

	// Espero o sensor responder
	for (i = 0; i < 10 && laststate == Bit_SET; i++) {
		laststate = GPIO_ReadInputDataBit(DHT21_GPIO, DHT21_PIN);
		Delay_us(1);
	}
	//Verifica se o sensor est� conectado
	if (laststate == Bit_SET) {
		return DHT_NAO_CONECTADO;
	}

	// Espera o sinal em n�vel baixo ~85us
	Delay_us(70);
	for (i = 0; i < 20 && laststate == Bit_RESET; i++) {
		laststate = GPIO_ReadInputDataBit(DHT21_GPIO, DHT21_PIN);
		Delay_us(1);
	}
	// Verifica "TIME OUT"
	if (laststate == Bit_RESET) {
		return DHT_TIME_OUT;
	}

	// Espera o sinal em n�vel alto ~85us
	Delay_us(70);
	for (i = 0; i < 20 && laststate == Bit_SET; i++) {
		laststate = GPIO_ReadInputDataBit(DHT21_GPIO, DHT21_PIN);
		Delay_us(1);
	}

	// Verifica "TIME OUT"
	if (laststate == Bit_SET) {
		return DHT_TIME_OUT;
	}

	//Pronto, sei que meu sensor come�ar� a transmitir os dados - Come�a a leitura dos Dados
	//Protocolo usado pelo usu�rio kvv do GitHub.

	for (j = 0; j < 40; j++) {
		//Espera sinal em n�vel baixo ~65us
		Delay_us(48);
		for (i = 0; i < 17 && laststate == Bit_RESET; i++) {
			laststate = GPIO_ReadInputDataBit(DHT21_GPIO, DHT21_PIN);
			Delay_us(1);
		}
		// Verifica "TIME OUT"
		if (laststate == Bit_RESET) {
			return DHT_TIME_OUT;
		}

		//Espera Sinal em n�vel alto ~30 us
		Delay_us(22);
		for (i = 0; i < 8 && laststate == Bit_SET; i++) {
			laststate = GPIO_ReadInputDataBit(DHT21_GPIO, DHT21_PIN);
			Delay_us(1);
		}
		//PROCURA PR�XIMO BIT
		data[j / 8] <<= 1;
		if (laststate == Bit_RESET) {
			continue;
		}

		// Espera o sinal baixo de ~50us
		Delay_us(35);
		for (i = 0; i < 12 && laststate == Bit_SET; i++) {
			laststate = GPIO_ReadInputDataBit(DHT21_GPIO, DHT21_PIN);
			Delay_us(1);
		}
		// Verifica "TIME OUT"
		if (laststate == Bit_SET) {
			return DHT_TIME_OUT;
		}
		data[j / 8] |= 1;
	}

	//Verifica a se recebeu todos os bits e se a Paridade bate
	if ((j == 40)
			&& (data[4]
					== ((data[0] + data[1] + data[2] + data[3])
							& 0xFF))) {
		return DHT_OK;
	}


	//Se o vetor data n�o est� completo ou a paridade n�o bate - erro de soma.
	return DHT_PARIDADE;
}

//Configura o Pino para Modo de Sa�da - ARM PARA SENSOR
static void pinModeWrite() {
	  GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Pin = DHT21_PIN; //Pino B0
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; // Modo sa�da (Vou mandar dados)
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //Open Drain - setar os dados
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;// J� tem pull up externo
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	  GPIO_Init(DHT21_GPIO, &GPIO_InitStructure);

}

//Configura Pino para Modo de ENTRADA - SENSOR PARA ARM
static void pinModeRead() {
	  GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Pin = DHT21_PIN; // Pino B0
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; // Modo entrada (Vou ler do sensor)
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(DHT21_GPIO, &GPIO_InitStructure);
}




