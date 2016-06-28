#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <assert.h>

int main(){
	const char name[]="/хрень любая";
	/*удаляемi виртуальный файл для общей памяти*/
	shm_unlink(name);
	/*созлаем виртуальный файл для общей памяти*/
	int fd=shm_open(name,O_CREAT|O_EXCL|O_RDWR,0666);
	assert(-1!=fd);
	/*обрезаем виртуальный файл до нужного размера (в байтах)*/
	int size=10*sizeof(int);
	int res=ftruncate(fd,size);
	if(res==-1){
		shm_unlink(name);
		std::cout<<"Не удалось установить размер виртуального файла.";
		exit(-1);
	}
	if(fork()){
		/*отображение виртуального файла (можно отобразить фрагмент файла, а последний параметр смещение, от которого меряется длина)*/
		void * ptr_map=mmap(nullptr,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,(__OFF_T_TYPE)0);//MAP_SHARED - изменения будут видны другим процессам
		if(MAP_FAILED==ptr_map){
			shm_unlink(name);
			std::cout<<"Не удалось отобразить файл в память";
			exit(-1);
		}
						
		/*пишем в массив и это немедленно отображается в файл*/
		int * m=(int*)ptr_map;
		for(int i=0,l=size/sizeof(int);i<l;++i){
			m[i]=255+i;
			std::cout<<i<<" <-- "<<m[i]<<std::endl;
		}

		/*отмена отображения(можно освободить фрагмент)*/	
		munmap(ptr_map,size);
	}else{
		sleep(3);
		/*получение сведений о файле (размер нужно узнать)*/
		struct stat buf;
		int res=fstat(fd,&buf);
		if(-1==res){
			shm_unlink(name);
			std::cout<<"Не удалось получить сведения о файле";
			exit(-1);
		}
		/*отображение файла в память*/
		void * ptr_map=mmap(nullptr,buf.st_size,PROT_READ,MAP_PRIVATE,fd,(off_t)0);
		if(MAP_FAILED==ptr_map){
			shm_unlink(name);
			std::cout<<"Не удалось отобразить файл на память (для чтения)";
			exit(0);
		}
		/*извлечение из общей памяти (чтение из файла)*/
		int * m=(int*)ptr_map;
		for(int i=0,l=buf.st_size/sizeof(int);i<l;++i)
			std::cout<<i<<" -> "<<m[i]<<std::endl;
		/*отключение от общей памяти*/
		munmap(ptr_map,buf.st_size);
	}

	/*удаляем виртуальный файл для общей памяти*/
	shm_unlink(name);


	return 0;
}
