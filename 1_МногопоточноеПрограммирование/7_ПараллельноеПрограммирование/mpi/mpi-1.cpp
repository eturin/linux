/* компиляция mpic++ -std=c++11 -o a mpi-1.cpp (получится программа с именем a)
 * запуск mpiexec -l -n 5 ./a  (будет 5 процессов)
 * */

#include <iostream>
#include <mpi.h>

int main(int argc,char ** argv){

	/*инициализация*/
	MPI::Init(argc,argv);
	
	std::cout <<"Hello, MPI!\n";

	/*Обязательно в конце*/
	MPI::Finalize();

	return 0;
}
