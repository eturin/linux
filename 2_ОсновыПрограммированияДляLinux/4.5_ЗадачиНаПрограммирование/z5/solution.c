#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main(){
	/*1. порождаем потомка*/
	pid_t pid=fork();
	if(pid==-1)
		perror("Ошибка порождения демона");
	else if(!pid){
		/*2. меняем каталог*/
		chdir("/");
		/*3. получаем новый идентификатор сессии*/
		pid=setsid();
		printf("%lld\n",pid);
		/*4. закрываем стандартные дескрипторы*/	
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		
		/*демон просто ждет сигналов*/
		pause();
	}

	return 0;
}
