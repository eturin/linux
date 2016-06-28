#include <sys/stat.h>
#include <unistd.h>  
#include <assert.h>
#include <iostream>
#include <signal.h>
#include <wait.h>
#include <fstream>

void hndl_sigchld(int signum, siginfo_t * inf, void * content);

int main()  {    
	/*обработчик SIGCHLD*/
	struct sigaction acc,old_acc;
	acc.sa_sigaction=hndl_sigchld;
	acc.sa_flags=SA_SIGINFO;
	sigaction(SIGCHLD,&acc,&old_acc);

	/*Если файл с таким именем существует, удалим его*/   
       	const char * path="fifo";
	unlink(path);    
	
	/*Создаем FIFO*/    
	assert(0==mkfifo(path,0666));
	int pid;	
	if(pid=fork()){
		std::cout<<getpid();
		/*Открываем fifo для записи*/    
	        std::ofstream fout(path);
		char c;
		while(std::cin.get(c))
			(fout <<c).flush();
		fout.close();	

		kill(pid,SIGKILL);
		/*ждем потомка*/
	        pause();
	}else{
		/*Открываем fifo для чтения*/    
		std::ifstream fin(path);
		assert(fin.is_open());
		char c;
		while(fin.get(c)){
			std::cout<<c;
		}
		fin.close();
	}
       
	std::cout <<"Bay! "<<getpid();
	return 0;  
}


void hndl_sigchld(int signum, siginfo_t * inf, void * content){
	int res;
	waitpid(-1,&res,WNOHANG);
	std::cout <<"SIGCHLD: "<<res<<std::endl;
}
