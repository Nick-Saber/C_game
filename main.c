#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <curses.h>
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



//Main for loop for switching between main menu, and play mode
for(;;){
	switch(mode){
		case MENU:
			mode = main_menu();
			break;

		case PLAY:
			mode = playmode(1);
			break;
		}
	}
}

static int playmode(int level){
	//create window, s.t. key inputs are allowed but not displayed
	// also make variables to record stdscr max x and y, past and present
	//, these variables allow us to detect screen resizing and change display accordingly
	initscr();
	cbreak();
	noecho();
	curs_set(FALSE);
	int past_scr_x;
	int past_scr_y;
	getmaxyx(stdscr,past_scr_y,past_scr_x);
	int pres_scr_x=past_scr_x;
	int pres_scr_y=past_scr_y;

	int dsp_wndw_size = 1;
	//make upper play window where game takes place
	WINDOW * play_wndw=newwin(pres_scr_y-dsp_wndw_size,pres_scr_x, 0,0);
	keypad(play_wndw,TRUE);
	halfdelay(1);

	//make display window where things like ammo, level, and score are displayed
	WINDOW * dsp_wndw=newwin(dsp_wndw_size,pres_scr_x, pres_scr_y-dsp_wndw_size,0);
	int d_max_x;
	int d_max_y;
	getmaxyx(dsp_wndw,d_max_y,d_max_x);


	//max x and y coordinates to deal with initial positioning for playing window
	int p_max_x=0;
	int p_max_y=0;
	getmaxyx(play_wndw,p_max_y,p_max_x);


	//array of all players, first player is always the user
	int num_enemies=level;
	int num_friendlies=1;
	Player ** players[num_friendlies+num_enemies];


	//Initializing Player inside of the grid
	Player player_1;
	player_1.x_pos=p_max_x/2;
	player_1.y_pos=(p_max_y-1);
	player_1.character="^";
	player_1.friendly=TRUE;
	player_1.ammo_size=5;
	player_1.alive=TRUE;
	init_ammo(player_1.ammo_size,&player_1);

	players[0]=&player_1;

	//Initializing a single enemy inside of the grid
	for(int i =0; i<level;i++){
	Player * enemy = malloc(sizeof(Player));

	enemy->x_pos=i*(p_max_x/8);
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

	//FOR LOOP FOR PLAYING THE GAME
	for(;;){
		//check for resizing and if true update past and present dimension and resize play and display windows
		getmaxyx(stdscr,pres_scr_y,pres_scr_x);
		if((pres_scr_x != past_scr_x) || (pres_scr_y != past_scr_y)){
			past_scr_x=pres_scr_x;
			past_scr_y=pres_scr_y;	
			// wclear(play_wndw);
			// wclear(dsp_wndw);
			// wrefresh(play_wndw);
			// wrefresh(dsp_wndw);	
			delwin(play_wndw);
			delwin(dsp_wndw);
			play_wndw=newwin(pres_scr_y-dsp_wndw_size,pres_scr_x, 0,0);
			keypad(play_wndw,TRUE);
			dsp_wndw=newwin(dsp_wndw_size,pres_scr_x, pres_scr_y-dsp_wndw_size,0);	
		}



		//refresh max x and y to deal with screen resizing
		getmaxyx(play_wndw,p_max_y,p_max_x);
		getmaxyx(dsp_wndw,d_max_y,d_max_x);



		//UPDATE positioning of characters and bullets every 1/2 second
		if((key=wgetch(play_wndw)) != ERR)
		{
			switch(key)
			{
				case KEY_LEFT:
					if(player_1.x_pos > 0)
					{
						player_1.x_pos-=1;
					}
					break;
				case KEY_RIGHT:
					if(player_1.x_pos < (p_max_x))
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


		for(int j = 0; j<num_friendlies+num_enemies;j++){
			Player * plyr=players[j];
			if(plyr->friendly==TRUE){
				plyr->y_pos=p_max_y-1;
			} else{
				plyr->y_pos=0;
			}
		}
		//update bullets for all characters
		for(int j =0;j<num_friendlies+num_enemies;j++){
		update_bullets(players[j],p_max_y,p_max_x);
		}	

		//make dead is called before is_enemy_shot so that
		// when an enemy gets hit an X is displayed and
		//then on the next iteration of the for loop
		//the enemies alive member will be switched to false
		//this is so that display_players will display the X


		//This is used to test the first function within the state.c file,
		/*
		int avg_MHD_bullets;
		for (int i = 0; i<2;i++)
			{
				avg_MHD_bullets = compute_MHD_bullets(i, players, 2);
			}
		*/
	

		//this is also for testing state.c
		//mvprintw(max_y-3,5,"avg MHD %i", avg_MHD_bullets);

		//go through all enemies to check if they've already been shot if so make them dead
		make_dead(players+num_friendlies,num_enemies);


		//if enemies are shot 
		is_enemy_shot(players,num_enemies,num_friendlies);



		//DISPLAY updated positions of a players, enemies and bullets
		wclear(play_wndw);
		wclear(dsp_wndw);
		display_players(play_wndw, num_friendlies+num_enemies,players);
		display_ammo(dsp_wndw, players[0],d_max_y,d_max_x);
		wrefresh(play_wndw);
		wrefresh(dsp_wndw);

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
		mvwprintw(wndw,cursor*max_y/6,max_x/2 -3,">");
		wrefresh(wndw);
	}
}




