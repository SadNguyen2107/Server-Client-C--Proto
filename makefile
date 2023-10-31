all:
	g++ -static-libgcc -static-libstdc++ -Wall -o server server.cpp -lws2_32
