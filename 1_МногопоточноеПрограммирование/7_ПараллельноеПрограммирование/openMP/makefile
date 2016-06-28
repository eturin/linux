all: openMP-1 openMP-2 openMP-3 openMP-4 openMP-5 openMP-6 openMP-7 openMP-8 openMP-9 openMP-10 openMP-11 openMP-12 openMP-13 test-1 

openMP-1:
	g++ -std=c++11 -o a openMP-parallel.cpp -fopenmp
openMP-2:
	g++ -std=c++11 -o b openMP-parallel-2.cpp -fopenmp
openMP-3:
	g++ -std=c++11 -o c openMP-shared-private.cpp -fopenmp
openMP-4:
	g++ -std=c++11 -o d openMP-for.cpp -fopenmp
openMP-5:
	g++ -std=c++11 -o e openMP-reduction.cpp -fopenmp
openMP-6:
	g++ -std=c++11 -o f openMP-single-barrier.cpp -fopenmp
openMP-7:
	g++ -std=c++11 -o i openMP-ordered.cpp -fopenmp
openMP-8:
	g++ -std=c++11 -o j openMP-critical.cpp -fopenmp
openMP-9:
	g++ -std=c++11 -o k openMP-sections.cpp -fopenmp
openMP-10:
	g++ -std=c++11 -o l openMP-atomic.cpp -fopenmp
openMP-11:
	g++ -std=c++11 -o m openMP-task.cpp -fopenmp
openMP-12:
	g++ -std=c++11 -o n openMP-lock.cpp -fopenmp
openMP-13:
	g++ -std=c++11 -o o openMP-nested.cpp -fopenmp
test-1:
	g++ -g -std=c++11 -o t test-1.cpp -fopenmp
