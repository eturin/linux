#include <mqueue.h> 
#include <fcntl.h>
#include <assert.h>

int main(){
	const char name[]="/test.mq";
	int id=mq_open(name,O_WRONLY|O_NONBLOCK);
	char data[]="Письмо Пузану, в деревню. Доставить лично!";
	int res=mq_send(id,data,sizeof(data),1);
	assert(-1!=res);
	mq_close(id);

	return 0;
}
