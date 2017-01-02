#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <curses.h>
#include <math.h>
#include <time.h>
#include "bullet.c"
#include "player.c"
#include "display.c"
#include "state.c"

#define MENU 0
#define PLAY 1

static int playmode(int level);
static int main_menu();


int main () 
{

//initial mode is main menu
int mode=MENU;

for(;;){
	switch(mode){
		case MENU:
			mode = main_menu();
			break;

		case PLAY:
			mode = playmode(3);
			break;
		}
	}
}

static int playmode(int level){
	//create window, s.t. key inputs are allowed but not displayed
	initscr();
	cbreak();
	noecho();
	curs_set(FALSE);
	WINDOW * wndw=stdscr;
	keypad(wndw,TRUE);
	halfdelay(1);

	//max x and y coordinates to deal with initial positioning
	int max_x=0;
	int max_y=0;
	getmaxyx(wndw,max_y,max_x);


	//array of all players, first player is always the user
	int num_enemies=level;
	int num_friendlies=1;
	Player * players[num_friendlies+num_enemies];


	//Initializing Player inside of the grid
	Player player_1;
	player_1.x_pos=max_x/2;
	player_1.y_pos=(max_y-2);
	player_1.character="^";
	player_1.friendly=TRUE;
	player_1.ammo_size=5;
	player_1.alive=TRUE;
	init_ammo(player_1.ammo_size,&player_1);

	players[0]=&player_1;

	//Initializing num_enemy enemies inside of the grid
	for(int i =0; i<level;i++){
		Player * enemy = malloc(sizeof(Player));

		enemy->x_pos=i*(max_x/8);
		enemy->y_pos=floor(i/4);
		enemy->character="V";
		enemy->friendly=FALSE;
		enemy->ammo_size=5;
		enemy->alive=TRUE;
		init_ammo(enemy->ammo_size,enemy);

		players[num_friendlies + i]=enemy;
	}


	//key variable stores current key pressed
	wchar_t key;


	Bullet * temp_bullet=malloc(sizeof(Bullet));
	temp_bullet->x_pos = max_x-1;
	temp_bullet->y_pos = max_y -1;
	temp_bullet->character= "f";
	temp_bullet->is_shot=TRUE;
	Player * temp_player = malloc(sizeof(Player));
	temp_player->ammo=&temp_bullet;
	temp_player->ammo_size=1;
	temp_player->friendly=TRUE;


	//start game
	for(;;){
		//refresh max x and y to deal with screen resizing
		getmaxyx(wndw,max_y,max_x);


		//UPDATE positioning of characters and bullets every 1/2 second
		if((key=wgetch(wndw)) != ERR)
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
					//113 is ASCII value for key q
				case 113:
					return MENU;

			}
		}
		//update bullets for all characters
		for(int j =0;j<num_friendlies+num_enemies;j++){
		update_bullets(players[j],max_y,max_x);
		}	

		//Go through all the enemies, compute their next action and update their current positions
		for (int i = 1; i < num_enemies + num_friendlies; i++) 
			{
				srand(time(NULL));
				int move = (rand() % 5) + 1;//compute_action(i, players, num_friendlies+num_enemies, max_y,max_x);
				switch(move) 
					{
						case 1:
							if (players[i]->y_pos-1 < 2)
								{
									i--;
									break;
								}
							players[i]->y_pos-=1;
							break;
						case 2:
							if (players[i]->y_pos+1 > max_y/2)
								{
									i--;
									break;
								}
							players[i]->y_pos+=1;
							break;
						case 3:
							if (players[i]->x_pos+1 > max_x-2)
								{
									i--;
									break;
								}
							players[i]->x_pos+=1;
							break;
						case 4:
							if (players[i]->x_pos-1 < 2)
								{
									i--;
									break;
								}
							players[i]->x_pos-=1;
							break;
						case 5:
							shoot(players[i]);
							break;
					}
			}


		//make dead is called before is_enemy_shot so that
		// when an enemy gets hit an X is displayed and
		//then on the next iteration of the for loop
		//the enemies alive member will be switched to false
		//this is so that display_players will display the X

		//go through all enemies to check if they've already been shot if so make them dead
		make_dead(players+num_friendlies,num_enemies);


		//if enemies are shot 
		is_enemy_shot(players,num_enemies,num_friendlies);



		//DISPLAY updated positions of a players, enemies and bullets
		wclear(wndw);
		display_players(wndw, num_friendlies+num_enemies,players);
		display_ammo(wndw, &player_1,max_y,max_x);
		wrefresh(wndw);

	}
	return 0;
}

static int main_menu(){
	//create window, s.t. key inputs are allowed but not displayed
	initscr();
	cbreak();
	noecho();
	curs_set(FALSE);
	WINDOW * wndw=stdscr;
	keypad(wndw,TRUE);
	halfdelay(1);

	//max x and y coordinates to deal with initial positioning
	int max_x=0;
	int max_y=0;
	getmaxyx(wndw,max_y,max_x);

	//cursor variable for selecting options
	int cursor = 2;
	wchar_t key;

	for(;;){
		//refresh max x and y to deal with screen resizing
		getmaxyx(wndw,max_y,max_x);


		//UPDATE positioning of characters and bullets every 1/2 second
		if((key=wgetch(wndw)) != ERR)
		{
			switch(key)
			{
				case KEY_DOWN:
					if(cursor<4)
					{
						cursor++;
					}
					break;
				case KEY_UP:
					if(cursor>2){
						cursor--;
					}
					break;
					//10 is ASCII value for the enter key, since KEY_ENTER is only for nurmeric keypad enter in ncurses
				case 10:
					if(cursor==2)
					{
						return PLAY;
					}
					if(cursor==4)
					{
						wclear(wndw);
						delwin(wndw);
						endwin();
						exit(1);
					}	
					break;


			}
		}

		wclear(wndw);
		mvwprintw(wndw, 2*max_y/6,max_x/2, "PLAY" );
		mvwprintw(wndw, 3*max_y/6,max_x/2, "INFO" );
		mvwprintw(wndw, 4*max_y/6,max_x/2, "QUIT" );
		mvwprintw(wndw,cursor*max_y/6,max_x/2 -3

			,">");
		wrefresh(wndw);
	}
}




