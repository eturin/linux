#include <iostream>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

static int CNT=16;

int main(){
	const char path[]="/tmp/sem.temp";
	/*удаляем файл*/
	unlink(path);
	/*создаем файл*/
	int fd=open(path,O_CREAT|0666);
	close(fd);
	assert(-1!=fd);
	/*формируем ключ на основе файла и второго параметра*/
	int key=ftok(path,0);
	/*создаем семафор в ядре*/
	int id=semget(key,CNT,IPC_CREAT|IPC_EXCL|0666);
	assert(-1!=id);
	/*инициализация элементов семафора*/

	for(int i=0;i<CNT;++i){

		int res=semctl(id,i,SETVAL,i);
		std::cout << res<<": "<<semctl(id,i,GETVAL,nullptr)<<std::endl; 
	}

	semctl(id,0,IPC_RMID,nullptr);
	return 0;


}
