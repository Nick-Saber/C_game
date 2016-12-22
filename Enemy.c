#include <stdio.h>

typedef struct Enemy {
	int x_pos;
	int y_pos;
	char character = '_';
} Enemy;

void Move_Enemy(Enemy *enemy, int x, int y) {
	enemy->x_pos = x;
	enemy->y_pos = y;
}