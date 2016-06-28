#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main(){
	/*1. порождаем потомка*/
	pid_t pid=fork();
	if(pid==-1)
		perror("Не удалось породить демона");
	else if(!pid){
		/*2. меняем текущий каталог (не обязательно, если файловая система не может быть отмонтирована)*/
		chdir("/");
		/*3. установить новый идентификатор сессии*/
		printf("pid=%lld\n",getpid());
		pid_t pid=setsid();
		printf("pid=%lld getpid()=%lld\n",pid,getpid());
		/*4. закрыть дескрипторы io*/
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		sleep(5);
	}

	return 0;
}
