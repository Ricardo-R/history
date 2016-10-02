bin : main.o binsearch.o
	gcc -o bin main.o binsearch.o
main.o : main.c bs.h
	gcc -c main.c
binsearch.o : binsearch.c bs.h
	gcc -c binsearch.c
clean : 
	rm -rf *.o
