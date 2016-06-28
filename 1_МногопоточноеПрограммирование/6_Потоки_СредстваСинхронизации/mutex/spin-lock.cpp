/*Если блокировки не долгие, то усыплять процесс будет "дорого" и лучше бесконечно проверять доступность ресурса.
 *!!!Однако процесс, выставивший spin-блокировку не прерывается(не вытесняется), но может быть остановлен обработкой прерывания!!!
 *Блокировка будет оставаться и циклы ожиданий будут крутиться, в то время как поток установивший блокировку не может получить процессорное время и снять блокировку. */
#include <pthread.h>

/*это будет общим ресурсов, к которому нужно обеспечить последовательный доступ, а не одновременный*/
volatile static int val;
/*объявляем spin*/
pthread_spinlock_t s_lock;

void * hndl_f1(void * p_arg){
	/*блокируем spin (входит в бесконечный цикл, если занято, т.е. процесс засыпать не будет)*/
	int res=pthread_spin_lock(&s_lock);
	//int res=pthread_spin_trylock(&s_lock);//это не блокирующий режим (при успехе res!=0)
	
	/*работаем с ресурсом (максимально быстро, избегать любых задержек)*/
	val=1;

	/*снимаем блокировку*/
	res=pthread_spin_unlock(&s_lock);

	return p_arg;
}

void * hndl_f2(void * p_arg){
	/*блокируем spin (входит в бесконечный цикл, если занято, т.е. процесс засыпать не будет)*/
	int res=pthread_spin_lock(&s_lock);

	/*работаем с ресурсом*/
	val=2;

	/*снимаем блокировку*/
	res=pthread_spin_unlock(&s_lock);

	return p_arg;
}

int main(){
	/*инициализируем spin*/
	//PTHREAD_PROCESS_SHARED  - для синхронизации между разными !!!процессами!!!
        //PTHREAD_PROCESS_PRIVATE - для синхронизации только между потоками процесса
	int pshared=PTHREAD_PROCESS_PRIVATE;
	pthread_spin_init(&s_lock,pshared);

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
	pthread_spin_destroy(&s_lock);//уничтожение
	return 0;
}
