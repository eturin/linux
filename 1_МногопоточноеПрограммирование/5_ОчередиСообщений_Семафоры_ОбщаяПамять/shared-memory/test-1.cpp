#include <iostream>
#include <bitset>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

int main(){
	const char name[]="/tmp/mem.temp";

	/*удаляем файл*/
	unlink(name);
	/*создаем файл*/
	int fd=open(name,O_CREAT|0666);
	assert(-1!=fd);
	close(fd);

	/*формируем ключ на основе файла и символа*/
	int key=ftok(name,0);
	/*создаем сегмент памяти размером 1мб*/
	unsigned size=1024*1024;
	int id=shmget(key,size,IPC_CREAT|IPC_EXCL|0666);
	assert(-1!=id);
	/*присоединяемся к памяти*/
	void * ptr=shmat(id,nullptr,0);//!!!будем писать в память!!!
	assert((void*)(-1)!=ptr);
	/*заполняем память*/
	char * m=(char*)ptr;			
	for(unsigned i=0;i<size;++i)
		m[i]=42;
	/*отсоединяемся*/
	shmdt(ptr);
	
	/*получить существующую*/
	id=shmget(key,0,0);
	assert(-1!=id);
	/*получение статитстики*/
	shmid_ds buf;
	int res=shmctl(id,IPC_STAT,&buf);
	if(res!=-1){
		std::cout<<"Права доступа:\n"
			<< "\tid владелеца: " <<buf.shm_perm.uid<<std::endl
			<< "\tid его группы: "<<buf.shm_perm.gid<<std::endl
			<< "\tid создателя: " <<buf.shm_perm.cuid<<std::endl
			<< "\tid его группв: "<<buf.shm_perm.cgid<<std::endl
			<< "\tмаска: "        <<std::bitset<9>(buf.shm_perm.mode)<<std::endl
			<< "Размер сигмента общей памяти(байт): "<<buf.shm_segsz<<std::endl
			<< "pid процесса выполнявшего последнюю операцию с памятью: "<<buf.shm_lpid<<std::endl
			<< "pid процесса создателя памяти: "<< buf.shm_cpid<<std::endl
			<< "Количество процессов подключенных к этому сегменту памяти: "<<buf.shm_nattch<<std::endl;
		std::cout<< "Время последнего подключения: "<<ctime(&buf.shm_atime);
		std::cout<< "Время последнего отключения: "<<ctime(&buf.shm_dtime);
		std::cout<< "Время последнего изменения настроек: "<<ctime(&buf.shm_ctime);
	}
	/*присоединение к памяти(увязка с экземпляром типа)*/
	ptr=shmat(id,nullptr,SHM_RDONLY);//вернет приатаченный адрес, но можем попросить свой адрес (только на чтение)
	assert((void*)(-1)!=ptr);
	/*пишем в out*/
	write(STDOUT_FILENO,ptr,size);

	/*отключение от памяти*/
	shmdt(ptr); //передаем присоеденнный адрес	

	/*удаляем*/
	shmctl(id,IPC_RMID,nullptr);
	return 0;
}

