#include <signal.h>
#include <unistd.h>
#include <fstream>
int main(){
	//пишем свой пид в файл
	std::ofstream fout("pid");
	fout <<getpid();
	fout.close();
	//игнорируем  сигналы
	signal(SIGINT,SIG_IGN);
	signal(SIGTERM,SIG_IGN);
	//ждем любые сигналы, кроме игнорируемых
	pause();
	return 0;
}
