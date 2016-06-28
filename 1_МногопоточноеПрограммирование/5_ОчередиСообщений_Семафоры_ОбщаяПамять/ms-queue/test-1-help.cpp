#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(){
	char path[]="/tmp/msg.temp";

	/*формируем ключ на основе файла*/
	int key=ftok(path,1);

	/*создаем очередь*/
	int id=msgget(key,0);
	assert(-1!=id);

	/*пишем в очередь*/
	struct message {
	    long mtype;
	    enum {LEN=80};
	    char mtext[LEN];
	} msg={2L,"Тестовое сообщение."};
     	int size=msgsnd(id,&msg,msg.LEN,0);	
	
	return 0;
}
