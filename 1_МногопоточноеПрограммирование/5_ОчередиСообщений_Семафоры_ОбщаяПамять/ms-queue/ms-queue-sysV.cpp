#include <iostream>
#include <bitset>
#include <ctime>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>

int main(int cntarg, char ** arg){
	std::cout <<"pid: "<<getpid()<<std::endl;

	/*формируем ключ на основе файла*/
	const char * path=arg[0];
	int key=ftok(path,'a');

	/*создаем очередь в ядре*/
	int id=msgget(key,IPC_CREAT|0666);
	//id=msgget(IPC_PRIVATE,0666); //в этом случае не требуется файл на диске как ключ (для fork, т.к. нет ключа в этом случае, а без ключа не подключиться к очереди)
	if(id==-1)
		throw "Не удалось создать очередь.";
	
	/*отправка сообщений*/
	{
		struct msg{
		       long type;      /* >0 и рекомендуется использовать для каждого типа свою очередь*/
		       enum {LEN=256};
	       	       char text[LEN]; /*любая структура на этом месте*/
		} cur_msg={1L,"Текст сообщения."};
		if(-1==msgsnd(id,&cur_msg,cur_msg.LEN,IPC_NOWAIT))
			throw "Не удалось отправить сообщение.";

	}
	
	/*чтение сообщения*/
	{
		
		struct msg{
		       long type;
		       enum {LEN=256};
	       	       char text[LEN]; /*любая структура на этом месте*/
		} cur_msg;
		long  type=1L; //>0 конкретный тип извлекаемого сообщения
		               //=0 любой тип
			       //<0 найти сообщение с наибольшим типом не превышающем |type|
		if(-1==msgrcv(id,&cur_msg,cur_msg.LEN,type,IPC_NOWAIT))
			throw "Ошибка чтения сообщения.";
		std::cout<<"Сообщени: "<< cur_msg.text<<std::endl;
	}
	/*получение статистики*/
	{
		msqid_ds data;
		if(-1==msgctl(id,IPC_STAT,&data))
			throw errno;
		std::cout << "Права доступа:\n" 
			<< "\tid владелеца: " <<data.msg_perm.uid<<std::endl
		        << "\tid его группы: "<<data.msg_perm.gid<<std::endl
			<< "\tid создателя: " <<data.msg_perm.cuid<<std::endl
			<< "\tid его группв: "<<data.msg_perm.cgid<<std::endl
			<< "\tмаска: "        <<std::bitset<9>(data.msg_perm.mode)<<std::endl
			<< "Количество сообщений в очереди: "<<data.msg_qnum<<std::endl
			<< "Максимальный размер очереди (байт): "<< data.msg_qbytes<<std::endl
			<< "pid последнего славшего  сообщение: "<<data.msg_lspid<<"\t время: "<<ctime(&data.msg_stime);
		std::cout<< "pid последнего читавшего сообщение: "<<data.msg_lrpid<<"\t время: "<<ctime(&data.msg_rtime);
		std::cout<< "Время последней команды msgctl: "<<ctime(&data.msg_ctime);
	}

	/*удаление очереди в ядре*/
	{
		msqid_ds data;
		if(-1==msgctl(id,IPC_RMID,&data))
			throw errno;
	}

	//pause();
	return 0;
}
