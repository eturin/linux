#include <pthread.h>
#include <iostream>

/*общий ресурс, к которому организуется последовательный доступ*/
volatile static int val;
/*создаем rwlock*/
pthread_rwlock_t rw_lock=PTHREAD_RWLOCK_INITIALIZER; //объявление и немедленная инициализация

void * hndl_f1(void * p_arg){
	/*предотвращение изменения ресурса другими, пока читаем */
	int res=pthread_rwlock_rdlock(&rw_lock);
	//int res=pthread_rwlock_tryrdlock(&rw_lock);//это не блокирующий режим (при успехе res!=0)

	/*читаем ресурс*/
	std::cout<<"чтение из 1 потока: val = "<<val<<std::endl;

	/*снимаем блокировку*/
	res=pthread_rwlock_unlock(&rw_lock);

	return p_arg;
}

void * hndl_f2(void * p_arg){
	/*ставим эксклюзивную блокировку (читать и писать можно только нам)*/
	int res=pthread_rwlock_wrlock(&rw_lock);
	//int res=pthread_rwlock_trywrlock(&rw_lock);//не блокирующий режим
	
	/*читаем и изменяем ресурс*/
	val=2;

	/*снимаем блокировку*/
	res=pthread_rwlock_unlock(&rw_lock);
	
	return p_arg;
}


int main(){
	/*можно инициализировать rwlock и так*/
	//pthread_rwlockattr_t rwattr;
	//pthread_rwlock_init(&rw_lock,&rwattr);

	/*создаем потоки в рамках процесса*/
	pthread_t th1,th2;
	pthread_attr_t attr;
	pthread_attr_init(&attr);//инициализируем структуру аттрибутов
	pthread_create(&th1,&attr,hndl_f1,nullptr);
	pthread_create(&th2,&attr,hndl_f2,nullptr);
	/*забираем у процессов возвращаемое значение, иначе повиснут до конца работы основного потока*/
	void * p_ret;
	pthread_join(th1,&p_ret);
	pthread_join(th2,&p_ret);

	pthread_attr_destroy(&attr);//освобождаем структуры аттрибутов
	pthread_rwlock_destroy(&rw_lock);//уничтожение блокировки

	return 0;
}
