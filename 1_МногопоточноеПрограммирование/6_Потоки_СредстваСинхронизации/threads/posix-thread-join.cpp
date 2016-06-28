//!!!!Память у всех потоков общая!!!!!
#include <pthread.h>
#include <stdexcept>
#include <iostream>
#include <unistd.h>

void * hndl_func(void * arg){
	int * ptr_val=(int*)arg;
	++*ptr_val;
	return arg;
}

int main(){
	try{
		/*создаем отдельный поток в процессе 
		 * (joinable, если не указано иное, т.е. поток будет ждать возможности вернуть результат)
		*/
		pthread_t   id;         //идентификатор потока
		pthread_attr_t attr;     //атрибуты для запуска
		int res=pthread_attr_init(&attr);//инициализация атрибутов
		/*можно заранее сделать поток joinable*/
		//pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
		int val=10;
		void * arg=(void*)&val; //аргумент, передаваемый в функцию
		res=pthread_create(&id,&attr,hndl_func,arg);//будет вызвана функция в потоке и ей передан аргумент
		if(0!=res)
			throw std::runtime_error("Не удалось создать поток.");
		
		std::cout<<"Поток не успел изменить значение 10 в переменной: val = "<<val<<std::endl;
		sleep(1);
		std::cout<<"Поток уже отработал и переменная обновлена: val = "<<val<<std::endl;
		

		/*если не нужно, что бы поток ждал, когда у него заберут результат (!обратно сменить тип нельзя!)*/
		//res=pthread_detach(id);//в этом случае, поток завершается с потерей возвращаемого значения(но будет работать после смерти основного потока

		/*Обязательно присоединяем поток(ждем) и забираем возвращаемое значение функции, 
		 *т.к. поток будет ждать возможности вернуть результат.
		 *Два потока не могут ждать друг друга. 
		 */
		void * ret_val;
		res=pthread_join(id,&ret_val);
		if(0!=res)
			throw std::runtime_error("Не удалось присоединить поток и забрать возвращаемое значение.");
		std::cout<<"Функция в потоке вернула: "<<*(int*)ret_val<<std::endl;
		
		pthread_attr_destroy(&attr);//удаление атрибутов
	}catch(std::runtime_error & e){
		std::cout <<"Поймана ошибка:\n"<< e.what()<<std::endl;
		std::cout.flush();
		throw;
	}
	return 0;
}
