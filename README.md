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

Em construção

### Pinagem

Em construção

### Fluxograma do Firmware

Em construção

### Interface do Usuário

A interface gráfica foi feita em Python utilizando o módulo *Qt4*. Conta com um painel que mostra, em tempo real, a temperatura e a umidade medidas pelo sensor.

![screenshot 1](https://github.com/Microcontroladores2018/Sampaio/blob/master/gui.png)

Para ativar a medição, deve-se escolher a porta na qual está ligada o microcontrolador por USB, e clicar no botão *Start*. O programa também permite que o usuário ligue um ar condicionado ou um vaporizador utilizando as caixas *Air Conditioner ON* e *Humidifier ON*.

### Referências

![Datasheet - DHT11](https://github.com/Microcontroladores2018/Sampaio/blob/master/DHT11.pdf)
![Reference Manual - STM32F4 Discovery](http://www.st.com/content/ccc/resource/technical/document/reference_manual/3d/6d/5a/66/b4/99/40/d4/DM00031020.pdf/files/DM00031020.pdf/jcr:content/translations/en.DM00031020.pdf)