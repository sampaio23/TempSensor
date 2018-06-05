# Monitor de Temperatura e Umidade

Implementação de um monitor de temperatura e umidade utilizando a placa STM32F4 Discovery com comunicação serial e Python.

### Conceito e Motivação

Controle de temperatura e umidade é algo fundamental em diversas aplicações, de manutenção de estufas ou processos industriais e laboratoriais, conservação de bens de consumo e controle automotivo até aplicações em residências.
É comum a utilização de ar condicionado ou vaporizadores para ambientes pequenos e de refrigerantes em grandes canos para indústrias, porém pode ser de interesse do gerente de querer um controle maior, juntamente com um sistema de monitoramento dos dados.

Este projeto consiste da implementação inicial de um protótipo funcional, utilizando um microcontrolador para monitoramento e controle de temperatura e umidade. O usuário será capaz de executar uma série de funções:
* Monitoramento em tempo real de temperatura e umidade do local via comunicação serial.
* Controle com funções *liga* e *desliga* de um ar condicionado e de um vaporizador.
* Utilização de interface gráfica amigável em uma aplicação para Desktop em Python.

### Diagrama de Blocos

Será utilizado o microcontrolador STM32F407VGT6 para a implementação do projeto, bem como um sensor de temperatura e umidade DHT11. Como ilustrado, para a comunicação da placa com o computador, será utilizada comunicação serial via USB. A comunicação com o sensor é feita por um só fio (single-wire bi-directional) e cada envio consiste de 40 bits que indicam temperatura, umidade, e 1 byte de paridade.

![screenshot 2](https://github.com/Microcontroladores2018/Sampaio/blob/master/images/blockdiagram.png)

### Pinagem e Periféricos

Foram utilizados para o projeto *GPIOs* e *USB*. Abaixo, pode-se ver a tabela de pinagem e os comandos USB.

| **Pino Discovery** |     **Descrição**     |
|:------------------:|:---------------------:|
|        PA11        |         USB DP        |
|        PA12        |         USB DM        |
|         PB0        |  Data In from sensor  |
|        PD12        |       Error LED       |
|        PD13        | Relay Air Conditioner |
|        PD14        |    Relay Humidifier   |

```cpp
// Command: info
// Get sensor values
uint16_t cmd_info(uint16_t argc, uint8_t *argv8[]){
	const char **argv=(const char **)argv8;
	uint16_t size=0;
	char* buffer=(char*)argv[0];
	
	if(argc==1) size+=sprintf(buffer+size, "%d %d\r\n", temp ,umi);
    else size+=sprintf(buffer+size, "Syntax: info\r\n");

	return size;
}

// Command: ligarAr num
// If num equals 1, turns air conditioner on; else, turns off 
uint16_t cmd_ligarAr(uint16_t argc, uint8_t *argv8[]){
    if (argc==2){
        char* buffer = (char*) argv8[1];
        if (atoi(buffer) == 1) GPIO_SetBits(GPIOD, LED[1]);
        else GPIO_ResetBits(GPIOD, LED[1]);
    }
    return 0x0000;
}

// Command: ligarUmid num
// If num equals 1, turns humidifier on; else, turns off
uint16_t cmd_ligarUmid(uint16_t argc, uint8_t *argv8[]){
    if (argc==2){
        char* buffer = (char*) argv8[1];
        if (atoi(buffer) == 1) GPIO_SetBits(GPIOD, LED[2]);
        else GPIO_ResetBits(GPIOD, LED[2]);
    }
    return 0x0000;
}
```

### Fluxograma do Firmware

O fluxograma consiste em um *loop* que continuamente lê o sensor. Ele contém tratamento de erros, e mostra, piscando o LED4, qual o tipo de erro em tempo real. Se não houver erros, o LED4 não pisca.

![screenshot 3](https://github.com/Microcontroladores2018/Sampaio/blob/master/images/flow.png)

O envio de comandos por USB pode ser representado pelo fluxograma abaixo.

![screenshot 4](https://github.com/Microcontroladores2018/Sampaio/blob/master/images/usb.png)

### Interface do Usuário

A interface gráfica foi feita em Python utilizando o módulo *Qt4*. Conta com um painel que mostra, em tempo real, a temperatura e a umidade medidas pelo sensor.

![screenshot 1](https://github.com/Microcontroladores2018/Sampaio/blob/master/images/gui.png)

Para ativar a medição, deve-se escolher a porta na qual está ligada o microcontrolador por USB, e clicar no botão *Start*. O programa também permite que o usuário ligue um ar condicionado ou um vaporizador utilizando as caixas *Air Conditioner ON* e *Humidifier ON*.

### Demonstração

Em construção

### Referências

[Datasheet - DHT11](https://akizukidenshi.com/download/ds/aosong/DHT11.pdf)

[Reference Manual - STM32F4 Discovery](http://www.st.com/content/ccc/resource/technical/document/reference_manual/3d/6d/5a/66/b4/99/40/d4/DM00031020.pdf/files/DM00031020.pdf/jcr:content/translations/en.DM00031020.pdf)