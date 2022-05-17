all: server client clean

server: server.o
	g++ -o server server.o -pthread 

client: client.o Notification.o Packet.o
	g++ -o client client.o Notification.o Packet.o

server.o: server.cpp Notification.cpp Packet.cpp
	g++ -c server.cpp Notification.cpp Packet.cpp

client.o: client.cpp Notification.cpp Packet.cpp
	g++ -c client.cpp Notification.cpp Packet.cpp

notification.o: Notification.cpp
	g++ -c Notification.cpp

packet.o: Packet.cpp
	g++ -c Packet.cpp

clean:
	rm -rf *.o