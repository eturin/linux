#include <signal.h>
#include <iostream>

void hndl_sig_rtm_15(int signum, siginfo_t * inf,void * content){
	std::cout << "Сигнал получен.\n";
	std::cout.flush();
}

int main(){
	//назначаем обработчик сигналу
	struct sigaction acc,old_acc;
	acc.sa_flags=SA_RESTART;
	acc.sa_sigaction=hndl_sig_rtm_15;
	sigaction(SIGRTMIN+15,&acc,&old_acc);

	//делаем маску принимаемых сигналов
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask,SIGRTMIN+16);

	//ждем сигнала
	siginfo_t inf;
	sigwaitinfo(&mask,&inf);
	

 	std::cout<<"\nномер сигнала - "<<inf.si_signo
	         <<"\nerrno для сигнала - "<<inf.si_errno
		 <<"\nкод сигнала (+ от ядра ОС, - от другого процесса) - "<<inf.si_code
        /* SI_USER  - пользователь послал kill, raise
	 * SI_QUEUE - сигнал пришел от sigqueue
	 * SI_TIMER - сигнал от таймера timer_settime
	 * SI_ASYNCIO - закончилась асинхронная операция ввода/вывода
	 * SI_MESGQ   - сообщение
	 */
	        <<"\npid процесса - "<<inf.si_pid
	        <<"\nпользователь - "<<inf.si_uid
	 /* может прийти сигнал SIGFPE с кодами:
	  *      FPE_INTDIV - деление на ноль целых чисел
	  *      FPE_INTOVE - переполнение целого числа
	  *      FPE_FLTDOV - деление на ноль действительных чисел
	  *      и т.д.
	  */
	        <<"\nзначение переданное с сигналом - (int)"<<inf.si_value.sival_int<<" (void*)"<<inf.si_value.sival_ptr<<std::endl;
        std::cout.flush();	

	return 0;
}
