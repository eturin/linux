all: static dinamic with_lib

static: main.cpp hello.cpp hello.h
	g++ -std=c++11 -o main.o -c main.cpp 
	g++ -std=c++11 -o hello.o -c hello.cpp
	g++ -std=c++11 -o a main.o hello.o

dinamic: hello.cpp
	g++ -std=c++11 -o libHello.so -shared -fPIC hello.cpp

with_lib: dinamic main.cpp	#export LD_LIBRARY_PATH=.
	g++ -std=c++11 -L. -lHello -o b main.cpp

clean:
	-rm *.o a b libHello.so 	2>/dev/null
