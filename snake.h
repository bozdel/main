#ifndef SNAKE_H
#define SNAKE_H

#include "enums.h"

typedef struct snake_body {
	int x;
	int y;
	char body_char;
	enum color color; //it can be used to make non-monocolor snake (in work)
	struct snake_body *next;
	struct snake_body *prev;
} body;

typedef struct snake {
	body *head;
	body *tail;
	enum color color;
	int direction;
} snake;

snake* init_snake(int x, int y, enum color col);

void llink(body *new_seg, body *seg);

void grow_head(snake *snk);

void cut_tail(snake *snk);

int get_next_cell_coord(snake *snk, int *x, int *y);

#endif //SNAKE_H