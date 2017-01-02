
	//function that displays bullets of players to stdscr using ncurses library
	//for use in display_players
void display_bullets(WINDOW * wndw, Player * plyr){
	for(int i =0;i<plyr->ammo_size;i++)
		{
		Bullet * bullt= *(plyr->ammo + i);
		if(bullt->is_shot)
			{//display bullet if its shot
			mvwprintw(wndw, bullt->y_pos,bullt->x_pos,bullt->character);
			}
		}
}

//Displays all alive players and bullets they've fired to the wndw
void display_players(WINDOW * wndw, int size, Player ** plyrs){

	for(int i =0; i<size;i++){
		Player * plyr= plyrs[i];
		//if the players alive display
		if(plyr->alive==TRUE){
			mvwprintw(wndw, plyr->y_pos,plyr->x_pos,plyr->character);
			display_bullets(wndw, plyr);
		}
	}
}


//function that displays ammo at the bottom right of the screen
void display_ammo(WINDOW * wndw, Player *playr, int max_y, int max_x){
	int temp = 0;

	//iterating through the bullets of the player and finding how many are not shot yet
	for(int i = 0; i<playr->ammo_size;i++) 
		{
		Bullet * bullt = *(playr->ammo+i);
		if(bullt->is_shot == FALSE)
			{
				temp++;
			}
		} 
	//printing out the numbers of bullets at the bottom right of the screen
	for(int i = 0; i<temp;i++)
		{
			mvwprintw(wndw, max_y-1,max_x-1-i,"|");
		}
}