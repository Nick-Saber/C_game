#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <curses.h>
#include <math.h>
#include <stdio.h>
#include "bullet.c"
#include "player.c"
#include "display.c"
#include "state.c"
#include "score_io.c"

#define MENU 0
#define PLAY 1
#define INFO 2
#define SCORE 3
#define SAVE 4
#define NAME 5
#define TIMEOUT_DELAY 100

//different game mode functions that change display to terminal
static int playmode(int * level, int * score);
static int main_menu();
static int game_info(int level, int score);
static int save_mode(int * level, int * score);
static int show_scores();
static int enter_name(int * level, int * score);


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
			break;

		case SCORE:
			mode = show_scores();
			break;

		case SAVE:
			mode = save_mode(&level,&score);
			break;
		case NAME:
			mode = enter_name(&level,&score);
			break;

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
	curs_set(0);
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

		//Computes and performs the actions of all the enemies
		for (int i = 1; i < num_friendlies + num_enemies; i++)
		{
			perform_action(players[i], compute_action(players[i], p_max_y, p_max_x));
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
		*score+=10*is_enemy_shot(players,num_enemies,num_friendlies);

		//If main player is shot then go to the save screen
		if (did_bullet_hit(player_1,players,num_enemies+num_friendlies)) 
			{
				return SAVE;
			}


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
	curs_set(0);
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
							return SCORE;
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

	for(;;)
		{

			if((key=getch())!=ERR)
				{
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

static int save_mode(int * level, int * score) {
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
	

	FILE *score_file;

	wchar_t key;

	for (;;) 
		{
			getmaxyx(wndw,max_y,max_x);

			if((key=getch())!=ERR)
				{
					switch(key)
					{
						//ASCII code for y
						case 121:
							//If the person wants to save their score go to name entering mode
							return NAME;
						//ASCII code for n
						case 110:
							*level = 1;
							*score = 0;
							return MENU;

					}

				}

			wclear(wndw);
			mvwprintw(wndw, 2*max_y/6,max_x/3, "Congratzzz you made it to level %i",*level);
			mvwprintw(wndw, 3*max_y/6,max_x/3, "Your current score is %i", *score);
			mvwprintw(wndw, 4*max_y/6,max_x/5, "Would you like to record your current score and level? (y/n)" );
			wrefresh(wndw);

		}
}

static int enter_name(int * level, int * score){
	initscr();
	cbreak();
	noecho();
	curs_set(TRUE);
	WINDOW * wndw=stdscr;
	//allows non-blocking input for getting characters
	nodelay(wndw,TRUE);
	//doesn't move the cursors location on refreshes
	wchar_t key;
	int read_limit=40;
	char input_buffer[read_limit];
	int keys_read =0;


	//max x and y coordinates to deal with screen resizing
	int max_x=0;	
	int max_y=0;


	//get User name to record score
	for(;;){
	getmaxyx(wndw,max_y,max_x);

		if((key = wgetch(wndw)) != ERR)
		{		//10 is ASCII value for the enter key
				// since KEY_ENTER is only for nurmeric keypad enter in ncurses
			if((key == 10 )|| (keys_read ==read_limit-1))
			{
				input_buffer[keys_read]='\0';
				FILE * score_file = fopen("score_file.txt", "r");

				if(score_file==NULL)
					{
						score_file = fopen("score_file.txt","w+");
						fprintf(score_file, "%s Level:%i Score: %i\n",input_buffer, *level, *score);
					} 
				else
					{
						//read in line by line the scores file in order to edit the lines of the file
						int lines = number_of_lines(score_file);
						char score_file_lines[lines][400];
						for(int i = 0; i< lines;i++)
						{
							fgets(score_file_lines[i],400,score_file);
						}

						//order the lines by score while storing original ordering in the second index.
						int score_value[lines][2];
						for(int i =0; i <lines; i ++)
						{
							score_value[i][0]=get_score(score_file_lines[i]);
							score_value[i][1]=i;
						}
						qsort(&score_value,sizeof(int)*2,lines,integer_comparison);


						//write lines into score_file.txt, there are never more than 10 entries in score_file.txt
						//variable to keep track
						int lines_written = 0;
						//close file to end reading and reopen with "w" option to overwrite previous contents of the file
						fclose(score_file);
						score_file=fopen("score_file.txt","w");

						//set the position_of_new_entry to be the position of the lowest upper bound score
						//of all the scores in score_value
						int position_of_new_entry=0;
						for(int i =lines-1;i>=0;i--)
						{
							if(score_value[i][0]>= *score)
							{
								position_of_new_entry=i;
							}
						}


						//re-write all scores that are bigger than the newest score to score_file
						for(int i = lines-1;i>=position_of_new_entry && lines_written<10;i--)
						{
							//find the original character line to rewrite corresponding to the value of
							//score_value[i][0]

							for(int j = 0;j<lines;j++)
							{	
								if(score_value[i][0]==get_score(score_file_lines[j]) && score_value[i][1]==j)
								{
									fprintf(score_file, score_file_lines[j]);
									lines_written++;
									break;
								}
							}
						}
						//write the current score if there's space
						if(lines_written <10)
						{
							fprintf(score_file, "%s Level:%i Score:%i\n",input_buffer, *level, *score);
							lines_written++;
						}

						//write the rest of the scores lower than current score if they exist
						if(position_of_new_entry>0)
						{
							for(int i = position_of_new_entry-1;lines_written<10 && lines_written<lines+1;i--)
							{
								//find the original character line to rewrite corresponding to the value of
								//score_value[i][0]
								for(int j = 0;j<lines;j++)
								{
									if((score_value[i][0]==get_score(score_file_lines[j]) && score_value[i][1]==j))
									{
										fprintf(score_file, score_file_lines[j]);
										lines_written++;
										break;
									}
								}

							}
						} else {
							
						}


					}

			 	fclose(score_file);
				*level = 1;
				*score = 0;
				return MENU;
			} 
			else
			{
				input_buffer[keys_read]=key;
				keys_read++;
			}
		}

		wclear(wndw);

		mvwprintw(wndw,max_y/3,max_x/3,"ENTER YOUR NAME");
		wmove(wndw, (max_y/3 + 3),max_x/3);


		wrefresh(wndw);

	}


	//ERROR if for loop breaks
	return -1;

}





static int show_scores() {


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

	wclear(wndw);

		
	wchar_t key;



	//Loading score file information
	FILE * score_file;
	score_file = fopen("score_file.txt", "r");
	char buff[10][255];
	int lines; 
	bool exist_scores;
	if(score_file){
	exist_scores=TRUE;
	for(int i=1; fgets(buff[i-1],255,score_file)!=NULL;i++){
			lines=i;
		}
	}else{
		exist_scores=FALSE;
	}



	//Displaying scoreboard
	for(;;){

	getmaxyx(wndw,max_y,max_x);


		//breakpt
		if((key=getch())!=ERR)
			{
			switch(key)
				{
					//ASCII code for b
					case 98:
						return MENU;
					}
			}
	



	wclear(wndw);

	if(exist_scores){
		mvwprintw(wndw, 2*max_y/12,max_x/3, "Top 10 Scores:");
		//Printing last three scores onto scorboard
		for (int i =0;i<lines; i++) 
			{
				mvwprintw(wndw, 2*max_y/12 + (i+1),max_x/3, "%s", buff[i]);
			}
		mvwprintw(wndw, 2*max_y/12 + 15,max_x/4, "Press (b) to go back to the Main Menu");
	} else{
		mvwprintw(wndw,2*max_y/12,max_x/6, "No scores made yet, go out there and make some history¡");
		mvwprintw(wndw, 3*max_y/12 + 15,2*max_x/6, "Press (b) to go back to the Main Menu");


	}
	wrefresh(wndw);


	}
	return -1;
}

