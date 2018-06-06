

/* Includes */
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "utils/commandline.h"
#include <usb/usb_stm32.h>
#include <usb/usb_device_class_audio.h>
#include <usb/usb_device_class_cdc_vcp.h>
#include <usb/usb_device_class_cdc_rndis.h>
#include <utils/io_pin_stm32.h>
#include <utils/serialnumber.h>
#include <utils/interrupt_stm32.h>
#include "main.h"
#include "dht21.h"
#include "dht21.c"

extern "C"{
#include "usb_dcd_int.h"
#include "usb_hcd_int.h"
}


//temp : Temperatura   umi : Umidade , vari�veis globais para o USB conseguir ler.
int16_t temp = 0;
uint16_t umi = 0;

uint8_t t1 = 0;
uint8_t h1 = 0;
/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */

/**
 **===========================================================================
 **
 **  Abstract: main program
 **
 **===========================================================================
 */

//USB - CONFIG DA AULA 4.2 - RETIRANDO OS ERROS PELO PROCESSO DO CAP RENAULT//
extern CommandLine cmdline;

IO_Pin_STM32 USB_DP(IO_Pin::IO_Pin_Mode_SPECIAL, GPIOA, GPIO_Pin_11, GPIO_PuPd_NOPULL, GPIO_OType_PP, GPIO_AF_OTG_FS);
IO_Pin_STM32 USB_DM(IO_Pin::IO_Pin_Mode_SPECIAL, GPIOA, GPIO_Pin_12, GPIO_PuPd_NOPULL, GPIO_OType_PP, GPIO_AF_OTG_FS);

uint8_t USB_DEVICE_CLASS::_numinterfaces=0;
uint8_t USB_DEVICE_CLASS::_numinendpoints=1;
uint8_t USB_DEVICE_CLASS::_numoutendpoints=1;
uint8_t USB_DEVICE_CLASS::_numdescriptorstrings=6;
std::list<USB_DEVICE_CLASS*> USB_DEVICE_CLASS::_usb_device_classes_list;

USB_DEVICE_CLASS_CDC_RNDIS usb_device_class_cdc_rndis(0);
USB_DEVICE_CLASS_CDC_VCP usb_device_class_cdc_vcp(1);
USB_DEVICE_CLASS_AUDIO usb_device_class_audio(0);


USB_STM32 usb(0x29BC, 0x0002, "IME", "Microcontroladores 2018", SerialNumberGetHexaString());

INTERRUPT_STM32 usb_otg_fs_interrupt(OTG_FS_IRQn, 0x0D, 0x0D, ENABLE);

CircularBuffer<uint8_t> buffer(0,1024);


int main(void)
{
	//Configura��o do Systick para tick de 1us
	SysTick_Config(168-1);

	//Inicializa GPIO D(LED) e B(BUS)
    init();
    DHT21_init();

	usb.Init();


	/* Infinite loop */
	while (1)
	{
		loop();
	}
}


void init() {
	//Habilita Clock e Configura GPIO
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio); // J� inicia algumas configura��es , s� mudo o que eu preciso.
    gpio.GPIO_Mode = GPIO_Mode_OUT; // Modo Sa�da
    gpio.GPIO_Pin = LEDS; // Pinos dos Leds de Alerta
    GPIO_Init(GPIOD, &gpio);
}

void loop() {
	//Delay de 2s entre cada Loop - ( Tempo para o DHT21 sair do modo SLEEP e entrar no POWER) //
	Delay_us(2000000);

	//Reseto o Status do LED A cada ciclo
	GPIO_ResetBits(GPIOD, LEDS);



	//Leitura - Erro  - TEMPERATURA - UMIDADE
	uint8_t erro = DHT21_read(DHT_C, &temp, &umi, &h1, &t1), i = 0, j = 0;

	//O sensor me fornece tempx10 e umix10
	temp = temp / 10;
	umi = umi / 10;

	// Verifica se est� tudo certo , caso contr�rio , atualiza j com o �ndice referente ao erro.
	if (erro != DHT_OK) {
		if (erro == DHT_NAO_CONECTADO) {
			j = 2;
		} else if (erro == DHT_TIME_OUT) {
			j = 3;
		} else if (erro == DHT_PARIDADE) {
			j = 4;
		}
		//Pisca o led dependendo do �ndice de erro (j)
		for (i = 0; i < j; i++) {
			GPIO_SetBits(GPIOD, LED[0]);
			Delay_us(250000);
			GPIO_ResetBits(GPIOD, LED[0]);
			Delay_us(200000);
		}
	}
    // Usarei os LEDs para indicar os rel�s
	/* else {
		//Muda os status dos LEDS dependendo do range de temperatura.
		if ((temp > 5) && (temp<=20)) {
			GPIO_SetBits(GPIOD, LED[2]);
		}
		if ((temp > 20)&&(temp<=30)) {
			GPIO_SetBits(GPIOD, LED[1]);
		}
		if (temp > 30) {
			GPIO_SetBits(GPIOD, LED[3]);
		}
	} */
}



//INTERRUP��O DO USB
extern USB_OTG_CORE_HANDLE USB_OTG_dev;
extern "C" void OTG_FS_IRQHandler(void){
	USBD_OTG_ISR_Handler (&USB_OTG_dev);
	USBH_OTG_ISR_Handler (&USB_OTG_dev);


	uint16_t size=usb_device_class_cdc_vcp.GetData(buffer,256);
	if(size) {
		cmdline.In(buffer);
	}

	uint8_t buffer[32];
	do{
		size=cmdline.Out(buffer, 32);
		if(size){
			usb_device_class_cdc_vcp.SendData(buffer, size);
		}
	} while(size==32);
}

extern "C" void OTG_FS_WKUP_IRQHandler(void){
	if(USB_OTG_dev.cfg.low_power){
		*(uint32_t *)(0xE000ED10) &= 0xFFFFFFF9 ;
		SystemInit();
		USB_OTG_UngateClock(&USB_OTG_dev);
	}
	EXTI_ClearITPendingBit(EXTI_Line18);
}

/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
extern "C" void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
	/* TODO, implement your code here */
	return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
extern "C" uint16_t EVAL_AUDIO_GetSampleCallBack(void){
	/* TODO, implement your code here */
	return -1;
}
