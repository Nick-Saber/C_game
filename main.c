#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <curses.h>
#include <time.h>
#include "Player.c"
#include "Enemy.c"
#include


int display_Grid(char ** grid);

int main () 
{
	//create window, s.t. key inputs are allowed but not displayed
	initscr();
	cbreak();
	noecho();
	curs_set(FALSE);
	keypad(stdscr,TRUE);
	halfdelay(5);

	//max x and y coordinates to deal with initial positioning
	int max_x=0;
	int max_y=0;
	getmaxyx(stdscr,max_y,max_x);




	//Initializing Player inside of the grid
	Player player_1;
	player_1.x_pos=max_x/2;
	player_1.y_pos=(max_y-1);
	player_1.character="^";

	//Initializing a single enemy inside of the grid
	Enemy enemy_1;
	enemy_1.x_pos=max_x/2;
	enemy_1.y_pos=0;
	enemy_1.character="V";

	//key variable stores current key pressed
	wchar_t key;

	//start game
	for(;;){
		//refresh max x and y to deal with screen resizing
		getmaxyx(stdscr,max_y,max_x);

		//update positioning of characters every 1/2 second
		if((key=getch()) != ERR)
		{
			switch(key)
			{
				case KEY_LEFT:
					if(player_1.x_pos > 1)
					{
						player_1.x_pos-=1;

					}
					break;
				case KEY_RIGHT:
					if(player_1.x_pos < (max_x -1))
					{
						player_1.x_pos+=1;
					}
					break;
			}
			switch(key)
			{

			}
		}

		//display updated positions
		clear();
		mvprintw(player_1.y_pos,player_1.x_pos,player_1.character);
		mvprintw(enemy_1.y_pos,enemy_1.x_pos,enemy_1.character);
		refresh();

	}
	return 0;
}

