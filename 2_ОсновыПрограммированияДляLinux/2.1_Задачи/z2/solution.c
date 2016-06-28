#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv){
	if(argc<4){
		printf("Не хватает аргументов (должно быть не менее 3)");
		return 1; /*не достаточно аргументов*/
	}
	/*получаем имя библиотеки и добавляем к нему локальный путь*/
	const char * path_to_lib=*(++argv);
	/*получаем имя функции*/
	const char * func_name  =*(++argv);
	/*получаем аргумент*/
	int val=atoi(*(++argv));

	/*получаем хендл библиотеки*/
	void * hdl=dlopen(path_to_lib,RTLD_LAZY);
	if(hdl==NULL){
		printf("Библиотека не найдена");
		return 2; 
	}

	/*получаем указатель на функцию*/
	int (*f)(int)=(int (*)(int))dlsym(hdl,func_name);
	if(f==NULL){
		printf("Не найдена функция в библиотеке");
		return 3;
	}

	printf("%d\n",(*f)(val));
	dlclose(hdl);


	return 0;
}

