#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

int main(){
	/*порождаем потомка*/
	pid_t pid=fork();
	if(pid==-1){
		perror("Ошибка порождения потомка");		
		return 1;
	}else if(pid){
		/*родитель должен забрать код возврата потомка*/
		int res=0;
		waitpid(pid,&res,0);//код возврата закодирован
		printf("Потомок (pid=%lld) звершился с кодом %d\n",pid,WEXITSTATUS(res));
	}else{
		/*потомок просто немного ждет*/
		printf("Потомок:\n\tpid = %lld\n\tppid = %lld\n",getpid(),getppid());
		sleep(5);
		exit(1); 
	}

	return 0;
}
