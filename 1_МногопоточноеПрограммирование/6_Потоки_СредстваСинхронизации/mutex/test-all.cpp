#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <assert.h>
#include <exception>
#include <fstream>

/*ресурс для mutex и сам mutex*/
volatile static int mutex_val;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

/*ресурс для spin и сам spin*/
volatile static int spin_val;
pthread_spinlock_t spin;

/*ресурс для rwlock и сам rwlock*/
volatile static int rwlock_val;
pthread_rwlock_t rwlock=PTHREAD_RWLOCK_INITIALIZER;

/*функция в потоке блокирует mutex и спит*/
void * hndl_mutex_lock(void * p_arg){
	std::cout<<"Процесс 1 блокирует mutex."<<std::endl;
	pthread_mutex_lock(&mutex);
	std::cout<<"Процесс 1 засыпает..."<<std::endl;
	sleep(5);
	std::cout<<"Процесс 1 отпускает mutex."<<std::endl;
	pthread_mutex_unlock(&mutex);

	return p_arg;
}
/*функия в потоке профукала момент и пытается занять mutex*/
void * hndl_mutex_lock_2(void * p_arg){
	sleep(1);
	std::cout<<"Процесс 2 пытается заблокировать mutex..."<<std::endl;
	pthread_mutex_lock(&mutex);
	std::cout<<"Процесс 2 заблокировал mutex."<<std::endl;
	mutex_val=2;
	pthread_mutex_unlock(&mutex);

	return p_arg;
}


/*функция в потоке блокирует spin и спит (для spin - эхто расстрел! надо максимально, короткое время блокировки, т.к. ожидающие процессы "греют камень")*/
void * hndl_spin_lock(void * p_arg){
	std::cout<<"Процесс 3 блокирует spin."<<std::endl;
	pthread_spin_lock(&spin);
	std::cout<<"Процесс 3 засыпает..."<<std::endl;
	sleep(5);
	std::cout<<"Процесс 3 отпускает spin."<<std::endl;
	pthread_spin_unlock(&spin);

	return p_arg;
}
/*функия в потоке профукала момент и пытается занять spin*/
void * hndl_spin_lock_2(void * p_arg){
	sleep(1);
	std::cout<<"Процесс 4 пытается заблокировать spin..."<<std::endl;
	pthread_spin_lock(&spin);
	std::cout<<"Процесс 4 заблокировал spin."<<std::endl;
	spin_val=4;
	pthread_spin_unlock(&spin);

	return p_arg;
}

/*функция в потоке блокирует mutex и спит*/
void * hndl_rwlock_lock_1(void * p_arg){
	std::cout<<"Процесс 5 блокирует rwlock эксклюзивно."<<std::endl;
	pthread_rwlock_wrlock(&rwlock);
	std::cout<<"Процесс 5 засыпает..."<<std::endl;
	sleep(5);
	rwlock_val=5;
	std::cout<<"Процесс 5 отпускает rwlock."<<std::endl;
	pthread_rwlock_unlock(&rwlock);

	return p_arg;
}
/*функия в потоке профукала момент и пытается занять rwlock эксклюзивно*/
void * hndl_rwlock_lock_2(void * p_arg){
	sleep(1);
	std::cout<<"Процесс 6 пытается заблокировать rwlock эксклюзивно..."<<std::endl;
	pthread_rwlock_wrlock(&rwlock);
	std::cout<<"Процесс 6 заблокировал rwlock."<<std::endl;
	rwlock_val=6;
	sleep(1);
	pthread_rwlock_unlock(&rwlock);

	return p_arg;
}
/*функия в потоке профукала момент и пытается занять rwlock на чтение*/
void * hndl_rwlock_lock_3(void * p_arg){
	sleep(1);
	std::cout<<"Процесс 7 пытается заблокировать rwlock на чтение..."<<std::endl;
	pthread_rwlock_rdlock(&rwlock);
	std::cout<<"Процесс 7 заблокировал rwlock."<<std::endl;
	std::cout<<"rwlock_val = "<<rwlock_val<<std::endl;
	pthread_rwlock_unlock(&rwlock);

	return p_arg;
}

void hndl_uncatch(){
	std::cout <<"Есть не перехваченное исключение.\n";
	std::cout.flush();
}

int main(){
	std::set_terminate(hndl_uncatch);
	
	std::ofstream fout("main.pid");
	assert(fout.is_open()==true);
	fout<<getpid();
	fout.close();

	/*Обязательная инициализация*/
	pthread_spin_init(&spin,PTHREAD_PROCESS_PRIVATE);


	/*создаем потоки в рамках процесса*/
	pthread_t ath[7];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE); //все присоединяемые
	int res=pthread_create(ath,&attr,hndl_mutex_lock,nullptr);
	assert(res==0);
	res=pthread_create(ath+1,&attr,hndl_mutex_lock_2,nullptr);
	assert(res==0);
	res=pthread_create(ath+2,&attr,hndl_spin_lock,nullptr);
	assert(res==0);
	res=pthread_create(ath+3,&attr,hndl_spin_lock_2,nullptr);
	assert(res==0);
	res=pthread_create(ath+4,&attr,hndl_rwlock_lock_1,nullptr);
	assert(res==0);
	res=pthread_create(ath+5,&attr,hndl_rwlock_lock_2,nullptr);
	assert(res==0);
	res=pthread_create(ath+6,&attr,hndl_rwlock_lock_3,nullptr);
	assert(res==0);

	/*забираем результат работы всех потоков*/
	void * p_ret;
	for(int i=0;i<7;++i){
		pthread_join(ath[i],&p_ret);
		std::cout<<i+1<<" присоединен\n";
	}

	/*уничтожения*/
	std::cout<<"Уничтожение объектов...\n";
	pthread_mutex_destroy(&mutex);
	pthread_spin_destroy(&spin);
	pthread_rwlock_destroy(&rwlock);

	std::cout<<"Готово!\n";
	return 0;
}
