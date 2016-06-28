#include <iostream>
#include <omp.h>
#include <fstream>
#include <assert.h>
#include <cstring>

template<typename T> class MassMP{
private:
	enum {BUF = 100, MULT = 2, NUM_THREAD = 7};
	unsigned size,len;
	T * m;
public:
	/*Конструктор экземпляра*/
	MassMP():size(BUF),len(0),m(new T[size]){}
	MassMP(const MassMP & other):size(other.len),len(size),m(new T[size]){
		#pragma omp parallel for num_threads(NUM_THREAD)
		for(unsigned i=0;i<len;++i)
			m[i]=other.m[i];
	}
	MassMP & operator=(const MassMP & other){
		if(this!=&other){
			delete [] m;
			size=len=other.len;
			m=new T[size];
			#pragma omp parallel for num_threads(NUM_THREAD)
			for(unsigned i=0;i<len;++i)
				m[i]=other.m[i];
		}
		return *this;
	}
	/*деструктор*/
	~MassMP(){delete [] m;}
	/*методы*/
	void add(T & val){ 
		if(len+1==size){
			/*вытягиваем внутренний массив*/
			size*=MULT;
			T * new_m=new T[size];
			#pragma omp parallel for num_threads(NUM_THREAD)
			for(unsigned i=0;i<len;++i)
				new_m[i]=m[i];
			delete [] m;
			m=new_m;
		}
		m[len++]=val;
	}
	friend std::ostream & operator<<(std::ostream & os, const MassMP & other){
		for(unsigned i=0;i<other.len;++i)
			os<<other.m[i]<<" ";
		return os;
	}
	void sort() {
		if(len > 1) {
			unsigned cnt = 1, l = (len + 1) / (2*cnt);
			bool is_ok = true;
			/*параллельная секция*/
			#pragma omp parallel num_threads(NUM_THREAD) shared(cnt,l,is_ok)
			{
				/*переменная будет у каждого потока своя*/
				bool is_mult = false;
				/*только один поток будет иметь значение отличное от остальных*/
				#pragma omp single
				is_mult = true;
				do {
					/*цикл разделится между потоками (в конце нет барьера)*/
					#pragma omp for 
					for(unsigned i = 0; i < l; ++i) {
						std::cout<<"i = "<<i<<"<- (cnt = "<<cnt<<") "<<omp_get_thread_num()<<std::endl;
						unsigned len_frag = ((i + 1) * 2 * cnt <= len ? 2 * cnt : len - i * 2 * cnt),/*длина обрабатываемого за раз фрагмента массива */
 							a = i * 2 * cnt,                           		   /*начало первой части*/
							b = a + cnt,                            		   /*начало второй части*/
							a_end = a + cnt, 				           /*граница первой части*/
							b_end = b + len_frag - cnt;                                /*граница второй части*/
						/*слияние двух частей*/
						T * new_m = new T[len_frag];
						for(unsigned j = 0; j < len_frag; ++j)
							if(b == b_end)
								new_m[j] = m[a++];
							else if(a == a_end)
								new_m[j] = m[b++];
							else if(m[a] < m[b])
								new_m[j] = m[a++];
							else
								new_m[j] = m[b++];
						/*копирование отсортированной части в обрабатываемый фрагмент массива*/
						std::memcpy(m + i * 2 * cnt, new_m, len_frag*sizeof(T));
						delete[] new_m;
					}
					/*только один поток меняет длинну обрабатываемого массива*/
					if(is_mult) {
						cnt *= 2;
						l = (len + 1) / (2*cnt);
						l = l == 0 ? 1 : l;
						if(cnt >= len) is_ok = false;
						std::cout << "Ключевые значения "<<omp_get_thread_num()<<std::endl;
					}

					std::cout<<"барьер<- "<<omp_get_thread_num()<<std::endl;
					/*остальные потоки дожидаются новых условий выполнения цикла сортировки*/
					#pragma omp barrier
					/*!!!!обязательно в каждом потоке обновляем значений из общей памяти!!!!*/
					#pragma omp flush(cnt)
					#pragma omp flush(l)
					#pragma omp flush(is_ok)
					std::cout<<"--> "<<omp_get_thread_num()<<std::endl;
				} while(is_ok);
			}
		}
	}
};

int main(){
	/*создаем экземпляр */
	MassMP<int> m;
	/*заполняем массив*/
	int x;
	while(std::cin >> x)
		m.add(x);
	/*сортируем массив*/
	m.sort();
	/*выводим массив*/
	std::ofstream fout("result.txt");
	assert(fout.is_open());
	fout<<m;
	fout.close();

	return 0;
}

