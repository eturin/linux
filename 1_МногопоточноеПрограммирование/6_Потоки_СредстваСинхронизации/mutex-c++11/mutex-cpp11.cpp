#include <iostream>
#include <unistd.h>
#include <thread>
#include <mutex>

static int LEN=10;

/*это функтор (боловства ради)*/
class my_functor{
private:
	int num;
	std::mutex & mutex;
	/*запрещаем копирование и присваивание*/
	my_functor(const my_functor & other);
	my_functor & operator=(const my_functor & other);
public:
	/*конструктор экземпляра*/
	my_functor(std::mutex & mutex,int num):mutex(mutex),num(num){}
	/*оператор () - эта функция, которая будет выполняться в потоке*/
	void operator()(int & val){
		std::cout<<"Поток "<<num<<" пытается заблокировать mutex..."<<(num==LEN ? " и сделать каку\n":"\n");
		mutex.lock();
		
		if(num==LEN) 
			sleep(1);//маленькая пакость, для остальных потоков
		
		std::cout<<"Поток "<<num<<" заблокировал mutex и меняет ресурс\n";
	  	++val;
		mutex.unlock();	
		std::cout<<"Поток "<<num<<" освободил mutex\n";
	}
};

int main(){
	/*это ресурс, к которому мы хотим организовать последовательный доступ*/
	int val=0;

	/*объявляем mutex c++11*/
	std::mutex mtx;
	
	/*запускаем потоки*/
	my_functor ** mf=new my_functor*[LEN];
	std::thread ** mth=new std::thread*[LEN];
	for(int i=0;i<LEN;++i){
		mf[i]=new my_functor(mtx,i+1);//инициализация функтора
		mth[i]=new std::thread(std::ref(*(mf[i])),std::ref(val)); //запускаем оператор () функтора в потоке
	}

	for(int i=0;i<LEN;++i){
		mth[i]->join();
		delete mth[i],mf[i];
	}

	delete [] mth,mf;

	std::cout << "val = "<<val<<std::endl;

	return 0;
}
