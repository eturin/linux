#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>
#include <fcntl.h>
#include <assert.h>
#include <stdlib.h>

#define LEN 100

int main(){
	//получаем дескрипторы каналов
	int in1=open("in1",O_RDONLY|O_NONBLOCK);
	assert(in1);
	int in2=open("in2",O_RDONLY|O_NONBLOCK);
	assert(in2);

	long long sum=0;
	int is_in1=1,is_in2=1;
	//мультиплексирование
	fd_set set;
	while(is_in1 || is_in2){
		//инициализация битового поля
		FD_ZERO(&set);	
		if(is_in1)
			FD_SET(in1,&set);
		if(is_in2)
			FD_SET(in2,&set);

		//заводим будильник
		int max=in1>in2 ? in1 : in2; //количество проверяемых битов от начала
		select(max+1,&set,NULL,NULL,NULL);

		//обработка 
		if(FD_ISSET(in1,&set)){
			char data[LEN];
			int len=read(in1,data,LEN-1);
			if(len==0 && errno!=EAGAIN)
				is_in1=0;
			else if(len!=-1){
				data[len]='\0';
				sum+=atoll(data);			
			}
		}
		if(FD_ISSET(in2,&set)){
			char data[LEN];
			int len=read(in2,data,LEN-1);
			if(len==0 && errno!=EAGAIN)
				is_in2=0;
			else if(len!=-1){
				data[len]='\0';
				sum+=atoll(data);			
			}				
		}

	}

	//закрываем каналы
	close(in1);
	close(in2);

	printf("%lld\n",sum);

	return 0;
}

