#include <iostream>
#include <fcntl.h>
#include <semaphore.h>
#include <assert.h>


int main(int cnt_arg,char ** argc){
	const char name[]="/eng_only";
	/*удаляем семафор*/
	sem_unlink(name);

	/*создаем новый семафор posix*/
	sem_t * ptr_id=sem_open(name,O_CREAT|O_EXCL,0666,0);//последний параметр - начальное значение семафора (один элемент в отличие от sysV)
	assert(SEM_FAILED!=ptr_id);
	
	/*закрываем семафор*/
	sem_close(ptr_id);

	/*открываем существующий семафор*/
	ptr_id=sem_open(name,0);
	assert(SEM_FAILED!=ptr_id);

	/*увеличиваем значение семафора на 1*/
	int res=sem_post(ptr_id);
	if(-1==res)
		std::cout << "не удалось увеличить значение семафора на 1\n";

	/*уменьшает значение семафора на 1(если уже 0, то ждет пока кто-то увеличит, чтоб уменьшить)*/
	res=sem_wait(ptr_id);
	
	/*для не блокирующего режима*/
	//res=sem_trywait(ptr_id);
	/*блокирование на определенное время*/
	//sem_timedwait(ptr_id,timespec);

	/*получить текущее значение*/
	int val;
	sem_getvalue(ptr_id,&val);
	std::cout<<"Текущее значение: "<<val<<std::endl;

	/*удаляем семафор (удалится с последним использующим семафор процессом)*/
	sem_unlink(name);
	
	return 0;
}
