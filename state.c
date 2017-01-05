//******************FUNCTIONALITY NOT IMPLEMENTED THUS FAR*******************//


//Struct used for checking the state of the current game for a specific player or enemy

//MHD = Manhattan distance
/*struct State {
>>>>>>> b2a3ae5a5dd7b3eebcf045349feadf3026249e3d
	int min_MHD_bullet;
	int min_MHD_player;

}State;

//Computes and returns the minimum MHD from the current player to all the shot bullets on the map
int compute_MHD_bullets(int playr_index, Player ** playrs, int no_of_players, int y_pos, int x_pos) {
	//Store the current player that you are computing MHDs for
	Player * current_playr = *(playrs + playr_index);

	//Temporary integer for storing the minimum MHD to another bullet
	int min_MHD = 0;

	//Iterate through finding all of the bullets in the current game
	for (int i = 0; i<no_of_players; i++) 
		{
			//Store the player that you are finding MHDs from its bullets
			Player * to_check = *(playrs + i);

			//Skip the player you are checking the MHDs for
			//and skip the friendly players
			if(i == playr_index) 
				{
					continue;
				}
			//Find the MHD to the bullets from the current player
			else 
				{	
					//Iterate through the bullets of to_check and if they are shot find the 
					//MHD from them to the current player and if it is smaller than the previous MHD then store it in min_MHD
					for(int j = 0; j<to_check->ammo_size; j++) 
						{
							Bullet * bullt = to_check->ammo[j];
							if (bullt->is_shot) 
								{
									//computing MHD
									int MHD = abs(x_pos - bullt->x_pos) + abs(y_pos - bullt->y_pos);
									//If this MHD is smaller than the previous then store it in the min_MHD variable
									if (min_MHD == 0 || MHD < min_MHD) 
										{
											min_MHD = MHD;
										}
								}
						}
				}
		}
	return min_MHD;
}

//Computes and returns the minimum manhatton distance from the current player to all the other players on the map
int compute_MHD_players(int playr_index, Player ** playrs, int no_of_players, int y_pos, int x_pos) {
	//stores the current player that you are computing MHDs for
	Player * current_playr = *(playrs + playr_index);

	//Variable to store min_MHD
	int min_MHD = 0;

	//Iterate through players and find the manhatton distances from current_playr
	for (int i = 0; i < no_of_players; i++) 
		{
			//Store the player that you are checking the MHD from
			Player * to_check = *(playrs+i);

			//If the to_check player is the same as the current_playr then increment i
			if (i == playr_index || current_playr->friendly) 
				{
					continue;
				}
			//Otherwise find the MHD from the current player to the other players.
			else
				{
					int MHD = abs(y_pos - to_check->y_pos) + abs(x_pos - to_check->x_pos);
					if (min_MHD == 0 || MHD < min_MHD) 
						{
							min_MHD = MHD;
						}
				}
		}
	if (min_MHD == 0) 
		{
			return 10;
		}
	return min_MHD;
}

void compute_state() {

}

//Computes which action is best to take (UP,LEFT,DOWN,RIGHT,Shoot) for the current player
//based on the state of the current game
int compute_action(int playr_index, Player ** playrs, int no_of_players, int max_y, int max_x) {
	//Storing the current player we are checking for
	Player * current_playr = *(playrs+playr_index);
	
	//Randomly generating an integer for movement
	srand(45);
	int action = (rand() % 5) + 1;

	//Determine if correct action is to move UP (1)
	//If when moving up you are too close to another enemy then set the action to a different integer
	for(;;)
		{
			if (action == 1 && current_playr->y_pos-1 < 2) 
				{
					return 1;
				}
			else 
				{
					srand(45);
					action = (rand() % 5) + 1;
					continue;
				}
			//Determine if correct action is to move DOWN (2)
			if (action == 2 && current_playr->y_pos+1 < max_y/2) 
				{
					return 2;
				}
			else 
				{
					srand(45);
					action = (rand() % 5) + 1;
					continue;
				}
			//Determine if correct action is to move RIGHT (3)
			if (action == 3 && current_playr->x_pos+1 < max_x-1) 
				{
					return 3;
				}
			else 
				{
					srand(45);
					action = (rand() % 5) + 1;
					continue;
				}
			//Determine if correct action is to move LEFT (4)
			if (action == 4 && current_playr->x_pos-1 > 3) 
				{
					return 4;
				}
			else 
				{
					srand(45);
					action = (rand() % 5) + 1;
					continue;
				}

			//Determine if correct action is to shoot (5)
			if (action == 5) 
				{
					return 5;
				}
		}
			
			//Making sure that the enemy doesn't move out of the screen or within two spaces of another enemy
			//and not below halfway down the screen.
}*/



//Dont have players be within two spaces of another player
//Dont have players go below max_y/2 (half way down the screen)







