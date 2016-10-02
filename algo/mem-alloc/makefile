fit : main.o fit.o
	gcc -o fit main.o fit.o
main.o : main.c mem.h
	gcc -c main.c
fit.o : fit.c mem.h
	gcc -c fit.c
clean : 
	rm -rf *.o
