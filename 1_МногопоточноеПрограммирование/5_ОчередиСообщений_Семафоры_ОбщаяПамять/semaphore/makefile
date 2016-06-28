all: a-sysV b-posix-named test-1 b-posix-noname test-2

a-sysV:
	g++ -std=c++11 -o a-sysV sem-sysV.cpp

test-1:
	g++ -std=c++11 -o t1 test-1.cpp

b-posix-named:
	g++ -std=c++11 -o b-posix sem-posix-named.cpp  -lrt -lpthread 

test-2:
	g++ -std=c++11 -o t2  test-2.cpp  -lrt -lpthread 

b-posix-noname:
	g++ -std=c++11 -o c-posix sem-posix-noname.cpp  -lrt -lpthread 
