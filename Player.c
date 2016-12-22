#include <stdio.h>


typedef struct Player {
	int x_pos;
	int y_pos;
	char character = '^';
} Player;

void move_Player(Player *player, int x, int y) {
	player->x_pos = x;
	player->y_pos = y;
}