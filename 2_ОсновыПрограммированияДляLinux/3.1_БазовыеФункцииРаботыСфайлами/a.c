#include <stdio.h>

#include <fcntl.h>
#include <unistd.h>

#include <errno.h>

int main(){
	const char * path="file";
	/*----------------------------*/
	int res=unlink(path);
	if(res==-1 && errno!=ENOENT){
		perror("Ошибка:\n");
		return 0;
	}

	/*----------------------------*/
	/*открываем файл, чтоб писать в него (если файла нет, то он создается с указанными правами mode & ~umask)*/
	umask(0000); //эффективные права
	int fd=open(path,O_WRONLY|O_CREAT,0607);
	if(fd==-1){
		perror("Ошибка:\n");
		return 1;
	}

	/*пишем в файл*/
	int m[]={0,1,2,3,4,5,6,7};
	int len=write(fd,m,sizeof(m));

	/*закрываем файл*/
	close(fd);

	/*----------------------------*/
	/*открываем файл, чтоб дописать в него*/
	fd=open(path,O_APPEND|O_WRONLY);
	if(fd==-1){
		perror("Ошибка:\n");
		return 1;
	}

	/*пишем в файл*/
	len=write(fd,m,sizeof(m));

	/*закрываем файл*/
	close(fd);

	/*----------------------------*/
	/*открываем файл, для чтения*/
	fd=open(path,O_RDONLY);
	if(fd==-1){
		perror("Ошибка:\n");
		return 1;
	}
	
	/*читаем из файла, указанное количество байт*/
	int n;
	while((len=read(fd,&n,sizeof(int)))==sizeof(int))
		printf("%d\n",n);
	
	/*закрываем файл*/
	close(fd);
	
	/*----------------------------*/
	/*открываем файл для чтения и записи одновременно*/
	fd=open(path,O_RDWR);
	if(fd==-1){
		perror("Ошибка:\n");
		return 1;
	}

	/*устанавливаем текущую позицию на указанный байт*/
	off_t pos=lseek(fd,3,SEEK_SET);
	unsigned char c=0xff;
	/*записываем 1 байт(текущая позиция сместится на 1 байт)*/
	len=write(fd,&c,1);
	/*читаем 1 байт*/
	len=read(fd,&c,1);
	printf("Прочитан с=%d\n",c);
	/*определим текущаю позицию*/
	pos=lseek(fd,0,SEEK_CUR);
	printf("Текущая позиция pos=%d\n",pos);
	/*перемещаемся в право на 3 байта*/
	pos=lseek(fd,3,SEEK_CUR);
	/*читаем 1 байт*/
	len=read(fd,&c,1);
	printf("Прочитан с=%d\n",c);
	/*перемещаемся в конец +1 байт*/
	pos=lseek(fd,1,SEEK_END);
	printf("Новая позиция pos=%d\n",pos);
	/*дописываем в файл (это расширит его размер)*/
	len=write(fd,&c,1);
	
	/*получаем новый файловый декриптор для того же ресурса*/
	int new_fd=fcntl(fd,F_DUPFD,7);//7 - желаемый новый номер дескриптора (можно не указывать)
	printf("new fd=%d\n",new_fd);

	/*закрываем файл*/
	close(fd);
	close(new_fd);

	/*----------------------------*/
	/*БЛОКИРОВКИ фрагментов файла*/
	fd=open(path,O_RDWR);
	if(fd==-1){
		perror("Ошибка:\n");
		return 1;
	}
	struct flock fl;
	fl.l_type=F_WRLCK;    //блокировка записи
	fl.l_whence=SEEK_SET; //блокируемый фрагмент задаётся явно
	fl.l_start=0;         //начиная с 0 позиции
	fl.l_len=5;           //следующие 5 байт
	fl.l_pid=getpid();    //идентификатор блокирующего процесса
	res=fcntl(fd,F_SETLKW,&fl); //F_SETLKW - будет ждать, а F_SETLK - не будет ждать

	/*снимаем установленную блокировку*/
	fl.l_type=F_UNLCK;
	res=fcntl(fd,F_SETLK,&fl);
	if(res==-1){
		perror("Ошибка:\n");
		return 1;
	}

	/*закрываем файл*/
	close(fd);

	/*----------------------------*/
	fd=open(path,O_RDONLY);
	if(fd==-1){
		perror("Ошибка:\n");
		return 1;
	}
	/*выясняем размер файла*/
	pos=lseek(fd,0,SEEK_END);
	printf("размер файла %d байт\n",pos);
	close(fd);


	return 0;
}

