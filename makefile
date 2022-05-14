all: server client clean

server: server.o
	g++ -o server server.o -pthread 

client: client.o
	g++ -o client client.o

server.o: server.cpp Notification.cpp Packet.cpp
	g++ -c server.cpp Notification.cpp Packet.cpp

client.o: client.cpp Notification.cpp Packet.cpp
	g++ -c client.cpp Notification.cpp Packet.cpp

clean:
	rm -rf *.o