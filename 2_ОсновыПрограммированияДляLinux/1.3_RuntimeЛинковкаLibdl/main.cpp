#include <dlfcn.h> 
#include <stdexcept>

int main(){
	/*получаем хендл библиотеки*/
	void * hdl=dlopen("./libHello.so",RTLD_LAZY);
	if(hdl==nullptr)
		throw std::runtime_error("Не удается загрузить библиотеку libHello.so из текущего каталога.");

	/*получаем указатель на нужную функцию по символам в библиотеке*/
	void (*f)(const char*);
	f=(void (*)(const char *))dlsym(hdl,"_Z5printPKc");
	char * err=dlerror();
	if(err!=nullptr)
		throw std::logic_error(err);

	/*вызываем функцию*/
	f("Hello, Пендальф!\n");

	/*закрываем библиотеку*/
	dlclose(hdl);

	return 0;
}
