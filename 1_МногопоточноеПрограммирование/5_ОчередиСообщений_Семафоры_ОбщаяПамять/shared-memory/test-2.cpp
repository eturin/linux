#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
#include <iostream>

int main(){
	const char name[]="/test.shm";
	try{
		/*создаем виртуальный файл*/
		shm_unlink(name);
		int fd=shm_open(name,O_CREAT|O_RDWR|O_EXCL,0666);
		if(-1==fd) throw std::runtime_error("Не удалось создать виртуальный файл.");
			
		/*усекаем до нужного размера*/
		unsigned size=1024*1024;
		if(ftruncate(fd,size)){
			shm_unlink(name);
			throw std::runtime_error("Не удалось установить размер файла.");	
		}

		/*отображаем файл на память*/
		void * ptr_adr=mmap(nullptr,size,PROT_WRITE,MAP_SHARED,fd,0);
		if(MAP_FAILED==ptr_adr){
			shm_unlink(name);
			throw std::runtime_error("Не удалось отобразить файл на память.");
		}

		/*заполняем*/
		char * m=(char*)ptr_adr;
		for(unsigned i=0;i<size;++i)
			m[i]=13;

		/*отменяем отображение*/
		munmap(ptr_adr,size);

		/*виртуальный файл оставляем для чтения другими процессами*/
	}catch(std::runtime_error & e){
		std::cout << e.what();
		throw;
	}

	return 0;
}

