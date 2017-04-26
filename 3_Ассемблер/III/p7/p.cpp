#include <iostream>

template<typename T>
class Heap {
private:
	/*сравнение элементов */
	static bool cmp(const T * pM, size_t i, size_t j) {
		return pM[i - 1] < pM[j - 1]; /*мини-куча будет*/
	}
	/*перестановка элементов*/
	static void swap(T * pM, size_t i, size_t j) {		
		T t = pM[i - 1];
		pM[i - 1] = pM[j - 1];
		pM[j - 1] = t;
	}		
public:
	/*погружение*/
	static void shiftDn(T * pM, size_t i, size_t size) {
		while(true) {
			/*максимум на дочерних узлах (если есть)*/
			size_t j = i * 2;
			if(j + 1 <= size) {
				j = cmp(pM, j, j + 1) ? j : j + 1;
			} else if(j > size)
				j = i; /*нет дочерних узлов*/

			/*финанльное сравнение с родителем*/
			if(i == j)
				/*свойство кучи восстановлено*/
				break;
			else if((j = cmp(pM, i, j) ? i : j) != i) {
				/*меняем местами*/
				swap(pM, i, j);
				/*дальше повторим для изменившегося дочернего узла*/
				i = j;
			} else
				/*свойство кучи восстановлено*/
				break;
		}
	}
	/*построение кучи*/
	static void makeHeap(T * pM, size_t size) {
		for(size_t i = size / 2; i > 0; --i)
			shiftDn(pM, i, size);
	}	
};


int main() {
	/*забираем кол-во апроцессоров и задач*/
	unsigned n, m = 0;
	std::cin>> n >> m;

	/*строим массив загруженности процессоров (в младшей части номер процессора, а в старшей части время освобождения от последней задачи)*/
	unsigned long long * M = new unsigned long long[n];
	for(unsigned i = 0; i < n; ++i)
		M[i] = i; /*при такой инициализации мини-куча строится сама собой*/
	//makeHeap(M, n);

	/*забираем задачи и распределяем их по процессорам сразу*/
	unsigned t, k = 0;
	while(k<m && std::cin>>t) {		
		/*ообщаем номер процессора(младшая часть) и время начала обработки(старшая)*/
		std::cout << *(unsigned*)M  << ' ' << *((unsigned*)M + 1) << std::endl;
		/*сдвигаем время освобождения процессора на поздний срок*/
		*((unsigned*)M+1) += t;
		++k;
		/*восстанавливаем свойство кучи*/
		Heap<unsigned long long>::shiftDn(M, 1, n);	
	}

	/*освобождение ресурсов*/
	delete[] M;

	/*std::system("pause");*/
	return 0;
}