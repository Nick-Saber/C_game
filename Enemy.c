#include <stdio.h>

typedef struct Enemy {
	int x_pos;
	int y_pos;
} Enemy;

void init_Enemy(Enemy *enemy) {
	enemy->x_pos = 25;
	enemy->y_pos = 1;
}

void spawn_Enemy(Enemy *enemy, char **grid ) {
	grid[enemy->y_pos][enemy->x_pos] =  '_';
}

int Move_Enemy() {
	return 0;
}