#include <omp.h>
#include <iostream>

/* По умолчанию вложенные параллельные секции выполняются одним потоком.
 * Можно выставить переменную окруждения OMP_NESTED, вызвать функцию omp_set_nested(true)  
 * и все вложенные параллельные секции будут распараллеливаться снова(а встретивший секцию поток будет становиться мастером) */

int main(){
	omp_set_nested(true);
	std::cout<<"Распараллеливание включено\n";
	#pragma omp parallel num_threads(2)
	{
		int n=omp_get_thread_num();
		#pragma omp parallel num_threads(3)
		std::cout<<"поток "<<n<<" породил => "<<omp_get_thread_num()<<std::endl;//ноумерация потоков меняется. Исходный поток получает номер 0 (и таких становится несколько!!!)
	}

	omp_set_nested(false);
	std::cout<<"Распараллеливание выключено (потоки не порождаются, но меняется номер потока на номер мастера)\n";
	#pragma omp parallel num_threads(4)
	{
		int n=omp_get_thread_num();
		#pragma omp parallel num_threads(3)
		std::cout<<"поток "<<n<<" породил => "<<omp_get_thread_num()<<std::endl;
	}
	return 0;
}
