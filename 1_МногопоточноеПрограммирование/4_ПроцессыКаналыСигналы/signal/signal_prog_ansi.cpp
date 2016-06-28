#include <signal.h>
#include <unistd.h>
#include <iostream>

void f_sigint(int signum){
	std::cout<<"Спасибо, не надо.\n";
}

int main() {
        auto pf=signal(SIGINT,f_sigint);
	while(true)
		sleep(10);
	return 0;
}
