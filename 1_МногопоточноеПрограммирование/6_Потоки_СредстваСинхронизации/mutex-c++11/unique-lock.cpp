#include <thread>
#include <mutex>
#include <iostream>
#include <unistd.h>

static const int LEN=7;

int main(){
	/*ресурс, к которому мы намериваемся обеспечить последовательный доступ*/
	int val=0;

	/*создаем mutex c++11*/
	std::mutex mtx;

	/*запускаем потоки*/
	std::thread ** m_th=new std::thread*[LEN];
	for(int i=0;i<LEN;++i){
		/*в потоке запускается лямбда-функция*/
		m_th[i]=new std::thread([&val](std::mutex & mtx,int num){
					std::cout<<"Поток "<<num<<" пытается установить блокировку...\n";
					/*блокируем mutex через unique_lock (блокировка снимется автоматически, когда мы покинем область видимости)*/
					std::unique_lock<std::mutex> uql(mtx);
					std::cout<<"Поток "<<num<<" установил блокировку\n";
					/*работаем с ресурсом*/
					++val;
					std::cout<<"Поток "<<num<<" покидвет область видимости переменной unl.\n";
				},std::ref(mtx),i+1);
	}

	/*цепляем все потоки назад*/
	for(int i=0;i<LEN;++i){
		m_th[i]->join();
		std::cout<<"Поток "<<i+1<<" присоединен.\n";
		delete m_th[i];
	}	
	delete [] m_th;

	return 0;
}
