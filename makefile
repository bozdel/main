all : main.o snake.o
	gcc main.c snake.o -o snake
main.o : main.c
	gcc -c main.c
snake.o : snake.c
	gcc -c snake.c
clean :
	rm *.o
