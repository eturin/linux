#include <pthread.h>
#include <assert.h>
#include <iostream>

void * hndl_func(void * ptr_val){
	if(*(int*)ptr_val==97){
		*(int*)ptr_val=100;
		pthread_exit(ptr_val);//явное завершение потока в этом месте (аналогично обычному)
	}
	*(int*)ptr_val=97;
	return ptr_val; //нормальное (не явное) завершение потока в этом месте
}

int main(){
	/*создаем поток в рамках процесса*/
	pthread_t th;
	pthread_attr_t attr;
	int res=pthread_attr_init(&attr);//инициализация
	//pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE); //делаем заранее присоединяемый поток(по умолчанию так и будет)
	int val=100;
	void * ptr_val=&val;
	res=pthread_create(&th,&attr,hndl_func,ptr_val);

	/*ждем завершения потока*/
	void * ptr_ret;
	res=pthread_join(th,&ptr_ret);
	if(0!=res)
		std::cout<<"res = "<<res<<std::endl;
	else
		std::cout<<"1 поток успешно отработал:\n\tval = "<<val<<"\n\t*ptr_ret = "<<*(int*)ptr_ret<<std::endl; 

	/*снова создаем поток*/
	val=97;
	res=pthread_create(&th,&attr,hndl_func,ptr_val);
	/*ждем снова*/
	res=pthread_join(th,&ptr_ret);
	if(0!=res)
		std::cout<<"res = "<<res<<std::endl;
	else
		std::cout<<"2 поток успешно отработал:\n\tval = "<<val<<"\n\t*ptr_ret = "<<*(int*)ptr_ret<<std::endl; 
	
	pthread_attr_destroy(&attr);//удаление

	return 0;
}
