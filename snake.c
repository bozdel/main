#include "snake.h"
#include "enums.h"

#include <stdlib.h>

snake* init_snake(int x, int y, enum color col) {
	//initiating body
	body *segment = (body*)malloc(sizeof(body));
	segment->body_char = '@';
	segment->color = col;
	segment->next = NULL;
	segment->prev = NULL;
	segment->x = x;
	segment->y = y;

	//initiating snake
	snake *snk = (snake*)malloc(sizeof(snake));
	snk->head = segment;
	snk->tail = segment;
	snk->color = col;
	snk->direction = RIGHT;

	return snk;
}

void llink(body *new_seg, body *seg) {
	if (new_seg) new_seg->next = seg;
	if (seg) seg->prev = new_seg;
}

void grow_head(snake *snk) {
	//initing new head
	body *segment = (body*)malloc(sizeof(body));
	segment->body_char = '@';
	segment->color = snk->color;
	// segment->color = BLACK + (1 + snk->head->color - BLACK) % 9;  //rainbow snake
	segment->next = NULL;
	segment->prev = NULL;
	get_next_cell_coord(snk, &(segment->x), &(segment->y));
	//fixing old head
	snk->head->body_char = '#';

	//appending
	llink(segment, snk->head);

	snk->head = segment;
}

void cut_tail(snake *snk) {
	body *tmp = snk->tail;

	llink(snk->tail->prev, NULL);

	snk->tail = snk->tail->prev;

	free(tmp);
}

int get_next_cell_coord(snake *snk, int *x, int *y) {
	*x = snk->head->x;
	*y = snk->head->y;
	switch (snk->direction) {
		case LEFT:
			(*y)--;
			break;
		case RIGHT:
			(*y)++;
			break;
		case UP:
			(*x)--;
			break;
		case DOWN:
			(*x)++;
			break;
	}
}