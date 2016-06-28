#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <iostream>

static void hndl_on_cancel(void * p_arg){
	std::cout<<(char*)p_arg<<std::endl;
}

void * hndl_func(void * p_val){
	/*Назначение запуска функции, если прервали или вызвали явное завершение.
	 *!!!Использовать макрос только в паре с pop!!!
	 *(это как скобка открылась и потом должна закрыться)*/
	pthread_cleanup_push(hndl_on_cancel,(void*)"За что?!\n");

	/*этой командой можно проверять отправку прерывания, чтоб не вставлять лишних системных вызово
	pthread_testcancel();   */

	/*поток может запретить себя останвдивать*/
	//int i_new=PTHREAD_CANCEL_DISABLE,i_old;
	//pthread_setcancelstate(i_new,&i_old);

	int * val=(int*)p_val;
	*val=1;
	sleep(2);
	*val=2;
	sleep(2);
	*val=3;

	/*Обязательная завершаяющая часть макроса push*/
	pthread_cleanup_pop(0);//если аргумент отличен от нуля, то вызывать хендлер в любом случае

	return p_val;
}

int main(){
	/*создаем поток в рамках процесса*/
	pthread_t th;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	int val=50;
	void * p_val=&val;
		
	int res=pthread_create(&th,&attr,hndl_func,p_val);
	assert(0==res);
	
	sleep(1);
	std::cout<<"Поток успел поменять значение переменной один раз: val = "<<val<<std::endl;

	/*Прерываем поток (возвращаемого значения не будет, как kill -s 9)
	 *Прерывание возможно только при ожидании системного вызова в убиваемом потоке (read, waitpid, sleep и т.д. 65 шт.) - это точки завершения.
	 *Если в потоке молотится цикл while (true) {}, то убить нельзя.
	 *Нельзя при free, malloc, realloc, calloc
	 *Можно вставить руками точку завершения:
	 *	pthread_testcancel()
	 *Поток может запретить прерывание себя.
	 */
	int old_type;
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,&old_type);//убить не смотря на всякие протесты(не рекомендуется и негарантируется)
	pthread_cancel(th);
	sleep(4);
	std::cout<<"Поток не успел(если его смогли остановить) поменять значение переменной второй раз(или даже третий): val = "<<val<<std::endl;

	pthread_attr_destroy(&attr);


	return 0;
}
