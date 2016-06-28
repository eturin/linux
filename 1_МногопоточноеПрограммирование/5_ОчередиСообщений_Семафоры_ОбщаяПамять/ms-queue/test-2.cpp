#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <mqueue.h>
#include <assert.h>
#include <signal.h>

//const int LEN=8192;

static int fd,id;

void setsig(int& id){
	struct sigevent sev;
	sev.sigev_notify=SIGEV_SIGNAL;
	sev.sigev_signo=SIGUSR1;
	int res=mq_notify(id,&sev);
	std::cout <<"Set Sig: "<<res<<std::endl;	
}

void hndl_mysig(int signum,siginfo_t * inf,void * content){
	setsig(id);
	/*определяем размер сообщения*/
	struct mq_attr attr;
	mq_getattr(id,&attr);
	unsigned len=attr.mq_msgsize;
	
	/*получаем сообщение*/
	char * buf=new char[len+1];
	unsigned pri;
	int size;
	do{
		size=mq_receive(id,buf,len,&pri);
		if(-1!=size){
			buf[size]='\0';
			std::cout <<size<<" "<<buf<<std::endl;
			write(fd,buf,len);
		} 
	}while(-1!=size);
	delete [] buf;
}

int main(){
	const char name[]="/test.mq";
	
	/*удаляем очередь*/
	mq_unlink(name);
	
	/*создаем очередь (неблокирующую для асинхронного чтения)*/
	id=mq_open(name,O_CREAT|O_RDONLY|O_EXCL|O_NONBLOCK,0666,nullptr);
	assert(-1!=id);
	
	/*читаем из очереди*/
	/*int fd=open("message.txt",O_CREAT|O_WRONLY,0666);
	char data[LEN];
	unsigned pri;
	do{
		int size=mq_receive(id,data,LEN,&pri);
		write(fd,data,size);
	

	}while(true);
	*/

	/*читаем асинхронно*/
	fd=open("message.txt",O_CREAT|O_WRONLY,0666);
	/*обработчик сигнала*/
	struct sigaction acc,old_acc;
	acc.sa_flags=SA_SIGINFO;
	acc.sa_sigaction=hndl_mysig;
	sigaction(SIGUSR1,&acc,&old_acc);
	/*появление сообщений вызовет генерацию сигнала*/
	setsig(id);
	/*ждем сигнала*/
	while(true){
		std::cout<<"Ждем сигнала...";
		pause();
	}
	
	/*закрываем*/
	close(fd);
	mq_close(id);
	mq_unlink(name);
	std::cout<<"Bay\n";
	return 0;
}
