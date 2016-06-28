#include <iostream>
#include <memory>

class AA{
private:
	int a,b,c;
public:
	AA(int a=0, int b=0, int c=0):a(a),b(b),c(c){}
	~AA(){}
	friend std::ostream & operator<<(std::ostream & os, const AA & other) {
		return os << "("<<other.a<<","<<other.b<<","<<other.c<<")";
	}
}; 

int main(){
{
	//заполняем
	int n;
	std::cin >>n;
	std::unique_ptr<AA> * m=new std::unique_ptr<AA>[n];
	for(int i=0;i<n;++i){
		std::unique_ptr<AA> u_ptr(new AA(i,i+1,i+2));
		m[i]=std::move(u_ptr);
	}

	//печатаем
	for(int i=0;i<n;++i)
		std::cout<<*(m[i])<<std::endl;

	//удаляем
	for(int i=0;i<n;++i)
		m[i].reset();
	delete [] m;
}
	return 0;
}
