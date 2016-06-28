#include <signal.h>
#include <iostream>
#include <unistd.h>

int main(){
	int pid,signum;
	std::cout <<"Укажите номер процесса - ";
	std::cin >> pid;
	std::cout <<"Укажите номер сигнала - ";
	std::cin >> signum;

	//отправляем сигнал
	//kill(pid,signum);
	/* pid>0  - это отправка сигнала процессу
	 * pid<-1 - это отправка сигнала группе процессов
	 * pid=0  - это отправка текущей группе процессов
	 * pid=-1 - это отправка вообще всем
	 */

	//отправка себе синхронно 
	//raise(signum); //и сразу переходим к обработке сигнала
	//abort(); //отправляем себе сигнал SIGABRT (гарантия самоубийства)

	//отправка сигнала реального времени
	union sigval val;
	val.sival_int=getpid();
	sigqueue(pid,SIGRTMIN+signum,val);
	return 0;
}
