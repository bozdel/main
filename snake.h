#ifndef MY_SNAKE_SNAKE_H
#define MY_SNAKE_SNAKE_H

typedef struct snake_body {
    int x;
    int y;
    char body_char;
    char *color;
    int color_num; // 6 in total
    struct snake_body *next;
    struct snake_body *prev;
} body;

typedef struct snake {
    body *head;
    body *tail;
    int direction;
} snake;

snake* init_snake(int x, int y);

#endif //MY_SNAKE_SNAKE_H