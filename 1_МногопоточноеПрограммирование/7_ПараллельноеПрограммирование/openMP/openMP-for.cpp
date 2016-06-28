#include <iostream>
#include <omp.h>
#include <cmath>
#include <atomic>

static const unsigned LEN=10;

int main(){
	double * m=new double[LEN];
	for(unsigned i=0;i<LEN;++i)
		m[i]=i*i;
	
	/*в трех потоках трижды будет извлечен квадратный корень*/
	#pragma omp parallel num_threads(3)
	{
		for(unsigned i=0;i<LEN;++i)//формат оператора for упращен, чтобы можно было определить итерации цикла; директива действует только на самый внешний цикл (не на вложенные) и итеративная переменная должна быть локальной для каждого потока
			m[i]=std::sqrt(m[i]);
	}
	
	for(unsigned i=0;i<LEN;++i)
		std::cout<<m[i]<<" ";
	std::cout<<std::endl;

	/*распараллелим цикл*/
	#pragma omp parallel for num_threads(4)
	for(unsigned i=0;i<LEN;++i)
		m[i]=i*i;

	/*или так*/
	#pragma omp parallel num_threads(2)
	{
		#pragma omp for //это работает, если только внутри блока параллельности (!!!но теперь нельзя добавлять parallel, иначе снова будут добавляться потоки!!!)
		for(unsigned i=0;i<LEN;++i)
			m[i]=std::sqrt(m[i]);

	}
	
	for(unsigned i=0;i<LEN;++i)
		std::cout<<m[i]<<" ";
	std::cout<<std::endl;
	delete [] m;


	/*можно сразу создать потоки, для обоих циклов (это дешевле, чем содзавать их перед каждой параллельной секцией)*/
	#pragma omp parallel  num_threads(7)
	{
		/*в конце цикла неявный барьер (синхронизация потоков)*/
		#pragma omp for  
		for (int i=0; i<LEN; i++) 
			std::cout<<"1-"<<i<<std::endl; 
		/*потоки покинувшие предыдущий цикл одновременно зайдут в следующий*/
		#pragma omp for  nowait
		for (int i=0; i<LEN; i++) 
			std::cout<<"2-"<<i<<std::endl;
	       	std::cout<<"Поток вышедший из цикла, сразу это печатает, и не ждет остальных\n";	
		
		/*потоки наяинают цикл как попало, но одну секцию (ordered) проходят в порядке индекса*/
		#pragma omp for  ordered
		for (int i=0; i<LEN; i++){ 
			std::cout<<"3-"<<i<<std::endl;
			#pragma omp ordered //лучше избегать эту опцию 
			{
				std::cout<<"в порядке очереди - "<<i<<std::endl;
			}
		}

		/*collapse(2) - распространяет прагму на оба вложенных цикла одновременно (образуется общее пространство итераций, которое делится между потоками)*/
		#pragma omp for collapse(2) 
		for(int i=0;i<LEN/3;++i)
			for(int j=0;j<LEN/3;++j)
				std::cout<<"поток - "<<omp_get_thread_num()<<" i="<<i<<" j="<<j<<std::endl;

		/*schedule - определяет способ распределения итераций цикла между потоками*/
		#pragma omp for schedule(static,3) //статическое распределение: нулевой поток выполнит блок i=0,1,2   , первый поток выполнит блок i=3,4,5 и т.д. (потоки последовательно получают свои фрагменты, с учетом возрастания номеров фрагментов)
		for(int i=0;i<LEN;++i)
			std::cout<<"schedule(static,3) поток "<<omp_get_thread_num()<<" i="<<i<<std::endl;
		#pragma omp for schedule(dynamic,3) //динамеческое распределение: нулевой поток выполнит блок i=0,1,2   , первый поток выполнит блок i=3,4,5 и т.д. (теперь освободившийся поток получает свою новую порцию из 3 элементов без очереди)
		for(int i=0;i<LEN;++i)
			std::cout<<"=>schedule(dynamic,3) поток "<<omp_get_thread_num()<<" i="<<i<<std::endl;
		#pragma omp for schedule(guided,3) //тоже динамеческое распределение: нулевой поток выполнит блок i=0,1,2   , а остальные будут получать пропорционально оставшемуся, но не более начальных 3 (для равномерности распределения нагрузки и балансировки остатка итераций цикла между всеми потоками)
		for(int i=0;i<LEN;++i)
			std::cout<<"schedule(guided,3) поток "<<omp_get_thread_num()<<" i="<<i<<std::endl;
		//#pragma omp for schedule(auto) //распределение выбирается автоматически при компиляции
		//#pragma omp for schedule(runtime) //распределение выбирается из переменной среды окружения (OMP_SCHEDULE="static,6"), но уже при выполнении программы


	} 
	return 0;
}
