#include <stdio.h>
#include <iostream>
#include <errno.h>
#include <unistd.h>

int main(){
	FILE * in=popen("ls -l","r");
	if(in){
		const int LEN=10;
		char buf[LEN];
		while(fgets(buf,LEN,in)!=NULL){
			std::cout <<buf;
		}
		pclose(in);//закрываем канал и это считывает код возврата потомка (уже хранящего только этот код)
		std::cout << std::endl;
	}else
		std::cout << errno<<std::endl;

	return 0;
}
