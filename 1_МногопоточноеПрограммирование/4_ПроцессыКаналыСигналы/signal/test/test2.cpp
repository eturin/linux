#include <signal.h>
#include <fstream>
#include <unistd.h>
#include <iostream>
#include <wait.h>

void hndl_sigchld(int signum, siginfo_t * inf, void * content){
	int code;
	waitpid(-1,&code, WNOHANG /*не блокирующий режим*/);
	std::cout << code<<std::endl;
}

int main(){
	//назначем обработчик, который будет считывать код возврата
	struct sigaction acc,old_acc;
	acc.sa_sigaction=hndl_sigchld;
	sigaction(SIGCHLD,&acc,&old_acc);

	//создаем потомка и записываем сведения о нем
	int pid=getpid(),chld_pid=fork();
	if(chld_pid){
		std::ofstream fout("pid_parent");
		fout<<pid;
		fout.close();
		fout.open("pid_child");
		fout<<chld_pid;
		fout.close();
	}
	//ждем сигналов
	pause();

	return 0;
}

