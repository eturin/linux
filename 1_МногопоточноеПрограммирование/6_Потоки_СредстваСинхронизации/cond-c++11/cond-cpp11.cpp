#include <thread>
#include <iostream>
#include <condition_variable>
#include <unistd.h>

static const unsigned LEN=5;

int main(){
	/*объявляем cond c++11*/
	std::condition_variable cond;
	
	/*и mutex c++11*/
	std::mutex mtx;

	/*запускаем потоки*/
	std::thread * ath[LEN];
	for(unsigned i=0;i<LEN;++i)
		ath[i]=new std::thread([&mtx,&cond](int i){
					if(i!=LEN){
						//нужно завернуть mutex или timed_mutex (блокировка снимается, когда покидаем область видимости переменной ul)
						std::unique_lock<std::mutex> ul(mtx);
						std::cout<<"Поток "<<i<<" ожидает сигнал через cond\n";
						cond.wait(ul);
						std::cout<<"Поток "<<i<<" принял сигнал через cond\n";
						ul.unlock(); //можно снять блокировку непосредственно
					}else{
						sleep(1);
						/*отправляем пробуждение одному*/
						std::cout<<"Отправляем один сигнал пробуждения.\n";
						cond.notify_one();
						sleep(1);
						/*отправляем пробуждение всем*/
						std::cout<<"Отправляем сигнал пробуждения всем.\n";
						cond.notify_all();
					}
				},i+1);	


	/*присоединяем потоки назад*/
	for(unsigned i=0;i<LEN;++i){
		ath[i]->join();
		delete ath[i];
	}

	return 0;
}
