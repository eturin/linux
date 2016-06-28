#include <iostream>
#include <omp.h>
#include <unistd.h>

static const int LEN=5;
int main(){
	/*параллельная секция*/
	#pragma omp parallel num_threads(2) //после параллельной секции дополнительные потоки уничтожаются 
	{
		/*создаем самостоятельные присоединяемые потоки*/
		#pragma omp task 
		{
			for(int i=0;i<LEN;++i){
				sleep(1);
				std::cout<<"Поток в свободном плавание\n";
			}
		}	

		#pragma omp single nowait //выполнится один раз
		{
			std::cout<<"Самостоятельные потоки запущены\n";
			#pragma omp taskwait //присоединяем эти потоки
			std::cout<<"Самостоятельные потоки присоединены\n";
		}

		std::cout<<"Поток после после одиночной секции\n";


	}

	return 0;
}

