#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <assert.h>

void * hndl_thread(void * p_arg){
	pause();
}

int main(){
	/*пишем в текстовый файл свой pid*/
	const char name[]="main.pid";
	unlink(name);
	FILE * file=fopen(name,"w");//будут права 0666
	fprintf(file,"%d",getpid());
	fclose(file);
	chmod(name,0666);

	/*создаем поток*/
	pthread_t th;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE); //присоединяемый поток
	int res=pthread_create(&th,&attr,hndl_thread,nullptr);
	assert(0==res);

	/*ждем завершения*/
	void * p_ret;
	res=pthread_join(th,&p_ret);

	pthread_attr_destroy(&attr);

	return 0;
}
