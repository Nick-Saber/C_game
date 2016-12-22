#include <stdio.h>


typedef struct Player {
	int x_pos;
} Player;

void init_Player(Player *player) {
	player->x_pos = 25;
}

void spawn_Player(Player *player, char **grid) {
	grid[24][player->x_pos] = 'M';
}

void move_Player(Player *player) {
	
}