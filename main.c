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
#define INFO 2
#define SCORE 3
#define TIMEOUT_DELAY 100

//different game mode functions that change display to terminal
static int playmode(int * level, int * score);
static int main_menu();
static int game_info(int level, int score);


int main () 
{

//initial mode is main menu
int mode=MENU;
int level = 1;
int score = 0;


//Main for loop for switching between main menu, and play mode
for(;;){
	switch(mode){
		case MENU:
			mode = main_menu();
			break;

		case PLAY:
			mode = playmode(&level,&score);
			break;

		case INFO:
			mode = game_info(level,score); 

		}
	}
	//ERROR
	return -1;
}

static int playmode(int * level,int * score){
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
	wtimeout(play_wndw,TIMEOUT_DELAY);

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
	int num_enemies=*level;
	int num_friendlies=1;
	Player ** players=malloc((num_friendlies +num_enemies)*sizeof(Player *));

	//variable used to track how many enemies have been killed on this level
	int num_dead_enemies=0;

	//Initializing Player inside of the grid
	Player * player_1 = malloc(sizeof(Player));
	player_1->x_pos=p_max_x/2;
	player_1->y_pos=(p_max_y-1);
	player_1->character="A";
	player_1->friendly=TRUE;
	player_1->ammo_size=20;
	player_1->alive=TRUE;
	init_ammo(player_1->ammo_size,player_1);

	players[0]=player_1;

	//Initializing enemies inside of the grid
	for(int i =0; i<*level;i++){
		Player * enemy = malloc(sizeof(Player));

		enemy->x_pos=i*(p_max_x/8);
		enemy->y_pos=floor(i/4);
		enemy->character="V";
		enemy->friendly=FALSE;
		enemy->ammo_size=1;
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
			wtimeout(play_wndw,TIMEOUT_DELAY);

			dsp_wndw=newwin(dsp_wndw_size,pres_scr_x, pres_scr_y-dsp_wndw_size,0);	
		}



		//refresh max x and y to deal with screen resizing
		getmaxyx(play_wndw,p_max_y,p_max_x);
		getmaxyx(dsp_wndw,d_max_y,d_max_x);



		// always make sure the player is at the bottom of the play window
		player_1->y_pos=p_max_y -1;


		//UPDATE positioning of characters and bullets every 1/2 second
		if((key=wgetch(play_wndw)) != ERR)
		{
			switch(key)
			{
				case KEY_LEFT:
					if(player_1->x_pos > 0)
					{
						player_1->x_pos-=1;
					}
					break;
				case KEY_RIGHT:
					if(player_1->x_pos < (p_max_x))
					{
						player_1->x_pos+=1;
					}
					break;
				case KEY_UP:
					shoot(player_1);
					break;
					//113 is ASCII value for key q
				case 113:
					return MENU;

			}
		}


		

		//Go through all the enemies, compute their next action and update their current positions
		for (int i = 1; i < num_enemies + num_friendlies; i++) 
			{
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
							if (players[i]->y_pos+1 > p_max_y/2)
								{
									i--;
									break;
								}
							players[i]->y_pos+=1;
							break;
						case 3:
							if (players[i]->x_pos+1 > p_max_x-2)
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

		//update bullets for all characters
		for(int j =0;j<num_friendlies+num_enemies;j++){
		update_bullets(players[j],p_max_y,p_max_x);
		}	

		//make dead is called before is_enemy_shot so that
		// when an enemy gets hit an X is displayed and
		//then on the next iteration of the for loop
		//the enemies alive member will be switched to false
		//this is so that display_players will display the X

		//go through all enemies to check if they've already been shot if so make them dead
		make_dead(players+num_friendlies,num_enemies,&num_dead_enemies);


		if(num_enemies==num_dead_enemies){
			(*level)+=1;
			delete_players(players, num_friendlies+num_enemies);
			return PLAY;
		}


		//if enemies are shot 
		*score+=100*is_enemy_shot(players,num_enemies,num_friendlies);





		//DISPLAY updated positions of a players, enemies and bullets
		wclear(play_wndw);
		wclear(dsp_wndw);
		display_players(play_wndw, num_friendlies+num_enemies,players);
		display_ammo(dsp_wndw, players[0],d_max_y,d_max_x);
		mvwprintw(dsp_wndw,0,0,"SCORE:%i",*score);
		wrefresh(play_wndw);
		wrefresh(dsp_wndw);

	}
	//ERROR
	return -1;
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
				//move the selector pointer
				case KEY_DOWN:
					if(cursor==5)
					{
						cursor=2;
					} else{
						cursor++;
					}
					break;
				case KEY_UP:
					if(cursor==2){
						cursor=5;
					} else{
						cursor--;
					}
					break;
				//10 is ASCII value for the enter key
				// since KEY_ENTER is only for nurmeric keypad enter in ncurses
				case 10:
					switch(cursor){
						case 2:
							return PLAY;
						case 3:
							return INFO;
						case 4:
							break;
							//return SCORES;
						case 5:
							wclear(wndw);
							delwin(wndw);
							endwin();
							exit(1);
					}

			}
		}

		wclear(wndw);
		mvwprintw(wndw,max_y/6,max_x/4, "GENERIC TERMINAL SHOOTING GAME TO WASTE ");
		mvwprintw(wndw,max_y/6+1,max_x/4, "TIME ON INSTEAD OF BEING PRODUCTIVE AND");
		mvwprintw(wndw,max_y/6+2,max_x/4, "GETTING ON WITH YOUR WORK");
		mvwprintw(wndw, 2*max_y/6,7*max_x/16, "PLAY" );
		mvwprintw(wndw, 3*max_y/6,7*max_x/16, "INFO" );
		mvwprintw(wndw,4*max_y/6,7*max_x/16, "SCORES");
		mvwprintw(wndw, 5*max_y/6,7*max_x/16, "QUIT" );		
		mvwprintw(wndw,cursor*max_y/6,7*max_x/16 -3,">");
		wrefresh(wndw);
	}
	//ERROR
	return -1;
}



static int game_info(int level,int score){

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

	wchar_t key;

	for(;;){

		if((key=getch())!=ERR){
			switch(key){
				//ASCII code for key b
				case 98:
					return MENU;

			}

		}

		wclear(wndw);
		mvwprintw(wndw, 2*max_y/6,max_x/3, "You are on level %i",level );
		mvwprintw(wndw, 3*max_y/6,max_x/3, "Your current score is %i",score );
		mvwprintw(wndw, 4*max_y/6,max_x/3, " Instructions:" );
		mvwprintw(wndw,4*max_y/6+1,max_x/3, "Press Left or Right arrow keys to move");
		mvwprintw(wndw,4*max_y/6+2,max_x/3, "Press Up arrow key to shoot");
		mvwprintw(wndw,4*max_y/6+3,max_x/3, "Press b to go back to main menu");
		wrefresh(wndw);

	}
	//ERROR
	return -1;
}

//Temporary function for moving enemies. This will be replaced later with functions from state.c
void move_enemy() {

}


