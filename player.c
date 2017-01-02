typedef struct Player {
	int x_pos;
	int y_pos;
	char * character;
	int ammo_size;
	Bullet ** ammo;
	bool friendly;
	bool alive;
	
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


//check through all shot bullets to see if the hit (are in the same position as) plyr argument
bool did_bullet_hit(Player * plyr, Player ** plyrs, int num_players){
	//iterate through all alive players
	int x_pos =plyr->x_pos;
	int y_pos = plyr->y_pos;
	for(int i = 0; i<num_players;i++){
	  Player * temp_plyr=plyrs[i];
	  //iterate through all bullets of a player
	  for(int j =0;j<temp_plyr->ammo_size;j++){
	  	Bullet * bullt = temp_plyr->ammo[j];
	  	if ((bullt->is_shot==TRUE) && (bullt->x_pos==x_pos) && (bullt->y_pos==y_pos))
	  	{
	  		//if bullet is shot and in position of character
	  		bullt->is_shot=FALSE;
	  		return TRUE;
	  	}
	  }
	}
	return FALSE;
}

//makes enemy dead after is shot
void make_dead(Player ** enemies, int num_enemies,int * num_dead_enemies){
	int counter=0;
	for(int i =0;i<num_enemies;i++){
		Player * enemy=enemies[i];
		if((strcmp(enemy->character,"X")==0) && (enemy->alive==TRUE)){
			enemy->alive=FALSE;
			counter++;
		}
	}
	(*num_dead_enemies)+=counter;
}


int is_enemy_shot(Player ** all_players, int num_enemies,int num_friendlies){
	int num_shot=0;

	for(int i = 0;i<num_enemies;i++){
		Player * enemy=all_players[i+num_friendlies];
		//If an alive enemy is shot change its character to X to represent dead
		if(enemy->alive==TRUE){
			if(did_bullet_hit(enemy,all_players,num_enemies+num_friendlies)==TRUE){
				num_shot++;
				enemy->character="X";
			}
		}
	}

	return num_shot;
}


//frees all allocated memory related to plyrs
void delete_players(Player ** plyrs, int size){
	for(int i =0;i<size;i++){
		Player * plyr= plyrs[i];
		Bullet ** ammo=plyr->ammo;
		for(int j=0;j<plyr->ammo_size;j++){
			Bullet * bullt = ammo[j];
			free(bullt);
		}
		free(ammo);
		free(plyr);
	}	
	free(plyrs);
}












