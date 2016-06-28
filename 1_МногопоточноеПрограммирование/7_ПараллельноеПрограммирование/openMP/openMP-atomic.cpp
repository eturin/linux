#include <omp.h>
#include <iostream>
static const int LEN=10;
int main(){
	/*если общий ресурс объявить внутри параллельной секции, то он будет своим для каждого потока*/
	int sum=0;
	/*параллельная секция*/
	#pragma omp parallel num_threads(7)
	{
		#pragma omp for
		for(int i=0;i<LEN;++i){
			#pragma omp atomic //выполнить следующую операцию без прерываний и гонок памяти(не повредится переменная значит)
			sum+=i;

			#pragma omp critical //это более тяжелая форма (т.к. ставятся и снимаются барьеры)
			std::cout<<"=>"<<sum<<std::endl;
		}
		std::cout<<"sum = "<<sum<<std::endl;

	}
	

	return 0;
}
