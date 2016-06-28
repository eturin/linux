#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

int main(){
	char path[]="c-d.fifo";
	
	//открываем канал как обычный файл (однако процесс ждет, пока не будет открыт на запись канал)
	int fd=open(path,O_RDONLY);
	if(-1==fd){
		perror("Ошибка открытия канала");
		 return 2;
	}

	//читаем от туда
	int i=0,j;
	for(i=0;i<5;++i)
		if(-1==read(fd,&j,sizeof(int)))
			perror("Ошибка чтения из канала");
		else
			printf("Прочитано из именованного канала j=%d\n",j);
	

	//закрываем дескриптор
	close(fd);

	return 0;
}
