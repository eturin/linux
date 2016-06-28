#include <thread>
#include <iostream>

class myFunctor{
private:
	int res=0;

public:
	int get_res()const;

	/*в функторе определен оператор (), который станет вызываемой функцией*/
	void operator()(){
		std::cout<<"Вызван operator():\n\tres = "<<res<<" ==> res = ";
		res=2;
		std::cout<<res<<std::endl;
	}
};
int myFunctor::get_res() const{
	return res;
}

int main(){
	/*удобно хранить с потоком его данные, а потом их забирать, как результат работы*/
	myFunctor my_f;
	//my_f.res=1;          //загружаем данные в функтор
	std::thread th(std::ref(my_f));//запускаем их обработку в отдельном потоке(передаем по ссылке, иначе передается копированием объекта и обработанных данных мы не увидим)
	th.join();           //дожидаемся завершения обработки
	std::cout<< my_f.get_res()<<std::endl;

	return 0;
}
