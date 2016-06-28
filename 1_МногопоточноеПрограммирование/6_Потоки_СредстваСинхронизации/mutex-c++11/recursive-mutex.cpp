/* std::recursive_mutex в отличие от std::mutex можно блокировать много раз подряд (как вложенные транзакции)
 * бывает std::recursive_timed_mutex
*/
#include <thread>
#include <mutex>
#include <iostream>
#include <unistd.h>

/*это потокобезопасный класс (в нем используется mutex, когда что-то меняется)*/
template<typename T> class Mass_save{
private:
	enum {BUF=10};
	unsigned len,size;
	T * m;
	mutable std::recursive_mutex rmtx;
public:
	/*конструкторы экземпляра*/
	Mass_save():len(0),size(BUF),m(new T[size]){}
	Mass_save(T * m, unsigned len):len(len),size(len),m(new T[size]){
		/*блокировки не нужны, т.к. экземпляр еще не может быть использован в другом потоке*/
		for(unsigned i=0;i<len;++i)
			this->m[i]=m[i];
	}
	Mass_save(const Mass_save & other):len(other.len),size(other.size),m(new T[len]){
		/*блокировки не нужны, т.к. экземпляр еще не может быть использован в другом потоке*/
		for(unsigned i=0;i<len;++i)
			m[i]=other.m[i];
	}
	
	/*операция присваивания*/
	Mass_save & operator=(const Mass_save & other){
		if(this!=&other){
			/*ставим блокировку, т.к. возможно, одновременное использование*/
			rmtx.lock();
			delete [] m;
			len=other.len;
			size=other.size;
			m=new T[size];
			for(unsigned i=0;i<len;++i)
				m[i]=other.m[i];
			rmtx.unlock();			
		}
		return *this;
	}

	/*деструктор*/
	~Mass_save(){
		/*ставим блокировку, т.к. возможно, одновременное использование*/
		rmtx.lock();
		delete [] m;
		len=size=0;
		rmtx.unlock();
	}

	/*методы*/
	unsigned get_len() const {
		/*ставим блокировку, т.к. возможно, одновременное использование*/
		std::unique_lock<std::recursive_mutex>(rmtx);
		return len;
	}
	unsigned add(T & val){
		/*ставим блокировку, т.к. возможно, одновременное использование*/
		rmtx.lock();
		if(len+1>size){
			size+=BUF;
			T * new_m=new T[size];
			for(unsigned i=0;i<len;++i)
				new_m[i]=m[i];
			delete [] m;
			m=new_m;
		}
		m[len++]=val;
		rmtx.unlock();
		return len;
	}
	T operator[](unsigned i)const{
		/*ставим блокировку, т.к. возможно, одновременное использование*/
		std::unique_lock<std::recursive_mutex>(rmtx);
		return m[i];
	}
	friend std::ostream & operator<<(std::ostream & os,const Mass_save & other){
		/*ставим блокировку, т.к. возможно, одновременное использование*/
		std::unique_lock<std::recursive_mutex>(other.rmtx);
		for(unsigned i=0;i<other.len;++i)
			os<<other.m[i]<<" ";
		return os<<std::endl;
	}
};

/*экземпляр типа (class - не забывать)*/
//template class Mass_save<int>;

static const unsigned LEN=5;

int main(){
	/*это потокобезопасный ресурс*/
	Mass_save<unsigned> ms;

	/*делаем потоки*/
	std::thread * ath[LEN];
	for(unsigned i=0;i<LEN;++i)
		ath[i]=new std::thread([](Mass_save<unsigned> & ms,int i){
					for(unsigned j=0;j<i;++j)
						(sleep(1),ms.add(j));
					
					std::cout<<ms;
				},std::ref(ms),2*i);

	/*присоединяем потоки назад*/
	for(unsigned i=0;i<LEN;++i){
		ath[i]->join();
		delete ath[i];
	}
	return 0;
}
