#include <iostream>
#include <omp.h>

static const int LEN=10;

int main(){

	#pragma omp parallel num_threads(5) //без фигурных скобок это относится только к одному оператору
	std::cout << 1<<" все потоки печатают"<<std::endl;

	int k;
	#pragma omp parallel num_threads(3) private(k) /*для каждого потока будет создана без инициализации своя переменная с именем k*/
	{
		std::cout << 2 << std::endl;
		std::cout <<"встал перед барьером, пока все потоки не дойдут к нему"<<std::endl;

		#pragma omp barrier
		std::cout << "прошел барьер\n";


		/*следующий блок или оператор выполнится только один раз в каком-то одном потоке, а остальные БУДУТ ЕГО ЖДАТЬ!!! (как на барьере)*/
		#pragma omp single //если надо, чтоб это был не произвольный поток, а главный - то singal заменяем на master (но тогда нет барьера в конце)
		std::cout << 3<<" печатает какой-то один поток, а остальные не смогут обогнать его и пройти дальше!"<< std::endl;

		std::cout <<4<<" снова печатают все,но после завершения печати 3"<<std::endl;

		/*следующий блок или оператор выполнится снова кем-то одним, но остальные теперь ждать завершения не будут*/
		#pragma omp single nowait
		{
			std::cout << 5<<" это печатет какой-то один поток, а остальные пропускают и просто идут дальше не дожидаясь"<<std::endl;
		}

		std::cout <<6<<" печатают все"<<std::endl;

		/*copyprivate(k) один какой-то поток инициализирует все локальные переменные с именем k у всех потоков(nowait - не допустима)*/
		#pragma omp single copyprivate(k)
		k=1;
		
		std::cout<<"k = "<<k<<std::endl;
	}

	#pragma omp parallel for num_threads(4)
	for(int i=0;i<LEN;++i){
		std::cout<<"Перед критической секцией"<<std::endl;
		#pragma omp critical
		std::cout<<"Критическая секция\n";
		
		/*ТАКОЙ СЕКЦИИ В ЦИКЛЕ БЫТЬ НЕ МОЖЕТ
		#pragma omp single
		{
			std::cout<<"Одиночная секция\n";
		}*/
		std::cout<<"Финал\n";
	}

	return 0;
}
