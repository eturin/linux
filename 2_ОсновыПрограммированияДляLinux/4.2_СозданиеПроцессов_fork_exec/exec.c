#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main(){
	/*порождаем потомка*/
	pid_t pid=fork();

	if(pid==-1){
		perror("Ошибка порождения потомка");
		return 1;
	}else if(pid){
		/*родитель ждет завершения потомка*/
		int res=0;
		waitpid(pid,&res,0);
		printf("Замещаемый потомок (pid=%lld) вернул %d\n",pid,WEXITSTATUS(res));//код возврата изначально закодирован
	}else{
		/*потомок замещает свой образ на другую программу*/
		if(execl("a",NULL)){
			perror("Ошибка замещения образа в потомке");
			exit(2);
		}
	}

	return 0;
}

