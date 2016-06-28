#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LEN 10

struct Buf{
	char data[LEN];
	int  len;
	struct Buf * next;
};

int main(){
	//создаем именованные каналы в ядре с местом в файловом пространстве имен
	printf("Создаю именованные каналы...");fflush(stdout);
	const char *pipe_in="in.fifo", *pipe_out="out.fifo";
	unlink(pipe_in);
	unlink(pipe_out);
	int res=mkfifo(pipe_in,0666);
	if(-1==res){
		perror("Ошибка создания канала in");
		return 1;
	}
	res=mkfifo(pipe_out,0666);
	if(-1==res){
		perror("Ошибка создания канала out");
		return 2;
	}
	printf("ok!\n");

	//получаем файловые дескрипторы каналов и переводим в неблокирующий режим (чтение и запись, чтоб не блокировалось здесь же сразу)
	printf("Получаю файловые дескрипторы...");
	int fd_in=open(pipe_in,O_RDWR|O_NONBLOCK);
	if(-1==fd_in){
		perror("Не получен дескриптор in");
		return 3;
	}
	int fd_out=open(pipe_out,O_RDWR|O_NONBLOCK);
	if(-1==fd_out){
		perror("Не получен дескриптор out");
		close(fd_in);
		return 4;
	}
	printf("ok!\n");

	//мультиплексирование
	printf("Мультиплексирование (select):\n");
	fd_set set_for_read, set_for_write;    //ровно по 1024 бит 
	printf("Цикл:\n");
	//узнаем наибольший номер дескриптора, чтоб узнать сколько битов в наборе нужно отслеживать
	int max=fd_in<fd_out ? fd_out : fd_in;
	//буфер на сервере (на случай задержки отправки)
	struct Buf *root=NULL, *tail=NULL;
	int is_out=0;
	//запускаем цикл проверки
	long long cnt=0;
	while(1){
		//инициализация
		FD_ZERO(&set_for_read); 
		FD_ZERO(&set_for_write);
		
		//добавляем в набор отслеживаемый на чтение дескриптор
		printf("\tдобавляем in...");
		FD_SET(fd_in,&set_for_read);
		printf("ok!\n");
		if(is_out){
			printf("\tдобавляем out...");
			FD_SET(fd_out,&set_for_write);
			printf("ok!\n");
		}
	        fflush(stdout);

		//запускаем проверку готовности
		++cnt;
		printf("\n%lld)-->select...",cnt);fflush(stdout);
		select(max+1,&set_for_read,&set_for_write,NULL,NULL);//эта команда сбрасывает не готовые биты поля, поэтому нужно регистрировать поновой	
		printf("ok!\n");
		//проснулись
		if(FD_ISSET(fd_out,&set_for_write)){ //проверка готовности писать
			printf("-->write...");
			if(root==NULL)
				FD_CLR(fd_out,&set_for_write);//снимаем с регистрации, т.к. нечего писать
			else{
				int len=write(fd_out,root->data,root->len);
				if(len!=-1){//успешно записано
					printf("ok!\n");
					struct Buf * tmp=root->next;
					free(root);
					root=tmp;
					if(root==NULL){ //все записано
						tail=NULL;
						is_out=0;
					}
				}
			}
		}
		if(FD_ISSET(fd_in,&set_for_read)){ //проверка готовности чтения
			printf("-->read...");
			//читаем из канала
			char data[LEN+1];
			int len=read(fd_in,data,LEN);
			if(-1!=len){
				if(root==NULL)
					tail=root=(struct Buf*)malloc(sizeof(struct Buf));
				else{
					tail->next=(struct Buf*)malloc(sizeof(struct Buf));
					tail=tail->next;
				}
				tail->next=NULL;
				tail->len=len;
				memcpy(tail->data,data,len);
				data[len]='\0';
				printf("ok! data=%s\n",data);

				//регистрируем дескриптор out
				is_out=1;
			}

		}
	}

	//закрываем дескрипторы
	close(fd_in);
	close(fd_out);

	//выносим каналы из фалового пространства
	unlink(pipe_in);
	unlink(pipe_out);
	
	return 0;
}
