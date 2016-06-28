#include <pthread.h>
#include <stdexcept>
#include <iostream>
#include <unistd.h>

void * hndl_func(void * arg){
	int * ptr_val=(int*)arg;
	++*ptr_val;
	std::cout<<"Сообщение из потока: *ptr_val = "<<*ptr_val<<std::endl;
	return arg;
}

int main(){
	try{
		/*создаем отдельный поток в процессе 
		 * (joinable, если не указано иное, т.е. поток будет ждать, чтоб вернуть результат)
		*/
		pthread_t   id;          //идентификатор потока
		pthread_attr_t attr;     //атрибуты для запуска
		int res=pthread_attr_init(&attr);//инициализация атрибутов
		/*можно заранее сделать поток detached*/
		//pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
		int val=10;
		void * arg=(void*)&val; //аргумент, передаваемый в функцию
		res=pthread_create(&id,&attr,hndl_func,arg);//будет вызвана функция в потоке и ей передан аргумент
		if(0!=res)
			throw std::runtime_error("Не удалось создать поток.");
		
		/*меняем тип потока (будет прерван с завершение основного потока)*/
		res=pthread_detach(id);//в этом случае, поток завершается с потерей возвращаемого значения
		
		sleep(1);
		pthread_attr_destroy(&attr);//удаление атрибутов

	}catch(std::runtime_error & e){
		std::cout <<"Поймана ошибка:\n"<< e.what()<<std::endl;
		std::cout.flush();
		throw;
	}
	return 0;
}
