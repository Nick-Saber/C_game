#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <curses.h>
#include "Bullet.c"
#include "Player.c"


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
	player_1.friendly=TRUE;
	init_ammo(5,&player_1);

	//Initializing a single enemy inside of the grid
	Player enemy_1;
	enemy_1.x_pos=max_x/2;
	enemy_1.y_pos=0;
	enemy_1.character="V";
	enemy_1.friendly=FALSE;
	init_ammo(5,&enemy_1);


	//key variable stores current key pressed
	wchar_t key;

	//start game
	for(;;){
		//refresh max x and y to deal with screen resizing
		getmaxyx(stdscr,max_y,max_x);


		//UPDATE positioning of characters and bullets every 1/2 second
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
				case KEY_UP:
					shoot(&player_1);
					break;
			}
		}
		update_bullets(&player_1,max_y,max_x);
		update_bullets(&enemy_1,max_y,max_x);




		//DISPLAY updated positions of a players, enemies and bullets
		clear();
		mvprintw(player_1.y_pos,player_1.x_pos,player_1.character);
		mvprintw(enemy_1.y_pos,enemy_1.x_pos,enemy_1.character);
		display_bullets(&player_1);
		display_bullets(&enemy_1);
		refresh();

	}
	return 0;
}

