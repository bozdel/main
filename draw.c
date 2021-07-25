#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "draw.h"
#include "enums.h"

int num_len(int x) {
	return 1 + (x >= 10) + (x >= 100);
}

void clear_line(int line_num) {
	//moving to line
	char seq[6];
	snprintf(seq, sizeof(seq), "\e[%dH", line_num + 1);
	write(STDOUT_FILENO, seq, 3 + num_len(line_num + 1));
	//clearing line
	write(STDOUT_FILENO, "\e[2K", 4);
}

void drawxy(char ch, int x, int y) {
	char seq[11];
	snprintf(seq, sizeof(seq), "\e[%d;%dH", x + 1, y + 1);
	write(STDOUT_FILENO, seq, 4 + num_len(x + 1) + num_len(y + 1));
	write(STDOUT_FILENO, &ch, 1);
}

void drawstrxy(const char *str, int x, int y) {
	char seq[11];
	snprintf(seq, sizeof(seq), "\e[%d;%dH", x + 1, y + 1);
	write(STDOUT_FILENO, seq, 4 + num_len(x + 1) + num_len(y + 1));
	write(STDOUT_FILENO, str, strlen(str));
}

void pcolor(enum color col) {
	char color_seq[6] = { 0 };
	snprintf(color_seq, sizeof(color_seq), "\e[%dm", col);
	write(STDOUT_FILENO, color_seq, 5);
}

void draw_fld(int size) {
	//drawing upper border
	drawstrxy("┌", 0, 0);
	for (int i = 1; i < size * 2 - 1; i++) {
		drawstrxy("─", 0, i);
	}
	drawstrxy("┐", 0, size * 2 - 1);

	//drawing field with side borders
	for (int i = 1; i < size; i++) {
		drawstrxy("│", i, 0);
		drawstrxy("│", i, size * 2 - 1);
	}

	//drawing lower border
	drawstrxy("└", size, 0);
	for (int i = 1; i < size * 2 - 1; i++) {
		drawstrxy("─", size, i);
	}
	drawstrxy("┘", size, size * 2 - 1);
}