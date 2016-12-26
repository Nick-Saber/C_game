
typedef struct Bullet {
	int x_pos;
	int y_pos;
	char * character;
	bool is_shot;
} Bullet;

//updates bullet positions for player struct
bool update_Bullets(Player * plyr){
	//boolean value returned at end, TRUE if one bullet is freed, FALSE otherwise
	bool freed = FALSE;

	for(int i =0; i<plyr->ammo_size;i++)
		{
		//get i'th bullet pointer in ammo array
		Bullet * bullt = (plyr->ammo + i*sizeof(Bullet *));

		//update procedure for friendly player bullets
		if(plyr->friendly)
			{
			//check if bullet has reached end of screen for friendly players
			if(bullt->y_pos == 0)
				{
				bullt->is_shot= FALSE;
				freed=TRUE;
				} 
			else//update bullet position otherwise
				{ 
				bullt->y_pos++;
				}
			}

		//update procedure for enemy player bullets	
		if(!(plyr->friendly))
			{
		//check if bullet has reached end of screen for enemies
			if(bullt->y_pos == max_y-1)
				{
				bullt->is_shot= FALSE;
				freed=TRUE;
				} 
			else
				{ //update bullet position otherwise
				bullt->y_pos--;
				}
			}
		}
	return freed;
	}
