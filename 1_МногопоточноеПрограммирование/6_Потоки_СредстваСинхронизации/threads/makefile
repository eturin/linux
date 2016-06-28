all: posix-thread-join posix-thread-detached posix-thread-stop-itself posix-thread-kill test

posix-thread-join:
	g++ -g -std=c++11 -o a posix-thread-join.cpp -pthread

posix-thread-detached:
	g++ -g -std=c++11 -o b posix-thread-detached.cpp -pthread

posix-thread-stop-itself:
	g++ -std=c++11 -o c posix-thread-stop-itself.cpp -pthread

posix-thread-kill:
	g++ -std=c++11 -o d posix-thread-kill.cpp -pthread

test:
	g++ -std=c++11 -o t test.cpp -pthread
