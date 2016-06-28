all: mutex spin-lock rw-lock test-all

mutex:
	g++ -std=c++11 -o a mutex.cpp -pthread

spin-lock:
	g++ -std=c++11 -o b spin-lock.cpp -pthread

rw-lock:
	g++ -std=c++11 -o c rw-lock.cpp -pthread

test-all:
	g++ -std=c++11 -o t test-all.cpp -pthread
