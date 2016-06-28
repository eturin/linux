#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <assert.h>
#include <time.h>

#define LEN 1000
#define LENNAME 10

int main(int argc, char ** argv){
	//проверяем количество аргументов
	assert(argc>=3);

	//ключи регионов
	key_t key_a=atoi(argv[1]), key_b=atoi(argv[2]);

	//идентификаторы регионов
	int id_a=shmget(key_a,LEN,0);
	if(id_a==-1){
		perror("Ошибка получения id первого региона");
		return 1;
	}
	int id_b=shmget(key_b,LEN,0);
	if(id_b==-1){
		perror("Ошибка получения id второго региона");
		return 1;
	}

	//получаем указатели на регионы памяти
	int *pa=(int*)shmat(id_a,NULL,SHM_RDONLY);
	if(*pa==-1){
		perror("Не удалось получить адрес первого региона");
		return 1;
	}
	int *pb=(int*)shmat(id_b,NULL,SHM_RDONLY);
	if(*pb==-1){
		perror("Не удалось получить адрес второго региона");
		shmdt(pa);
		return 2;
	}

	//формируем произвольный ключ 
	srand(time(0));
	key_t key=ftok(argv[0],rand()%255);
	if(key==-1){
		perror("Ошибка формирования ключа");
		shmdt(pa);
		shmdt(pb);
		return 3;
	}
	//создаем на основе ключа разделяемую память
	int id_out=shmget(key,LEN,IPC_CREAT|0666);
	if(id_out==-1){
		perror("Ошибка открытия/создания общего региона памяти");
		shmdt(pa);
		shmdt(pb);
		return 4;
	}
	//получаем указатель на региион памяти
	int *pout=(int*)shmat(id_out,NULL,0);
	if(*pout==-1){
		perror("Не удалось получить адрес конечного региона");
		shmdt(pa);
		shmdt(pb);
		shmctl(id_out,IPC_RMID,NULL);
		return 5;
	}

	//вычисляем суммы
	int i=0;
	for(i=0;i<100;++i)
		*pout++=*pa++ + *pb++;

	//закрываем регионы
	shmdt(pa);
	shmdt(pb);
	shmdt(pout);

	printf("%ld\n",key);
		
	return 0;
}
