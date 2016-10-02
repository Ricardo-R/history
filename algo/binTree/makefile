bin : main.o cr.o
	gcc -o bin main.o cr.o
main.o : main.c bt.h
	gcc -c main.c
cr.o : createTree.c bt.h
	gcc -c createTree.c -o cr.o
clean : 
	rm -rf *.o
