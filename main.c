#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Player.c"
#include "Enemy.c"

#define ncols 50
#define nrows 25

int display_Grid(char ** grid);

int main () 
{

	//create 2d ncols x nrows grid array and fill it with empty space characters
	char ** grid = malloc(sizeof(char *)*nrows);

	for ( int i = 0; i<nrows; i++){
		grid[i]= (char*) malloc(ncols);
		memset(grid[i],' ',ncols);
	}

	//Initializing Player inside of the grid
	Player player_1;
	init_Player(&player_1);
	spawn_Player(&player_1, grid); 
	//Initializing a single enemy inside of the grid
	Enemy enemy_1;
	init_Enemy(&enemy_1);
	spawn_Enemy(&enemy_1, grid);

	display_Grid(grid);
	return 0;
}

int display_Grid(char ** grid) 
{
	for(int i = 0; i < nrows; i++)
	{	
		for(int j = 0; j < ncols; j++)
		{
			printf("%c",grid[i][j]);
		}
		printf("\n");
	}

	return 0;
}
