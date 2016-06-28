#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>

//void hndl_sigchld(int,siginfo_t*,void*);

int main(){
	//обработчик кодов завершения
	/*struct sigaction acc, old_acc;
	acc.sa_sigaction=hndl_sigchld;
	acc.sa_flags=SA_RESTART;
	sigaction(SIGCHLD,&acc,&old_acc);*/
	

	//who | sort | uniq | wc -l

	//создаем канал
	int fd[2];
	pipe(fd);
	//порождаем процесс
	if(fork()){
		//для родителя
		printf("Start who\n");
		close(STDOUT_FILENO);             //замена выходного потока
		dup2(fd[1],STDOUT_FILENO);        
		close(fd[0]);close(fd[1]);        //закрываем лишнее. Обязательно!
		execl("/usr/bin/who","/usr/bin/who",NULL);//замена исполняемой программы
	}else{  //для потомка
		close(STDIN_FILENO);               //замена входного потока
		dup2(fd[0],STDIN_FILENO);	    
		close(fd[0]);close(fd[1]);         //закрываем лишнее. Обязательно!
		pipe(fd);
		if(fork()){
			//для родителя
			printf("Start sort\n");
			close(STDOUT_FILENO);              //замена выходного потока
			dup2(fd[1],STDOUT_FILENO);
			close(fd[0]);close(fd[1]);         //закрываем лишнее. Обязательно!
			execl("/usr/bin/sort","/usr/bin/sort",NULL);//замена исполняемой программы
		}else{  //для потомка
			close(STDIN_FILENO);               //замена входного потока
			dup2(fd[0],STDIN_FILENO);
			close(fd[0]);close(fd[1]);         //закрываем лишнее. Обязательно!
			pipe(fd);
			if(fork()){
				//для родителя
				printf("Start uniq\n");
				close(STDOUT_FILENO);              //замена выходного потока
				dup2(fd[1],STDOUT_FILENO);         
				close(fd[0]);close(fd[1]);         //закрываем лишнее. Обязательно!
				execl("/usr/bin/uniq","/usr/bin/uniq",NULL);//замена исполняемой программы
			}else{  //для потомка
				printf("Start wc -l");
				close(STDIN_FILENO);                  //замена входного потока 
				dup2(fd[0],STDIN_FILENO);
				close(fd[0]);close(fd[1]);            //закрываем лишнее. Обязательно!
				execlp("wc","wc","-l",NULL);//замена исполняемой программы
			}
		}
	}


	return 0;
}

/*void hndl_sigchld(int signum,siginfo_t * inf,void * content){
	int rez;
	waitpid(-1,&rez,WNOHANG);
}*/
