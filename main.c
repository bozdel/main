#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

#include "draw.h"
#include "snake.h"
#include "enums.h"

struct termios old_mode;


void clear_screen() {
	write(STDOUT_FILENO, "\e[2J", 4);
	write(STDOUT_FILENO, "\e[H", 3);
}

void set_new_terminal_mode() {
	tcgetattr(STDIN_FILENO, &old_mode); //STDIN_FILENO == fileno(stdin)

	setbuf(stdout, NULL);

	struct termios mode;
	mode = old_mode;

	mode.c_lflag &= ~(ICANON | ECHO); //turning off canonical mode and echo
	mode.c_cc[VMIN] = 0; //setting 0 to minimal input character amount
	mode.c_cc[VTIME] = 0; //setting time to 0 of waiting of putting next character


	tcsetattr(STDIN_FILENO, TCSANOW, &mode);

	clear_screen(); //for clearing any text in terminal
	// printf("\e[?25l"); //for hiding mouse
	write(STDOUT_FILENO, "\e[?25l", 6); //for hidding cursor
}

void set_old_terminal_mode() {
	tcsetattr(STDIN_FILENO, TCSANOW, &old_mode);
	write(STDOUT_FILENO, "\e[?25h", 6); //for showing cursor
}


typedef struct food_list {
	int x;
	int y;
	struct food_list *next;
} foodl;

typedef struct field {
	int size;
	int expected_food_amo;
	int cur_food_amo;
	foodl *food;
} field;



field* init_field(int size, int food_amo) {
	field *fld = (field*)malloc(sizeof(field));
	fld->size = size;
	fld->expected_food_amo = food_amo;
	fld->cur_food_amo = 0;
	fld->food = NULL;

	return fld;
}




enum cell_type get_cell_type(field *fld, snake *snk, int x, int y) {
	//checking wall
	if ((fld->size <= x || x <= 0) || (fld->size <= y || y <= 0)) {
		return WALL;
	}
	//checking snake
	for (body *seg = snk->head->next; seg != NULL; seg = seg->next) {
		if (x == seg->x && y == seg->y) {
			return SNAKE;
		}
	}
	//checking food
	for (foodl *food = fld->food; food != NULL; food = food->next) {
		if (x == food->x && y == food->y) {
			return FOOD;
		}
	}
	//each case was checked
	return EMPTY;
}



void spawn_food(field *fld, snake *snk) {
	if (fld->cur_food_amo >= fld->expected_food_amo) return;

	int x, y;
	do {
		x = rand() % fld->size;
		y = rand() % fld->size;
	} while (get_cell_type(fld, snk, x, y) != EMPTY);

	foodl *food = (foodl*)malloc(sizeof(foodl));
	food->x = x;
	food->y = y;
	food->next = fld->food;
	fld->food = food;
	pcolor(BLACK + rand() % 9);
	drawxy('$', x, y * 2);
	pcolor(DEFAULT);

	fld->cur_food_amo++;
}

void eat_food(field *fld, int x, int y) {
	for (foodl **food = &(fld->food); *food != NULL; ) {
		if ((*food)->x == x && (*food)->y == y) {
			foodl *tmp = *food;
			*food = (*food)->next;
			free(tmp);
		}
		else {
			food = &((*food)->next);
		}
	}
	fld->cur_food_amo--;
}


int control(snake *snk) {
	//try to add any useful action/function in cases
	char direction;
	char tmp;
	//skipping keys (reading last)
	while (read(STDIN_FILENO, &tmp, 1)) {
		direction = tmp;
	}
	switch (direction) {
		case 'a':
			if (snk->direction != RIGHT) {
				snk->direction = LEFT;
			}
			break;
		case 'd':
			if (snk->direction != LEFT) {
				snk->direction = RIGHT;
			}
			break;
		case 's':
			if (snk->direction != UP) {
				snk->direction = DOWN;
			}
			break;
		case 'w':
			if (snk->direction != DOWN) {
				snk->direction = UP;
			}
			break;
		case 'p':
			//pause func (in work)
			break;
		default:
			return 0;
	}
	return 1;
}



void grow_snake(snake *snk) {
	grow_head(snk);

	body *head = snk->head;
	body *old_head = snk->head->next;

	//redrawing old head and drawing new head, coloring
	pcolor(old_head->color);
	drawxy(old_head->body_char, old_head->x, old_head->y * 2);
	pcolor(head->color);
	drawxy(head->body_char, head->x, head->y * 2);
	pcolor(DEFAULT);
}

void move(snake *snk) {
	grow_head(snk);

	body *head = snk->head;
	body *old_head = snk->head->next;

	//redrawing old head and drawing new head, coloring
	pcolor(old_head->color);
	drawxy(old_head->body_char, old_head->x, old_head->y * 2);
	pcolor(head->color);
	drawxy(head->body_char, head->x, head->y * 2);
	pcolor(DEFAULT);

	//wiping tail
	drawxy(' ', snk->tail->x, snk->tail->y * 2);

	cut_tail(snk);
}



//return value <0 if game over, otherwise something else
int action(snake *snk, field *fld) {
	//getting coords of next cell
	int x, y;
	get_next_cell_coord(snk, &x, &y);

	char stat[40] = { 0 };
	snprintf(stat, 41, "snake head: (%d, %d)", x, y);
	clear_line(fld->size + 1);
	drawstrxy(stat, fld->size + 1, 0);

	switch (get_cell_type(fld, snk, x, y)) {
		case EMPTY:
			move(snk);
			return 1;
		case FOOD:
			grow_snake(snk);
			eat_food(fld, x, y);
			return 2;
		case SNAKE:
			return -1;
		case WALL:
			return -2;
		default:
			return -3;
	}
}

void end_game(int game_status) {
	//can be expansed for more complicated actions
	clear_screen();
	set_old_terminal_mode();
	switch (game_status) {
		case -1:
			printf("you ate yourself\n"
					"you lose\n");
			break;
		case -2:
			printf("you bumped on the wall\n"
					"you lose\n");
			break;
		case -3:
			printf("oops\n"
					"something wrong\n");
			break;
		default:
			printf("internal error\n");
			break;
	}
}

int main() {

	set_new_terminal_mode();

	atexit(set_old_terminal_mode);

	int field_size = 30;
	int food_amo = 15;
	field *fld = init_field(field_size, food_amo);
	snake *snk = init_snake(fld->size / 2, fld->size / 2, GREEN);

	draw_fld(field_size);

	int move = 0;
	int game_status = 1;

	//place for time control to add snake speedup

	while (game_status > 0) {
		usleep(250000);

		move = control(snk);
		
		if (1) { //for further adding snake speedup by holding key

			spawn_food(fld, snk);

			game_status = action(snk, fld);
		}
	}

	end_game(game_status);

	return 0;
}
