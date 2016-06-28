#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

const int LEN=512;

int main(){
	//формируем ключ
	key_t key=ftok("gale",'b');
	//получаем регион общей памяти
	int id=shmget(key,LEN,0);
	if(-1==id){
		perror("Ошибка получения региона общей памяти");
		return 1;
	}
	//получаем указатель на начало региона
	void * ptr=shmat(id,NULL,SHM_RDONLY);
	if((void*)-1==ptr)
		perror("Ошибка получения адреса региона");
	else{
		//чиатаем 
		int * i=(int*)ptr;
		char * name=(char*)(ptr+sizeof(i));
		printf("i = %d name = %s\n",*i,name);

		//отключаем регион памяти
		shmdt(ptr);
	}

	//удаляем регион
	shmctl(id,IPC_RMID,NULL);

	return 0;
}
