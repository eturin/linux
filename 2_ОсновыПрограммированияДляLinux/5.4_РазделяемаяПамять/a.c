#include <sys/shm.h>
#include <sys/ipc.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

const int LEN=512;

int main(int argc,char ** argv){
	//формируем ключ на основе строки и символа
	int key=ftok("gale",'b');
	//получаем(создаем, если нет) регион памяти в адресном пространстве (ipcs - для проверки того, что насоздавали)
	int id=shmget(key,LEN,IPC_CREAT|0666);
	if(id==-1){
		perror("Ошибка получения региона");
		return 1;
	}

	//Получаем указатель на регион общей памяти 
	void * ptr=shmat(id,NULL,0);//вернет приатаченный адрес, но можем попросить свой адрес (только на чтение)
	if((void*)-1==ptr)
		perror("Ошибка подключения к региону общей памяти");
	else{
		//пишем в регион
		int *i=(int*)ptr;
		*i=1980;
		char *name=(char*)(ptr+sizeof(i));
		strncpy(name,"Галина",sizeof("Галина"));	

		//отключение от региона памяти
		shmdt(ptr);	
	}

	//удаление региона
	//shmctl(id,IPC_RMID,NULL);

	return 0;
}
