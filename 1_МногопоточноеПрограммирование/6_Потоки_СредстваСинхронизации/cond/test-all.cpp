#include <pthread.h>
#include <unistd.h>
#include <iostream>


/*объявление и инициализация cond*/
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
/*объявление и инициализация mutex для баоьера*/
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
/*объявление барьера*/
pthread_barrier_t barrier;


void * hndl_func(void * p_arg){
	int num=*(int*)p_arg;
	if(num==1){
		/*ждем сигнала*/
		pthread_mutex_lock(&mutex);
		std::cout<<"Поток "<<num<<" встал на ожидание cond...\n";
		pthread_cond_wait(&cond,&mutex);
		std::cout<<"==>Поток "<<num<<" получил сигнал от cond.\n";
		pthread_mutex_unlock(&mutex);
	}else{
		/*ждем на барьере, это не точка завершения и прервать не удастся*/
		std::cout<<"Поток "<<num<<" встал перед барьером...\n";
		pthread_barrier_wait(&barrier);
		std::cout<<"==>Поток "<<num<<" прошел барьер.\n";
	}

	return 0;
}

int main(){
	/*инициализация барьера*/
	pthread_barrierattr_t battr;
	pthread_barrierattr_init(&battr);
	pthread_barrier_init(&barrier,&battr,2);//барьер открывается для каждых двух потоков

	/*запуск потоков*/
	pthread_t th1,th2;
	pthread_attr_t attr;
	pthread_attr_init(&attr);//поумолчанию потоки присоединяемые, т.е. будут хранить возвращаемое значение до join
	int m[]={1,2,3};
	pthread_create(&th1,&attr,hndl_func,&m[0]);
	pthread_create(&th2,&attr,hndl_func,&m[1]);

	sleep(1);

	/*присоединение первого*/
	pthread_cond_signal(&cond);
	pthread_join(th1,nullptr);
	std::cout<<"==Поток 1 присоединен\n";

	/*завершение второго*/
	pthread_create(&th1,&attr,hndl_func,&m[2]);
	sleep(1);
	pthread_join(th1,nullptr);
	std::cout<<"==Поток 3 присоединен\n";
	pthread_join(th2,nullptr);
	std::cout<<"==Поток 2 присоединен\n";



	/*уничтожение*/
	pthread_attr_destroy(&attr);
	pthread_barrierattr_destroy(&battr);
	pthread_barrier_destroy(&barrier);
	pthread_cond_destroy(&cond);


	return 0;
}
