#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(){
	//создаем именованный канал в ядре с местом в файловом пространстве имен
	char path[]="c-d.fifo";
	unlink(path);
	int res=mkfifo(path,0666);
	if(res==-1){
		perror("Ошибка создания именованного канала в ядре");
		return 1;
	}
	
	//открываем канал как обычный файл (однако процесс ждет, пока не будет открыт на чтение канал)
	int fd=open(path,O_WRONLY);
	if(-1==fd){
		perror("Ошибка открытия канала");
		 return 2;
	}

	//пишем туда
	int i=0;
	for(i=0;i<5;++i)
		if(-1==write(fd,&i,sizeof(int)))
			perror("Ошибка записи в канал");
	

	//закрываем дескриптор
	close(fd);

	//удаляем канал из пространства файловой системы и ядра
	unlink(path);

	return 0;
}
