#include <thread>
#include <future>


int main(){
	/*можно запустить функцию в присоединяемом потоке совсем просто*/
	std::future<long> fut=std::async([](short k)->long { return k;}, 15);//запуск на выполнение лямбда-функции в отдельном потоке

	/*получение возвращаемого значения*/
	long l=fut.get();

	return 0;
}
