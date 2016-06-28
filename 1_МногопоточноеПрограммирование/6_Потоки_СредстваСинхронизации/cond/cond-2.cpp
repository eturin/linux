#include <iostream>
#include <unistd.h>
#include <pthread.h>

static const int LEN = 8;
static int sleeping_count = 0;

struct sync {
	pthread_mutex_t *mutex;
	pthread_cond_t  *cond_var;
	int thread_no;
};

static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void * hndl_func(void * p_arg){
	struct sync * p = (struct sync*)p_arg;

	pthread_mutex_lock(p->mutex);
	std::cout<<"Поток "<<p->thread_no<<" готов \n";
	
	sleeping_count += 1;
	if (sleeping_count == LEN) {
	     /* Последний  worker должен разбудить "процесс будильщик", если тот уже спит. */
	     pthread_cond_signal(&cond);
	}
	
	int err = pthread_cond_wait(p->cond_var, p->mutex);
	if (err) 
		std::cerr<<"pthread_cond_wait: "<<err;

	std::cout<<"Поток "<<p->thread_no<<" завершился \n";
	pthread_mutex_unlock(p->mutex);
	
   	return NULL;
}
	     
 void * hndl_wake_up(void * p_arg){
	struct sync * p =(struct sync*)p_arg;
	pthread_mutex_lock(p->mutex);
	while (sleeping_count < LEN) //засыпаем, т.к. не все спят
		pthread_cond_wait(&cond, p->mutex);

	std::cout<<"Отправляем всем сигнал пробуждения\n";
	int err = pthread_cond_broadcast(p->cond_var);
	if (err) 
		std::cerr<<"pthread_cond_broadcast: "<<err;

	std::cout<<"Все потоки должны работать\n";
	pthread_mutex_unlock(p->mutex);
	
	return NULL;
}

int main(void){
	pthread_mutex_t mutex   = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;
	
	pthread_t ath[LEN];
	struct sync info[LEN];
	for (int i = 0; i < LEN; i++) {
		struct sync *arg = &info[i];
	        arg->mutex    = &mutex;
	        arg->cond_var = &cond_var;
	        arg->thread_no = i;
	        pthread_create(&ath[i],NULL,hndl_func,arg);
	}
	     
	sleep(10);
	pthread_t dispatcher;
        struct sync p = {&mutex, &cond_var, LEN};
        pthread_create(&dispatcher, nullptr, hndl_wake_up, &p);
        pthread_exit(NULL);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	pthread_cond_destroy(&cond_var);
        return 0;
}
