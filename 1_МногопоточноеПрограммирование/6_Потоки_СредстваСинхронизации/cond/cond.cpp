#include <pthread.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>

/*объявляем условную переменную*/
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;

/*объявляем mutex*/
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

static const int LEN=7;

void * hndl_func(void * p_arg){
	if(*((int*)p_arg)!=LEN){
		/*будем ждать*/
		pthread_mutex_lock(&mutex);
		std::cout<<"Поток "<<*(int*)p_arg<<" встал на ожидание cond...\n";
		std::cout.flush();
		int res=pthread_cond_wait(&cond,&mutex);
		//int res=pthread_cond_timedwait(&cond,&mutec,&timespec); //ожидание с таймаутом
		std::cout<<"-->Поток "<<*(int*)p_arg<<" принял сигнал через cond.\n";
		pthread_mutex_unlock(&mutex);
	}else{
		sleep(1);
		std::cout<<"Поток "<<LEN<<" отправляет один сигнал.\n";
		int res=pthread_cond_signal(&cond);//отправляем сигнал последнему, кто ждет встал на ожидание этого cond
		std::cout<<res<<std::endl;
		sleep(1);
		//res=pthread_cond_signal(&cond);
		//std::cout<<res<<std::endl;
		std::cout<<"Поток "<<LEN<<" отправляет всем сигналы.\n";
		res=pthread_cond_broadcast(&cond);//отправляем сигналы всем, кто ждет этот cond
		std::cout<<res<<std::endl;
	}
	return p_arg;
}

int main(){
	/*можно инициализировать условную переменную так*/
	pthread_condattr_t cond_attr;
	pthread_condattr_init(&cond_attr);//инициализация аттрибутов
	int res=pthread_cond_init(&cond,&cond_attr);

	/*создаем потоки*/
	struct {
		pthread_t th;
		pthread_attr_t attr;
		int val;
	} asth[LEN];
	for(int i=0;i<LEN;++i){
		pthread_attr_init(&(asth[i].attr));//инициализация атрибутов,!!! не забывать!!!
		asth[i].val=i+1;
		sleep(1);
		std::cout<<"Запускаю процесс "<<i+1<<std::endl;
		res=pthread_create(&(asth[i].th),&(asth[i].attr),hndl_func,&(asth[i].val));
		assert(res==0);
	}

	/*присоединяем потоки*/
	void * p_ret;
	for(int i=0;i<LEN;++i){
		pthread_join(asth[i].th,&p_ret);
		std::cout<<"==Принято возвращаемое значение из потока "<<i+1<<std::endl;
		pthread_attr_destroy(&(asth[i].attr));

	}

	/*уничтожаем*/
	pthread_condattr_destroy(&cond_attr);
	pthread_cond_destroy(&cond);

	return 0;
}
