#include <iostream>
#include <unistd.h>
//хранит переменные окружения
extern char ** environ;

int main(){
	int k=10;
	int pid=fork();
	std::cout <<"my pid = "<< getpid()
	 	  <<"\n parent pid = "<<getppid()
		  <<"\n new id = "<<pid<<std::endl;
	if(pid){
		k=3;
		std::cout << "\n(parent) k = "<<k;
		std::cout <<"\n(parent) uid = "<< getuid();
		std::cout.flush();//отправляем буфер стрима на печать
		sleep(5);
		std::cout <<"\n(parent) uid = "<< getuid();
		std::cout <<"\n(parent) gid = "<< getgid();
		std::cout <<"\n(parent) real uid = "<< geteuid();
		std::cout <<"\n(parent) real gid = "<< getegid()<<std::endl;;
		//заменяем программный код на другой и стартуем
		execl("/usr/bin/ps","-a",NULL);
		//std::system("ps -a"); //порождение отдельного процесса через интерпритатор
	}else{
		//печать переменных окружения
		for(int i=0;(*(environ+i))!=0;++i){
			std::cout<<i<<" "<<*(environ+i)<<std::endl;
		}
		std::cout << "(child) k = "<<k;
		std::cout <<"\n(child) uid = "<< getuid();
		std::cout <<"\n(child) gid = "<< getgid();
		std::cout <<"\n(child) real uid = "<< geteuid();
		std::cout <<"\n(child) real gid = "<< getegid();
		k=1;
		std::cout <<"\n(child) k="<<k<<"\nBay!";
	}
	return 0;
}
