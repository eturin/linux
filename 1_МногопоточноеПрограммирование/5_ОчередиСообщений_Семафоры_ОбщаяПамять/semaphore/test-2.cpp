#include <iostream>
#include <fcntl.h>
#include <semaphore.h>
#include <assert.h>

int main(){
	const char name[]="/test.sem";
	/*удаляем*/
	sem_unlink(name);

	/*создаем*/
	sem_t * ptr_id=sem_open(name,O_CREAT|O_EXCL,0666,66);
	assert(SEM_FAILED!=ptr_id);
	
	/*смотрим текущее значение*/
	int val;
	int res=sem_getvalue(ptr_id,&val);
	if(res!=-1)
		std::cout<<"Текущее значение - "<<val<<std::endl;

	return 0;
}
