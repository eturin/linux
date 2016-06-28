#include <stdio.h>
#include <assert.h>
#include <string.h>

#define LEN 512

int main(int argc, char ** argv){
	//параметров должно быть не меньше 3
	assert(argc>=3);

	//строим команду
	char cmd[LEN];
	sprintf(cmd,"%s %s",argv[1],argv[2]);

	//захватываем вывод команды
 	FILE * stream=popen(cmd,"r");
	assert(stream!=NULL);

	//подсчет символа
	size_t cnt=0;
	char c;
	while((c=fgetc(stream))!=EOF)
		if(c=='0')
			++cnt;

	//закрываем 
	pclose(stream);	
	
	printf("%lld",cnt);

	return 0;
}
