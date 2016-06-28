#include <pthread.h>
#include <iostream>
#include <unistd.h>


/*объявляем и инициализируем контролера однократного запуска*/
pthread_once_t once=PTHREAD_ONCE_INIT;

void hndl_other_func(){
	std::cout<<"==>Выполнена ключевая функция\n";
}

void * hndl_func(void * p_arg){
	int num=*(int*)p_arg;
	int res=pthread_once(&once,hndl_other_func);//функция hndl_other_func должна выполниться не более одного раза
	std::cout<<"Поток "<<num<<" выполнил запуск ключевой функции.\n";

	return p_arg;
}

static const int LEN=5;

int main(){
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
	pthread_attr_destroy(&attr);

	return 0;
}


