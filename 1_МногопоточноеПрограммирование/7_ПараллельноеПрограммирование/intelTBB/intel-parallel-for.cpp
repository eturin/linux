#include <iostream>

void foo(int x){
	std::cout << x<< std::endl;	
}

/*класс-функтор обрабатывающий массив параллельно*/
class Apply_foo{
	unsigned size;
	int * m;
public:
	void operator()(const tbb::blocked_range<unsigned> & r){
		for(unsigned i=r.begin(); i!=r.end();++i)
			foo(m[i]);
	}
	Apply_foo(int * m, unsigned size):size(size),m(new int[size]){
		for(unsigned i=0;i<size;++i)
			this->m[i]=m[i];
	}
	~Apply_foo(){delete [] m;}
}

int main(){
	int m[]={1,2,3,4,5,6,7,8,9};
	unsigned size=sizeof(m)/sizeof(int);

	/*последовательный цикл*/
	for(unsigned i=0;i<size;++i)
		foo(m[i]);
		
	/*параллельный цикл (функтор)*/
	tbb::parallel_for(tbb::blocked_range<unsigned>(0,size),Apply_foo(m,size));

	/*параллельный цикл (лямбда-функция)*/
	tbb::parallel_for(tbb::blocked_range<unsigned>(0,size),
			  [=](const tbb::blocked_range<unsigned> & r){
									for(unsigned i=r.begin(); i!=r.end();++i)
										foo(m[i]);
			 					      });

	/*параллельный цикл с разбиением на блоки (функтор)*/
	tbb::parallel_for(tbb::blocked_range<unsigned>(0,size,G),
			  Apply_foo(m,size),
			  simple_partitioner());

	return 0;
}
