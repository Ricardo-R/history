all : server.c client.c unp.h
	gcc server.c -o server -pthread
	gcc client.c -o client
clean :
	rm -rf client server
