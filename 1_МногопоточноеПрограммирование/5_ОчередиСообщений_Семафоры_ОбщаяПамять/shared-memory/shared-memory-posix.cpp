#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#include <assert.h>

int main(int cntarg, char ** argval){
	const int LEN=20;

	/*формируем имя файла, который будем отображать в память*/
	unsigned len=std::strlen(argval[0]);
	char * path=new char[len+5];
	std::memcpy(path,argval[0],len);
	std::memcpy(path+len,".bin",5);
	
	if(fork())
	{//формирование общей памяти
		/*создаем/открываем этот файл, усекая до нулевой длинны*/
		int fd=open(path,O_CREAT|O_TRUNC|O_RDWR,0666);
		assert(-1!=fd);
		/*вытягиваем до нужного размера, сдвигая текущую позицию чтения/записи*/
		int res=lseek(fd,sizeof(int)*LEN-1,SEEK_SET);
		if(-1==res){
			close(fd);
			std::cout<<"Не удалось изменить размер файла";
			exit(-1);
		}else{
			/*требуется что-то записать в конец файла, чтобы зафиксировать его размер на диске*/
			res=write(fd,"",1);
			if(-1==res){
				std::cout<<"Не удалось зафиксировать размер файла на диске";
				close(fd);
				exit(-1);
			}
		}

		/*отображение подготовленного файла на память(данные с диска читаются в ленивом режиме)*/
		void * ptr_map=mmap(nullptr,sizeof(int)*LEN,PROT_READ|PROT_WRITE,MAP_SHARED,fd,(__OFF_T_TYPE)0);//MAP_SHARED - изменения будут видны другим процессам
		if(MAP_FAILED==ptr_map){
			close(fd);
			std::cout<<"Не удалось отобразить файл в память";
			exit(-1);
		}
		
		
		/*пишем в массив и это немедленно отображается в файл*/
		int * m=(int*)ptr_map;
		for(int i=0;i<LEN;++i){
			m[i]=255+i;
			std::cout<<i<<" <-- "<<m[i]<<std::endl;
		}

		/*отмена отображения*/	
		munmap(ptr_map,sizeof(int)*LEN);
		close(fd);	
	}
	else
	{//открытие общей памяти
		sleep(3);
		/*открываем файл на чтение*/
		int fd=open(path,O_RDONLY);
		assert(-1!=fd);
		/*получение сведений о файле (размер нужно узнать)*/
		struct stat buf;
		int res=fstat(fd,&buf);
		if(-1==res){
			close(fd);
			std::cout<<"Не удалось получить сведения о файле";
			exit(-1);
		}
		/*отображение файла в память*/
		void * ptr_map=mmap(nullptr,buf.st_size,PROT_READ,MAP_PRIVATE,fd,(off_t)0);
		if(MAP_FAILED==ptr_map){
			close(fd);
			std::cout<<"Не удалось отобразить файл на память (для чтения)";
			exit(0);
		}
		/*извлечение из общей памяти (чтение из файла)*/
		int * m=(int*)ptr_map;
		for(int i=0,l=buf.st_size/sizeof(int);i<l;++i)
			std::cout<<i<<" -> "<<m[i]<<std::endl;

		/*отключение от общей памяти*/
		munmap(ptr_map,buf.st_size);
		close(fd);

	}

	delete [] path;
	return 0;
}
