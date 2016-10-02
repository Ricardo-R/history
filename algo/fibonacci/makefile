fibo : main.o fibo.o
	gcc -o fibo main.o fibo.o
main.o : main.c fib.h
	gcc -c main.c
fibo.o : fibo.c fib.h
	gcc -c fibo.c
clean : 
	rm -rf *.o
