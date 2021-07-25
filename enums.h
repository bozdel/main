#ifndef ENUMS_H
#define ENUMS_H

enum direction {
	UP, DOWN, LEFT, RIGHT
};

enum cell_type {
	WALL, SNAKE, FOOD, EMPTY
};

enum color {
	BLACK = 30,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE,
	DEFAULT = 39
};

#endif //ENUMS_H
