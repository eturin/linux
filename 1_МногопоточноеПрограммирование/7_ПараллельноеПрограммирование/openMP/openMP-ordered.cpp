#include <iostream>
#include <omp.h>
static const int LEN=4;
int main(){
	#pragma omp parallel for num_threads(4) ordered
	for(int i=0;i<LEN;i+=1){
		std::cout << "это печатается параллельно всеми\n";

		/*следующий фрагмент выполняется последовательно (по индексу i) и не более одного одновременно*/
		#pragma omp ordered
		{
			std::cout<<i<<std::endl;
		}
		i++;/*цикл параллелится равномерно и это путает его на втором шаге и последующих, а вот на входе весь диапазон разделяется равномерно по потокам*/
		std::cout <<"прошел очередь\n";
	}




	return 0;
}
