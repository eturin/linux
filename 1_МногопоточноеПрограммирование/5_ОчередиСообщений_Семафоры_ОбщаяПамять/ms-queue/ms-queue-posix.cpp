#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>

int main(){
	try{
		const char name[]="/любая строка";
		/*удаляем очередь*/
		mq_unlink(name);
		
		/*создаем новую очередь (O_EXCL - выдавать ошибку, если такая очередь уже есть)*/
		mqd_t id=mq_open(name,O_CREAT|O_WRONLY|O_EXCL,0666,nullptr);
		if(-1==id)
			throw "Не удалось создать очередь.";
		
		
		/*получение атрибутов*/
		mq_attr attr;
		if(-1==mq_getattr(id,&attr))
			throw "Не удалось получить атрибуты очереди.";			
		else
			std::cout<<"Флаги: "<<attr.mq_flags<<std::endl
				<<"Максимальное количество сообщений в очереди: "<< attr.mq_maxmsg<<std::endl
				<<"\tсейчас в очереди сообщений - "<< attr.mq_curmsgs<<std::endl
				<<"максимальный размер одного сообщения: "<<attr.mq_msgsize<<std::endl;
		/*отправка сообщения*/
		char data[]="Моё сообщение.";
		unsigned priority=1; /*приоритет [0,31] */
		if(-1==mq_send(id,data,sizeof(data),priority))
			throw "не удалось отправить сообщение в очередь.";

		
		/*закрываем очередь*/
	 	mq_close(id);

		/*открываем существующую очередь*/
		id=mq_open(name,O_RDONLY|O_NONBLOCK);
		if(-1==id)
			throw "Не удалось получить дескриптор очереди.";
		
		/*получение атрибутов*/
		if(-1==mq_getattr(id,&attr))
			throw "Не удалось получить атрибуты очереди.";			
		else
			std::cout<<"Флаги: "<<attr.mq_flags<<std::endl
				<<"Максимальное количество сообщений в очереди: "<< attr.mq_maxmsg<<std::endl
				<<"\tсейчас в очереди сообщений - "<< attr.mq_curmsgs<<std::endl
				<<"максимальный размер одного сообщения: "<<attr.mq_msgsize<<std::endl;
		
		/*чтение очереди*/
		int size=mq_receive(id,data,sizeof(data),&priority);
		if(size)
			std::cout<<"Приоритет сообщения: "<<priority<<std::endl
				<<"Содержание: "<<data<<std::endl;

		/*установка атрибутов*/
		mq_attr old_attr;
		attr.mq_maxmsg;  /*максимальное число сообщений в очереди*/
		attr.mq_flags;   /*(можно менять на лету)*/
		attr.mq_msgsize; /*(нельзя менять при создание) максимальный размер одного сообщения*/
		attr.mq_curmsgs; /*(нельзя изменить) количество сообщений в очереди*/
		if(-1==mq_setattr(id,&attr,&old_attr))
			throw "Не удалось установить атрибуты очереди.";			

		/*закрываем очередь*/
	 	mq_close(id);
		/*удаляем (удалится с последним использующим процессом)*/
		mq_unlink(name);	

	}catch(const char * err){
		std::cout <<err<<std::endl<<errno;
	}
	

	return 0;
}
