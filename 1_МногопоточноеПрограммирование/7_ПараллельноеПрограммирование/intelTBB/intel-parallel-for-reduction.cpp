
class CSum{
private:
	unsigned size;
	int * m;
public:
	int sum;
	/*конструктор экземпляры (вызываем мы)*/
	CSum(const int * m,unsigned size):size(size),m(new int[size]),sum(0){
		for(unsigned i;i<size;++i)
			this->m[i]=m[i];
	}
	/*выполнит разделение исходного экземпляра на одинаковые копии*/
	CSum(const CSum & other, split):size(other.size),m(new int[size]),sum(0){
		for(unsigned i;i<size;++i)
			this->m[i]=m[i];
	}
	/*выполнит суммирование в каждой копии*/
	void operator(const tbb::blocked_range<unsigned> & r){
	    	for(unsigned i=r.begin();i!=r.end();++i)
			sum+=m[i];	
	}
	/*объединение результатов*/
	void join(const CSum & other){
		sum+=other.sum;
	}
	~CSum(){delete [] m;}
};

int main(){
	int m[]={1,2,3,4,5,6,7};
	unsigned size=sizeof(m)/sizeof(int);
	int sum=0;

	/*последовательное суммирование*/
	for(unsigned i=0;i<size;++i)
		sum+=m[i];

	/*параллельное суммирование (функтор)*/
	CSum cs(m,size);
	tbb::parallel_reduce(tbb::blocked_range<unsigned>(0.size),cs);
	sum=cs.sum;

	/*tbb::parallel_reduce(tbb::blocked_range<unsigned>(0.size),
			     [=](const tbb::blocked_range<unsigned> & r){
			    	for(unsigned i=r.begin();i!=r.end();++i)
					sum+=m[i];	
			     });*/

	return 0;
}
