#include "snake.h"
#include "enums.h"

#include <stdlib.h>

snake* init_snake(int x, int y) {
    //initiating body
    body *segment = (body*)malloc(sizeof(body));
    segment->body_char = '@';
    segment->color = (char*)malloc(sizeof(char) * 7);
    segment->color = "\e[0m";
    segment->color_num = 0;
    segment->next = NULL;
    segment->prev = NULL;
    segment->x = x;
    segment->y = y;

    //initiating snake
    snake *snk = (snake*)malloc(sizeof(snake));
    snk->head = segment;
    snk->tail = segment;
    snk->direction = RIGHT;

    return snk;
}