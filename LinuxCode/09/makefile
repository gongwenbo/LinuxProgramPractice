all:client server

client:client.o process.o
	gcc -o client client.o process.o
server:server.o process.o
	gcc	-o server server.o process.o
clean:
	rm -f *.o
