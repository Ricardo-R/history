code : main.o stack.o
	gcc -o ps main.o stack.o
stack.o : func.h stack.c
	gcc -c stack.c
main.o : main.c func.h
	gcc -c main.c
clean : 
	rm -rf *.o
