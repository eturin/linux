#include <signal.h>
#include <iostream>

int main(){
	//маска принимаемых сигналов
	sigset_t mask;
	sigaddset(&mask,SIGRTMIN+10);//Сигнал из списка сигналов реального времени
	sigaddset(&mask,SIGINT);     //Ctrl+C
	//ждем прихода сигнала
	int signum;
	sigwait(&mask,&signum);

	std::cout<<"получен сигнал - "<<signum;
	return 0;
}
