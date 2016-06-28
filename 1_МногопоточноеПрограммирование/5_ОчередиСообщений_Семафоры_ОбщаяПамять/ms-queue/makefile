all: sysV posix test-1 test-1-help test-2 test-2-help

sysV:
	g++ -g -std=c++11 -o a ms-queue-sysV.cpp

posix:
	g++ -g -lrt -std=c++11 -o b ms-queue-posix.cpp

test-1:
	g++ -g -std=c++11 -o t1 test-1.cpp

test-1-help:
	g++ -g -std=c++11 -o h1 test-1-help.cpp


test-2:
	g++ -g -lrt -std=c++11 -o t2 test-2.cpp

test-2-help:
	g++ -g -lrt -std=c++11 -o h2 test-2-help.cpp
