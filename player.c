typedef struct Player {
	int x_pos;
	int y_pos;
	char * character;
	int ammo_size;
	Bullet ** ammo;
	bool friendly;


} Player;

void init_ammo(int size, Player * plyr){
	Bullet ** bullets = malloc(size*sizeof(Bullet *));
	for(int i =0; i<size;i++){
		Bullet * temp = malloc(sizeof(Bullet));
		temp->is_shot=FALSE;
		temp->character="|";
		bullets[i] = temp;
	}
	plyr->ammo=bullets;
}

	//function that displays bullets of players to stdscr using ncurses library
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


// This function handles the shooting of ammo 
bool shoot(Player *player) {

	// Iterate through ammo array and find the first free bullet. 
	for(int i = 0; i < player->ammo_size; i++) 
	{
		Bullet * bullet = *(player->ammo + i);

		// If the bullet is not shot yet then update its position accordingly and shoot the bullet.
		if (bullet->is_shot == FALSE) 
			{
			bullet->x_pos = player->x_pos;
			bullet->y_pos = player->y_pos;
			bullet->is_shot = TRUE;
			return TRUE;
			}
	}

	// If there are no bullets in the ammo then return false
	return FALSE;
}



//updates bullet positions for player struct
bool update_bullets(Player * plyr, int max_y,int max_x){
	//boolean value returned at end, TRUE if one bullet is freed, FALSE otherwise
	bool freed = FALSE;
	for(int i =0; i<plyr->ammo_size;i++)
		{
		//get i'th bullet pointer in ammo array
		Bullet * bullt = *(plyr->ammo + i);

		if(bullt->is_shot==TRUE)
			{//update only if the bullet is shot
			//update procedure for friendly player bullets
			if(plyr->friendly==TRUE)
				{
				//check if bullet has reached end of screen or outside of screen for friendly players
				if(bullt->y_pos == 0 )
					{
					bullt->is_shot= FALSE;
					freed=TRUE;
					} 
				else//update bullet position otherwise
					{ 
					bullt->y_pos--;
					}
				}

			//update procedure for enemy player bullets	
			if(plyr->friendly==FALSE)
				{
			//check if bullet has reached end of screen for enemies
				if(bullt->y_pos >= max_y-1)
					{
					bullt->is_shot= FALSE;
					freed=TRUE;
					} 
				else
					{ //update bullet position otherwise
					bullt->y_pos++;
					}
				}
			}

		}
	return freed;
}
