#include <pthread.h>
#include <unistd.h>
#include <iostream>

static const int LEN=7;

/*объявляем барьер*/
pthread_barrier_t barrier;


void * hndl_func(void * p_arg){
	int num=*(int*)p_arg;
	std::cout<<"Поток "<<num<<" дошел до барьера..."<<std::endl;
	pthread_barrier_wait(&barrier);
	std::cout<<"...Поток "<<num<<" прошел барьер."<<std::endl;

	return p_arg;
}

int main(){
	/*инициализация барьера*/
	pthread_barrierattr_t battr;
	pthread_barrierattr_init(&battr);
	int cnt=3;//барьер откроется, если наберется указанное число потоков
	pthread_barrier_init(&barrier,&battr,cnt);

	/*создаем потоки в рамках процесса*/
	struct {
		pthread_t th;
		int val;
	} ath[LEN];
	pthread_attr_t attr;
	pthread_attr_init(&attr);//инициализация обязательна
	for(int i=0;i<LEN;++i){
		sleep(1);
		ath[i].val=i+1;
		pthread_create(&(ath[i].th),&attr,hndl_func,&(ath[i].val));
	}


	/*присоединяем потоки*/
	void * p_ret;
	for(int i=0;i<LEN;++i){
		pthread_join(ath[i].th,&p_ret);
		std::cout<<"==Присоединен поток "<<i+1<<std::endl;
	}

	/*уничтожаем*/
	pthread_barrierattr_destroy(&battr);
	pthread_barrier_destroy(&barrier);
	pthread_attr_destroy(&attr);

	return 0;
}
