#include <semaphore.h>


int main(){
	/*создание семафора*/
	sem_t s;
	int pshared;//общая память
	int res=sem_init(&s,pshared,0); //последний параметр - начальное значение

	/*в остальном как обычный семафор posix*/

	/*удаление*/
	res=sem_destroy(&s);

	return 0;
}
