merge : main.o mergesort.o
	gcc -o merge main.o mergesort.o
main.o : main.c ms.h
	gcc -c main.c
mergesort.o : mergesort.c ms.h
	gcc -c mergesort.c
clean :
	rm -rf *.o
