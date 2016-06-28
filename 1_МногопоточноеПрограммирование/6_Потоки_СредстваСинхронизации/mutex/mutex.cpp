#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <iostream>

/*это будет общим ресурсом*/
volatile static int val;
/*объявляем мьютекс (mutex) и сразу его инициализмруем*/
static pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;

void * hndl_th1(void * p_arg){
	std::cout<<"Занимаем мьютекс в 1\n";
	pthread_mutex_lock(&mtx);  //ставим блокировку или ждем (процесс засыпает и это "дорого"!!! если ожидания короткие, то надо бесконечно проверять, "грея камень")
	//int res=pthread_mutex_trylock(&mtx);//этим можно попробовать установить блокировку, но не ждать, если занято
	sleep(5);
	val+=1;			   //работаем с ресурсом
	std::cout<<"Отпускаем мьютекс в 1.\n";
	pthread_mutex_unlock(&mtx);//снимаем блокировку
	return p_arg;
}

void * hndl_th2(void * p_arg){
	sleep(1);
	std::cout<<"Пытаемся блокировать мьютекс во 2...\n";
	pthread_mutex_lock(&mtx);   //ставим блокировку или ждем 
	//int res=pthread_mutex_trylock(&mtx);//этим можно попробовать установить блокировку, но не ждать, если занято
	std::cout<<"Получилось во 2.\n";
	val+=2;
	pthread_mutex_unlock(&mtx); //снимаем блокировку
	return p_arg;
}

int main(){
	/*можно инициализировать мьютекс и так*/
	//pthread_mutexattr_t mtx_attr;
	//int r=pthread_mutex_init(&mtx,&mtx_attr);

	/*создаем потоки в рамках процесса*/
	pthread_t th1,th2;
	pthread_attr_t attr;
	int res=pthread_attr_init(&attr);  //инициализация
	res=pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED); //оба потока будут сразу не присоединяемые
	res=pthread_create(&th1,&attr,hndl_th1,nullptr);
	assert(0==res);
	res=pthread_create(&th2,&attr,hndl_th2,nullptr);
	assert(0==res);
	
	/*ждем пока потоки делят свой мьютекс*/
	sleep(6);
	std::cout<<"val = "<<val<<std::endl;

	pthread_attr_destroy(&attr);//освобождение
	pthread_mutex_destroy(&mtx);//уничтожение мьютекса
	return 0;
}
