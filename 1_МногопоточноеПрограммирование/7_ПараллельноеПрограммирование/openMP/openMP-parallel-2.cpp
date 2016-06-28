/*жесткое указание количества потоков для распаралелливания, можно указать в переменной среды*/
//export OMP_NUM_THREADS=7;

#include <iostream>
#include <omp.h>

int main(){
	std::cout << "Следующий блок пускаем в параллель\n";

	/*блок для распараллеливания*/
	#pragma omp parallel 
	{
		std::cout<<"7 - потоков\n";
	}

	#pragma omp parallel num_threads(2)
	{
		std::cout<<"2 - потока (т.к. жестко задано количество!!!)\n";
	}

	return 0;
}
