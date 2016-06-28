#include <iostream>
#include <omp.h>
#include <unistd.h>
static const int LEN=5;
int main(){
	/*все параллельные фрагменты нужно собирать в одну большую секцию, чтоб избежать потерь на создание и уничтожение потоков*/
	#pragma omp parallel num_threads(7)
	{
		/*распаралеливание цикла с барьером синхронизации потоков в конце*/
		#pragma omp for
		for(int i=0;i<LEN;++i)
			std::cout<<i<<std::endl;

		/*распаралеливание без барьера в конце (nowait)*/
		#pragma omp for nowait
		for(int i=0;i<LEN;++i)
			std::cout<<"->"<<i<<std::endl;
		std::cout<<"Поток перед явным барьером"<<std::endl;
		/*явный барьер синхронизации (все потоки собирутся перед ним и потом уже все двинутся дальше)*/
		#pragma omp barrier

		/*секции параллелности, где каждую секцию можно пройти только один раз (если секций больше чем потоков, то в оставшиеся секции будут ждать своего "героя", когда он освободится)*/
		#pragma omp sections nowait //nowait - удаляет барьер на выходе из секций
		{
			#pragma omp section
			{
				sleep(1);
				std::cout<<omp_get_thread_num()<<")Только один поток зашел в 1 секцию\n";
			}

			#pragma omp section
			std::cout<<omp_get_thread_num()<<")И во 2 секции только один\n";
		}
		std::cout<<"Поток после секций\n";

	}
	return 0;
}
