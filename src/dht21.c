
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
	while (contador != 0);
}

//Inicializa o DHT21
void DHT21_init() {
	RCC_AHB1PeriphClockCmd(DHT21_BUS, ENABLE); // Habilitando o clock.
	pinModeRead(); //Modo leitura
}

//FUNÇÃO DE LEITURA
uint8_t DHT21_read(uint8_t S, int16_t *t, uint16_t *h) {

	int16_t _t=0; // Variável temperatura
	uint16_t _h =0; // Variável umidade
	uint8_t data[5]; // Defino o dado a ser lido - Composto de 5 elementos de 8 bits cada - é o
	// protocolo de transmissão do  DHT21 - Manda UMIDADE HIGH(8bits) - UMIDADE LOW(8bits)
	//TEMPERATURA HIGH(8bits) - TEMPERATURA LOW(8bits) - PARIDADE(8bits)

	//OBS - primeiro bit da temperatura indica o sinal! - Usarei uma conversão.
	//Modo Simplificado de pegar os dados corretamente do meu vetor de informação (Aprendido no GITHUB)
	uint8_t err = read(data);
	if (err == DHT_OK) { // Se não tem erro - Posso coletar os dados!
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
	*t = _t; // Já tenho minha temperatura
	*h = _h; // Já tenho minha umidade
	return err;
}

static int16_t convertCtoF(int16_t c) {
	return c * 9 / 5 + 32; // Conversão(usado na temperatura)
}

static uint8_t read(uint8_t *data) {
	uint8_t laststate = Bit_SET; //Uso para verificar as "quebras" de bit
	uint8_t i = 0, j = 0; // índices de vetores

	//CLEAR DO "BANCO DE DADOS"
	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	//Modo escrita - Vou acordar o Sensor
	pinModeWrite();

	// BUS em nível baixo ~ 1ms
	GPIO_ResetBits(DHT21_GPIO, DHT21_PIN);
	Delay_us(1000);

	//BUS em nível alto por ~ 30us - "Release The Bus" do Protocolo
	GPIO_SetBits(DHT21_GPIO, DHT21_PIN);
	Delay_us(30);
	pinModeRead();

	// Espero o sensor responder
	for (i = 0; i < 10 && laststate == Bit_SET; i++) {
		laststate = GPIO_ReadInputDataBit(DHT21_GPIO, DHT21_PIN);
		Delay_us(1);
	}
	//Verifica se o sensor está conectado
	if (laststate == Bit_SET) {
		return DHT_NAO_CONECTADO;
	}

	// Espera o sinal em nível baixo ~85us
	Delay_us(75);
	for (i = 0; i < 10 && laststate == Bit_RESET; i++) {
		laststate = GPIO_ReadInputDataBit(DHT21_GPIO, DHT21_PIN);
		Delay_us(1);
	}
	// Verifica "TIME OUT"
	if (laststate == Bit_RESET) {
		return DHT_TIME_OUT;
	}

	// Espera o sinal em nível alto ~85us
	Delay_us(75);
	for (i = 0; i < 10 && laststate == Bit_SET; i++) {
		laststate = GPIO_ReadInputDataBit(DHT21_GPIO, DHT21_PIN);
		Delay_us(1);
	}

	// Verifica "TIME OUT"
	if (laststate == Bit_SET) {
		return DHT_TIME_OUT;
	}

	//Pronto, sei que meu sensor começará a transmitir os dados - Começa a leitura dos Dados
	//Protocolo usado pelo usuário kvv do GitHub.

	for (j = 0; j < 40; j++) {
		//Espera sinal em nível baixo ~65us
		Delay_us(48);
		for (i = 0; i < 17 && laststate == Bit_RESET; i++) {
			laststate = GPIO_ReadInputDataBit(DHT21_GPIO, DHT21_PIN);
			Delay_us(1);
		}
		// Verifica "TIME OUT"
		if (laststate == Bit_RESET) {
			return DHT_TIME_OUT;
		}

		//Espera Sinal em nível alto ~30 us
		Delay_us(22);
		for (i = 0; i < 8 && laststate == Bit_SET; i++) {
			laststate = GPIO_ReadInputDataBit(DHT21_GPIO, DHT21_PIN);
			Delay_us(1);
		}
		//PROCURA PRÓXIMO BIT
		data[j / 8] <<= 1;
		if (laststate == Bit_RESET) {
			continue;
		}

		// Espera o sinal baixo de ~50us
		Delay_us(38);
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


	//Se o vetor data não está completo ou a paridade não bate - erro de soma.
	return DHT_PARIDADE;
}

//Configura o Pino para Modo de Saída - ARM PARA SENSOR
static void pinModeWrite() {
	  GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Pin = DHT21_PIN; //Pino B0
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; // Modo saída (Vou mandar dados)
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //Open Drain - setar os dados
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;// Já tem pull up externo
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




