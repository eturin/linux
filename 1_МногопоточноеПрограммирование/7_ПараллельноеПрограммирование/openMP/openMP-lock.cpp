#include <iostream>
#include <omp.h>

int main(){
	/*объявляем mutex*/
	omp_lock_t lock_1;
	/*!!!обязательно инициализируем его!!!*/
	omp_init_lock(&lock_1);
	#pragma omp parallel num_threads(3)
	{
		#pragma omp sections
		{
			#pragma omp section
			{
				omp_set_lock(&lock_1); //устанавливать и снимать надо в одном и том же потоке
				std::cout<<"После установки блокировки 1\n";
				omp_unset_lock(&lock_1);
			}
			#pragma omp section
			{
				omp_set_lock(&lock_1);
				std::cout<<"После установки блокировки 2\n";
				omp_unset_lock(&lock_1);
			}
			#pragma omp section
			{
				omp_set_lock(&lock_1);
				std::cout<<"После установки блокировки 3\n";
				omp_unset_lock(&lock_1);
			}
		}
	}

	return 0;
}
