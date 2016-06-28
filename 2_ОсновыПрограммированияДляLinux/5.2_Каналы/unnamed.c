#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>

int main(){
	//формируем канал в ядре ОС
	int fd[2];
	int res=pipe(fd);
	if(res==-1){
		perror("Ошибка создания канала");
		return 1;
	}

	//порождаем потомка
	pid_t pid=fork();
	if(pid==-1){
		perror("Ошибка порождения потомка");
		return 2;
	}else if(pid){
		//родитель
		close(fd[0]);//закрываем дескриптор in(откуда читать)
		for(int i=0;i<5;++i)
			write(fd[1],&i,sizeof(int));//пишем в канал

		//забираем код возврата потомка
		waitpid(pid,&res,0);
		printf("Потомок pid=%lld вернул %d\n",pid,WEXITSTATUS(res));		
	}else{
		//потомок
		close(fd[1]);//закрываем дескриптор out(куда писать) 
		for(int i=0,j;i<5;++i){
			ssize_t len=read(fd[0],&j,sizeof(int));
			if(len==-1){
				perror("Ошибка чтения из канала на стороне потомка");
				--i;
			}else
				printf("Прочитано из канала %d\n",j);
		}
	}


	return 0;
}
