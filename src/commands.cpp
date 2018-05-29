#include "utils/commandline.h"

extern CommandLine cmdline;

extern int16_t temp;
extern uint16_t umi;


uint16_t cmd_info(uint16_t argc, uint8_t *argv8[]){
	const char **argv=(const char **)argv8;
	uint16_t size=0;
	char* buffer=(char*)argv[0];

//Printar Temperatura e Umidade com o comando info** no Terminal
	if(argc==1){
		size+=sprintf(buffer+size, "%d %d\r\n", temp ,umi);
	} else {
		size+=sprintf(buffer+size, "Syntax: info\r\n");
	}
	return size;
}

uint16_t cmd_ligarAr(uint16_t argc, uint8_t *argv8[]){
    if (argc==2){
        char* buffer = (char*) argv8[1];
        if (atoi(buffer) == 1) GPIO_SetBits(GPIOD, LED[1]);
        else GPIO_ResetBits(GPIOD, LED[1]);
    }
    return 0x0000;
}

uint16_t cmd_ligarUmid(uint16_t argc, uint8_t *argv8[]){
    if (argc==2){
        char* buffer = (char*) argv8[1];
        if (atoi(buffer) == 1) GPIO_SetBits(GPIOD, LED[2]);
        else GPIO_ResetBits(GPIOD, LED[2]);
    }
    return 0x0000;
}

//Comando info**
CommandLine cmdline({"info","ligarAr","ligarUmid"},
					{cmd_info,cmd_ligarAr,cmd_ligarUmid});
