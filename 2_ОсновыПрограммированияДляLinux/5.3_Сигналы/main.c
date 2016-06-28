#include <unistd.h>
#include <signal.h>
#include <stdio.h>

//SIGINT  - 2 (Ctrl-C)
//SIGABRT - 6 (abort(3))
//SIGTERM - 15 (просьба завершиться)
//SIGKILL - 9 ("убивают по 9")
//SIGCLD  - 17 (завершение потомка)

void f(int num){
	printf("Получен сингал %d\n",num);//обработчик просто печатает номер и фактически игнорирует сигнал
}

int main(){
	//назначаем обработчик сигнала
	signal(SIGINT,f);

	//отправка сигналов (см. kill -l)
	kill(getpid(),SIGINT); //самому себе здесь

	int i;
	for(i=0;i<5;++i)
		sleep(5);

	return 0;
}
