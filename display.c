
	//function that displays bullets of players to stdscr using ncurses library
	//for use in display_players
void display_bullets(Player * plyr){
	for(int i =0;i<plyr->ammo_size;i++)
		{
		Bullet * bullt= *(plyr->ammo + i);
		if(bullt->is_shot)
			{//display bullet if its shot
			mvprintw(bullt->y_pos,bullt->x_pos,bullt->character);
			}
		}
}

void display_players(int size, Player ** plyrs){

	for(int i =0; i<size;i++){
		Player * plyr= plyrs[i];
		//if the players alive display
		if(plyr->alive==TRUE){
			mvprintw(plyr->y_pos,plyr->x_pos,plyr->character);
			display_bullets(plyr);
		}
	}
}