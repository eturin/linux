/* компиляция mpic++ -std=c++11 -o c mpi-3.cpp (получится программа с именем c)
 * запуск mpiexec -l -n 5 ./c  (будет 5 процессов)
 */
#include <iostream>
#include <cstring>
#include <mpi.h>

static const int LEN=20;
int main(int argc, char ** argv){
	char msg[LEN];
	int rank,size;

	/*запускаем все процессы сразу*/
	MPI::Init(argc,argv);

	/*запрашиваем свой ранк (номер соединения с коммуникатором по умолчанию)*/
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);//MPI_COMM_WORLD - коммуникатор по умолчанию (в которому прицеплен процесс сразу после запуска)
	/*запрашиваем сколько процессов связано с коммуникатором*/
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	/*пусть один процесс рассылает остальным сообщения через коммуникатор*/
	if(rank==0){
		std::strncpy(msg,"Превед, Пузану.",LEN);
		int len=std::strlen(msg), type=55;
		for(int i=1;i<size;++i){
			/* Отправляем через коммуникатор (MPI_COMM_WORLD - общий по умолчанию), 
			 * сообщение (msg), конкретной длинны (len) и типа (MPI_CHAR), 
			 * дополнительно указан произвольное числовое значение (type),
			 * !!!процессу с рангом i (себе нельзя)!!!*/
			std::cout<<"Процесс "<<rank<<" отправляет сообщение через коммуникатор "<<MPI_COMM_WORLD<<" процессу - "<<i<<std::endl;
			MPI_Send(msg,len,MPI_CHAR,i,type,MPI_COMM_WORLD);
		}
	}else{/*остальные процессы получают сообщение через коммуникатор*/
		int type=55;
		MPI_Status status;
		/* Пытаемся получить у конкретного коммуникатора (MPI_COMM_WORLD), 
		 * сообщение (msg), максимальной длинны (LEN),
		 * !!!от процесса с рангом 0!!!,
		 * !!!сообщение конкретного типа (MPI_CHAR)!!!,
		 * !!!и имеющее, установленное отправителем, конкретное числовое значение (type)*/
		int res=MPI_Recv(msg,LEN,MPI_CHAR,0,type, MPI_COMM_WORLD,&status);
		if(res==MPI_SUCCESS)
			std::cout<<"Процесс "<<rank<<" принял сообщение msg = "<<msg<<std::endl;
	}

	/*Завершаем все процессы*/
	MPI::Finalize();

	return 0;
}
