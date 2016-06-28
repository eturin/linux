#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(){
	char path[]="/tmp/msg.temp";

	/*удаляем файл*/
	unlink(path);

	/*создаем файл*/
	int fd=open(path,O_CREAT,0666);
	assert(-1!=fd);
	/*закрываем файл*/
	close(fd);

	/*формируем ключ на основе файла*/
	int key=ftok(path,1);

	/*создаем очередь*/
	int id=msgget(key,IPC_CREAT|0666);
	assert(-1!=id);

	/*читаем из очереди*/
	fd = open("message.txt",O_CREAT|O_WRONLY,0666);
	assert(-1!=fd);
	struct message {
	    long mtype;
	    enum {LEN=80};
	    char mtext[LEN];
	} msg;
     	do{
		int size=msgrcv(id,&msg,msg.LEN,0,0);	
		write(fd,msg.mtext,size);
	}while(true);
	close(fd);	

	/*закрываем очередь*/
	msgctl(id,IPC_RMID,nullptr);
	
	return 0;
}
