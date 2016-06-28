#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define LEN 1000

int main(int argc, char ** argv){
	//делаем ключ
	key_t key_a=ftok(argv[0],'a');
	assert(key_a!=-1);
	key_t key_b=ftok(argv[0],'b');
	
	//формируем регионы памяти
	int id_a=shmget(key_a,LEN,IPC_CREAT|0666);
	if(id_a==-1){
		perror("Ошибка создания первого региона");
		return 1;
	}
	int id_b=shmget(key_b,LEN,IPC_CREAT|0666);
	if(id_b==-1){
		perror("Ошибка создания второго региона");
		shmctl(id_a,IPC_RMID,NULL);
		return 2;
	}

	//получаем адреса регионов
	int * pa=(int*)shmat(id_a,NULL,0);
	if(*pa==-1){
		perror("Ошибка получения адреса первого региона");
		shmctl(id_a,IPC_RMID,NULL);
		shmctl(id_b,IPC_RMID,NULL);
		return 3;
	}
	int * pb=(int*)shmat(id_b,NULL,0);
	if(*pa==-1){
		perror("Ошибка получения адреса второго региона");
		shmctl(id_a,IPC_RMID,NULL);
		shmctl(id_b,IPC_RMID,NULL);
		return 4;
	}

	//заполняем регионы
	int i=0;
	for(i=0;i<100;++i)
		*pa++ = *pb++ = i;
	
	//закрываем регионы
	shmdt(pa);
	shmdt(pb);

	//сообщаем регионы
	printf("%d %d\n",key_a,key_b);

	return 0;
}
