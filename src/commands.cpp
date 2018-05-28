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

//Comando info**
CommandLine cmdline({"info"},
					{cmd_info});
