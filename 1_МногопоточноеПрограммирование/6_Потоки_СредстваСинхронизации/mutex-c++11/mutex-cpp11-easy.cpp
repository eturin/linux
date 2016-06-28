#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>
#include <unistd.h>

static const int LEN = 10;

int main() {
	/*объявляем mutex*/
	std::mutex mtx;
		
	int k = 0;

	/*экземпляры потоков в куче и через лямбда-функции*/
	std::thread ** mth=new std::thread*[LEN];
	for(int i = 0; i < LEN; ++i) {
		mth[i] = new std::thread([&k,&mtx](int num) {
							std::cout << "Поток " << num << " пытается заблокировать mutex....\n";
							mtx.lock(); //обеспечим последовательность работы с ресурсом
							//bool is_ok=mtx.try_lock();//не засыпающий режим
							std::cout << "Поток " << num << " заблокировал ресурс и меняет его.\n";
							++k; 
							mtx.unlock();
							std::cout << "Поток " << num << " отпустил ресурс.\n";
							 },i+1);
	}

	for(int i = 0; i < LEN; ++i) {
		mth[i]->join();
		delete mth[i];
	}
	delete[] mth;

	std::cout << k<<std::endl;
	
	/*mutex может работать с таймером*/
	std::timed_mutex t_mtx; 
	auto now=std::chrono::steady_clock::now();
	std::cout<<"Блокировка по времени +4сек...";
	if(t_mtx.try_lock_until(now + std::chrono::seconds(4))){ //до указанноно времени будет ОЖИДАТЬСЯ блокировка
		std::cout<<"Ok!";
		std::cout.flush();
		t_mtx.unlock();//снимаем блокировку, т.к. сама не снимется
	}else
		std::cout<<"неудача";
	std::cout<<" Блокировка еще на 5 сек...";
	if(t_mtx.try_lock_for(std::chrono::seconds(5))){   //на указанное время
		std::cout<<"Ok!\n";
		t_mtx.unlock();//снимаем блокировку, т.к. сама не снимется
	}else
		std::cout<<"неудача";
	
											
	return 0;
}
