all: main.c utils.c transfer.c  control.c server.c ser_control.c unp.h
	gcc -o ftpclient main.c utils.c transfer.c control.c -pthread
	gcc -o ftpserver ser_control.c utils.c transfer.c server.c -pthread
