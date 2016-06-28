#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int cnt_user1=0;
int cnt_user2=0;

void f_usr(int num){
	if(num==SIGUSR1)
		++cnt_user1;
	else
		++cnt_user2;
}

void f_term(int num){
	printf("%d %d\n",cnt_user1,cnt_user2);
	fflush(stdout);
	_exit(0);
}

int main(){
	//регистрируем обработчики сигналов
	signal(SIGUSR1,f_usr);
	signal(SIGUSR2,f_usr);
	signal(SIGTERM,f_term);

	while(1)
		pause();//ждем сигналов
	
	return 0;
}

