#include <signal.h>
#include <unistd.h>

int main(){
	//готовим новую маску обрабатываемых сигналов
	sigset_t mask;
	//разблокируем все сигналы в маске (все сигналы будут влиять на программу)
	sigemptyset(&mask);
	//заблокируем всек сигналы в маске (будут игнорироваться все)
	//sigfillset(&mask);
	//включаем игнор для сигнала Ctrl+C
	sigaddset(&mask,SIGINT);
	
	//установливаем сгенерированную маску нашему процессу
	sigset_t old_mask;
	sigprocmask(SIG_SETMASK,&mask,&old_mask);


	while(true)
		sleep(5);

	return 0;
}
