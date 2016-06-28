#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <assert.h>
#include <cstring>

void hndl_SIGCHLD(int signum, siginfo_t * inf, void * content){
	int res;
	waitpid(-1,&res,WNOHANG);
	abort();
}

const int LEN=512;

int main(){
	/*обработчик события*/
	struct sigaction acc,old_acc;
	acc.sa_sigaction=hndl_SIGCHLD;
	acc.sa_flags=SA_SIGINFO;
	sigaction(SIGCHLD,&acc,&old_acc);

	int fd[2];
	int res=socketpair(AF_UNIX,SOCK_STREAM,0,fd);
	assert(-1!=res);

	if(fork()){
		/*родитель*/
		close(fd[1]);
		bool is_repeat=true;
		char buf[LEN];
		do{
			std::memcpy(buf,"Hello!",LEN);						
			int size=send(fd[0],buf,std::strlen(buf),MSG_NOSIGNAL);	
			sleep(5);
			size=recv(fd[0],buf,LEN,MSG_NOSIGNAL);
			if(size==0){
				shutdown(fd[0],SHUT_RDWR);
				close(fd[0]);
				is_repeat=false;
			}else
				std::cout<<buf<<std::endl;

		}while(is_repeat);

	}else{
		/*потомок*/
		close(fd[0]);
		bool is_repeat=true;
		char buf[LEN];
		do{
			int size=recv(fd[1],buf,LEN,MSG_NOSIGNAL);
			if(size==0){
				shutdown(fd[1],SHUT_RDWR);
				close(fd[1]);
				is_repeat=false;
			}else
				send(fd[1],buf,size,MSG_NOSIGNAL);
		}while(is_repeat);
	}

	return 0;
}
