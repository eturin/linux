#include <signal.h>
#include <unistd.h>
#include <iostream>

void h_sigint(int signum){
	std::cout << "Спасибо не надо.\n";
}

int main(){
	//готовим структуру с настройками
	struct sigaction old_acc,acc;
	//указываем обработчик
	acc.sa_handler=h_sigint; //SIG_IGN, SIG_DFL
	//временная маска
	//acc.sa_mask;
	//флаги
	//acc.sa_flags=SA_RESETHAND;//сбросить на дефолтный обработчик после первого вызова
	acc.sa_flags=SA_RESTART; //не прерывать работу обработчиков
	//acc.sa_flags=SA_SIGINFO;//обработка сигналов реалбного времени через acc.sigaction
	//назначаем обработку
	sigaction(SIGINT,&acc,&old_acc);

	while(true)
		pause();//останавливаем программу, пока её не пнет сигналом

	return 0;
}
