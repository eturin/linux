#include <iostream>
#include <omp.h>

int main(){
	/*Блок будет распараллелен (с общей памятью, т.е. все переменные модуля доступны всем)
	 * В каждом потоке будет выполняться один и тот же код.*/
	#pragma omp parallel
	{
		std::cout << "1 - Хеллоу, Пузан\n";
	}

	/*жестко задаем количество потоков, на которые нужно распараллелить*/
	#pragma omp parallel num_threads(4)
	{
		std::cout << "4 - Хеллоу, Пузан\n";
	}

	/*или так*/
	omp_set_num_threads(3);
	#pragma omp parallel 
	{
		/* внутри параллельной секции нельзя менять количество потоков*/
		std::cout << "3 - Хеллоу, Пузан\n";
	}

	#pragma omp parallel 
	{
		/* это игнорируется */
		#pragma omp parallel
		std::cout << "6 - Хеллоу, Пузан\n";
	}

	return 0;
}
