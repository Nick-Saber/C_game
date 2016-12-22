#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>
#include "Player.c"
#include "Enemy.c"

#define FALSE 0
#define TRUE 1

#define ncols 50
#define nrows 25


int display_Grid(char ** grid);

int main () 
{



	//Initializing Player inside of the grid
	Player player_1;
	init_Player(&player_1);

	//Initializing a single enemy inside of the grid
	Enemy enemy_1;
	init_Enemy(&enemy_1);


	//create window, s.t. key inputs are allowed but not displayed
	initscr();
	cbreak();
	noecho();
	curs_set(FALSE);
	keypad(stdscr,TRUE);

	//max x and y coordinates to deal with resizing
	int max_x=0;
	int max_y=0;

	//start game
	for(;;){

		getmaxyx(stdscr,max_y,max_x);

		//refresh screen every 1/2 second
		if(((clock()/CLOCKS_PER_SEC)%1 == 0) || ((clock()/CLOCKS_PER_SEC)%1 == 0.5))
			{
			clear();
			mvprintw(max_y-1,max_x/2,"^");
			refresh();
			}

	}
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
