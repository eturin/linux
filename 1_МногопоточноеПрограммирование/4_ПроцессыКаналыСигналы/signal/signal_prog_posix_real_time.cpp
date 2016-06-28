#include <iostream>
#include <signal.h>
#include <unistd.h>

void hndl_real_time_signal(int signum, siginfo_t * inf, void * content){
	std::cout<<"\nномер сигнала - "<<inf->si_signo
		<<"\nerrno для сигнала - "<<inf->si_errno
		<<"\nкод сигнала (+ от ядра ОС, - от другого процесса) - "<<inf->si_code
	/* SI_USER  - пользователь послал kill, raise
	 * SI_QUEUE - сигнал пришел от sigqueue
	 * SI_TIMER - сигнал от таймера timer_settime
	 * SI_ASYNCIO - закончилась асинхронная операция ввода/вывода
	 * SI_MESGQ   - сообщение
	 */ 
	<<"\npid процесса - "<<inf->si_pid
	<<"\nпользователь - "<<inf->si_uid
	/* может прийти сигнал SIGFPE с кодами:
	 * 	FPE_INTDIV - деление на ноль целых чисел
	 * 	FPE_INTOVE - переполнение целого числа
	 * 	FPE_FLTDOV - деление на ноль действительных чисел
	 * 	и т.д.
	 */
	<<"\nзначение переданное с сигналом - (int)"<<inf->si_value.sival_int<<" (void*)"<<inf->si_value.sival_ptr<<std::endl;
	std::cout.flush();

}

int main(){
	//структура описывающая обработку нового сигнала
	struct sigaction acc, old_acc;
	acc.sa_flags=SA_SIGINFO;//сигнал реального времени
	acc.sa_sigaction=hndl_real_time_signal;
		


	//назначаем обработчик (стгналы реального времени от SIGRTMIN до SIGRTMAX), к примеру SIGRTMIN+1. Всего сигналов RTSIG_MAX. Чем меньше номер, тем выше приоритет сигнала при обработке.
	sigaction(SIGINT,&acc,&old_acc);

	while(true)
		sleep(5);
	return 0;
}











