all: server client clean

server: server.o
	g++ -o server server.o -pthread 

client: client.o
	g++ -o client client.o

server.o: server.cpp
	g++ -c server.cpp

client.o: client.cpp
	g++ -c client.cpp

clean:
	rm -rf *.o