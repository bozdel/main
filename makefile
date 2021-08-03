all: main.o snake.o draw.o
	gcc main.o snake.o draw.o -o snake
main_v2.o: main.c
	gcc -c main.c 
snake.o: snake.c
	gcc -c snake.c
draw.o: draw.c
	gcc -c draw.c
clean:
	rm *.o
