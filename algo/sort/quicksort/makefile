qsort : main.o quicksort.o
	gcc -o qsort main.o quicksort.o
main.o : main.c qs.h
	gcc -c main.c
quicksort.o : quicksort.c qs.h
	gcc -c quicksort.c
clean :
	rm -rf *.o
