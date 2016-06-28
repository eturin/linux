#include <iostream>
#include <bitset>
#include <ctime>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <assert.h>

/*!!!Обязательно описать union!!!*/
union semun {
        int val;                /* значение для SETVAL             */
       	struct semid_ds *buf;   /* буффер   для IPC_STAT & IPC_SET */
        ushort *array;          /* array    для GETALL & SETALL    */
       	struct seminfo *__buf;  /* буффер   для IPC_INFO           */
        void *__pad;
};

int main(int cntarg, char ** argc){
	/*создаем массив семафоров*/
	int cnt=5;
	//int id=semget(IPC_PRIVATE,cnt,IPC_EXCL|IPC_CREAT); //создаем семафор (только для fork, т.к. нет ключа)
	int key=ftok(argc[0],'A');
	int id=semget(key,cnt,IPC_EXCL|IPC_CREAT|0666); 
	assert(-1!=id);

	/*инициализация элементов семафораб если 0 не подходит*/
	{
		semun arg;
		for(int i=0;i<cnt;++i){
			arg.val=i;
			semctl(id,i,SETVAL,arg);
		}
	}
	
	/*получение сведений о значения элементов семафора*/
	{
		for(int i=0;i<cnt;++i)
			std::cout <<i<<": "<< semctl(id,i,GETVAL,nullptr)<<std::endl;
	}


	/*получение статистики семафора*/
	{
		struct semid_ds buf;
		semun arg;
		arg.buf=&buf;
		int res=semctl(id,0,IPC_STAT,arg);
		std::cout<< "Права доступа:\n" 		
			<< "\tid владелеца: " <<buf.sem_perm.uid<<std::endl
			<< "\tid его группы: "<<buf.sem_perm.gid<<std::endl
			<< "\tid создателя: " <<buf.sem_perm.cuid<<std::endl
			<< "\tid его группв: "<<buf.sem_perm.cgid<<std::endl
			<< "\tмаска: "        <<std::bitset<9>(buf.sem_perm.mode)<<std::endl
			<<"\t(unsigned) количество элементов семафора: "<<buf.sem_nsems<<std::endl
			<<"\tвремя последнего вызова: "<<ctime(&buf.sem_otime);
		std::cout<<"\tвремя последнего вызова semctl: "<<ctime(&buf.sem_ctime);
	}

	/*прочие общие сведения*/
	for(int i=0;i<cnt;++i){
		std::cout<<"pid последнего обратившегося процесса: "<<semctl(id,i,GETPID,nullptr)<<std::endl;
		std::cout<<"Кол-во процессов, ожидающих повышение значения "<<i<<": "<<semctl(id,i,GETNCNT,nullptr)<<std::endl;
		std::cout<<"Кол-во процессов, ожидающих 0 "<<i<<": "<<semctl(id,i,GETZCNT,nullptr)<<std::endl;
	}

	/*операции над набором семафоров (одновременно как транзакция)*/
	{
		sembuf sops;
		sops.sem_num=3; //номер элемента
		sops.sem_op=99; //>0 то увеличиваем на указанную дельту
		                //<0 то уменьшаем на указанную дельту (если уходит в минус, то будем ждать пока можно будет вычесть оставшееся, т.е. пока кто-то прибавит)
			        //=0 то просто ждем, когда семафор сам дойдет до нуля
		sops.sem_flg=SEM_UNDO; //IPC_NOWAIT - не блокирующий режим
			               //SEM_UNDO - если процесс умрет, то все что он наплюсовал вычтется 
		int res=semop(id,&sops,1);//если не один элемент, то нужно передать массив и 1 заменить на кол-во элементов массива
	
	}
	
	
	/*получение значений всех элементов семафора*/
	{
		semun arg;
		arg.array=new unsigned short[cnt];
		int res=semctl(id,0,GETALL,arg); //можно все установить SETALL
		assert(-1!=res);
		for(int i=0;i<cnt;++i)
			std::cout<<i<<": "<<arg.array[i]<<std::endl;
		delete [] arg.array;
	}

	/*удаление семафора из ядра*/
	int res=semctl(id,0,IPC_RMID,nullptr);
	assert(-1!=res);

	return 0;
}
