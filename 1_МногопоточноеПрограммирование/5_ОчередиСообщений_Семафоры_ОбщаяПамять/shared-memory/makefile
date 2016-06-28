all: shared-memory-sysV test-1 shared-memory-posix shared-memory-posix-virtual-file test-2

shared-memory-sysV:
	g++ -std=c++11 -o a shared-memory-sysV.cpp

test-1:
	g++ -g -std=c++11 -o t1 test-1.cpp

shared-memory-posix:
	g++ -std=c++11 -o b shared-memory-posix.cpp

shared-memory-posix-virtual-file:
	g++ -std=c++11 -o c shared-memory-posix-virtual-file.cpp -lrt

test-2:
	g++ -std=c++11 -o t2 test-2.cpp -lrt
