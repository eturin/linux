all: mutex-cpp11 mutex-cpp11-easy unique-lock recursive-mutex

mutex-cpp11:
	g++ -std=c++11 -o a mutex-cpp11.cpp -pthread

mutex-cpp11-easy:
	g++ -std=c++11 -o b mutex-cpp11-easy.cpp -pthread

unique-lock:
	g++ -std=c++11 -o c unique-lock.cpp -pthread

recursive-mutex:
	g++ -g -std=c++11 -o d recursive-mutex.cpp -pthread
