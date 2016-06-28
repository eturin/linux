all: cond cond-2 barrier once test-all

cond:
	g++  -std=c++11 -o a1 cond.cpp -pthread

cond-2:
	g++  -std=c++11 -o a2 cond-2.cpp -pthread

barrier:
	g++ -std=c++11 -o b barrier.cpp -pthread

once:
	g++ -std=c++11 -o c once.cpp -pthread

test-all:
	g++ -std=c++11 -o t test-all.cpp -pthread
