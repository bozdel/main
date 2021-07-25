#ifndef DRAW_H
#define DRAW_H

#include "enums.h"

int num_len(int x);

void clear_line(int line_num);

void drawxy(char ch, int x, int y);

void drawstrxy(const char *str, int x, int y);

void pcolor(enum color col);

void draw_fld(int size);

#endif