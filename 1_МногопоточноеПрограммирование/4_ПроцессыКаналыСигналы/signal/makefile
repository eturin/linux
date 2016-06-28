all: ansi posix-ignor posix-hndl posix-real-time send wait wait-info

ansi:
	g++ -std=c++11 -o sgp-ansi signal_prog_ansi.cpp
posix-ignor:
	g++ -std=c++11 -o sgp-posix-ignor signal_prog_posix_ignor.cpp
posix-hndl:
	g++ -std=c++11 -o sgp-posix-hndl signal_prog_posix_hndl.cpp
posix-real-time:
	g++ -std=c++11 -o sgp-posix-real-time signal_prog_posix_real_time.cpp
send:
	g++ -std=c++11 -o sgp-send signal_prog_send.cpp

wait:
	g++ -std=c++11 -o sgp-wait signal_prog_wait.cpp
wait-info:
	g++ -std=c++11 -o sgp-wait-info signal_prog_wait_info.cpp
