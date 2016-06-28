all: server-sync server-async client-sync client-async

server-sync:
	g++ -g -std=c++11 -lboost_system -lboost_thread -o srv-sync serv-sync.cpp


server-async:
	g++ -g -std=c++11 -lboost_system -lboost_thread -o srv-async serv-async.cpp

client-sync:
	g++ -g -std=c++11 -lboost_system -o cln-sync client-sync.cpp

client-async:
	g++ -g -std=c++11 -lboost_system -o cln-async client-async.cpp
