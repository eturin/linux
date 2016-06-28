#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

const int LEN=512;

int main(){
	//открываем другую программу/команду
	FILE * stream=popen("ls -la","r");
	if(stream==NULL){
		perror("Не удалось открыть программу/команду");
		return 1;
	}

	//читаем вывод программы/команды
	char buf[LEN];	
	while(!feof(stream)){
		//инициализация буфера
		memset(buf,'\0',LEN);
		//читаем		
		fgets(buf,LEN-1,stream);
		//печатаем
		printf(buf);
	}

	//закрываем
	pclose(stream);

	return 0;
}
