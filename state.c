//Struct used for checking the state of the current game for a specific player or enemy
//MHD = Manhatton distance
struct State {
	int MHD_bullet;
	int MHD_player;

}State;

//Computes and returns the average MHD from the current player to all the shot bullets on the map
int compute_MHD_bullets(int playr_index, Player ** playrs, int no_of_players) {
	//Store the current player that you are computing MHDs for
	Player * current_playr = *(playrs + playr_index);

	//Count for the number of bullets and total MHDs
	int total_bullets = 0;
	int total_MHD = 0;
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
					//MHD from them to the current player and add that to the total MHD as well as increment the 
					//number of bullets
					for(int j = 0; j<to_check->ammo_size; j++) 
						{
							Bullet * bullt = to_check->ammo[j];
							if (bullt->is_shot) 
								{
									total_MHD += abs(current_playr->x_pos - bullt->x_pos) + abs(current_playr->y_pos - bullt->y_pos);
									total_bullets++;
								}
						}
				}
		}
		
		//return the avg MHD from the bullets
		if (total_bullets != 0)
			{
				return total_MHD/total_bullets;
			}
		else return 0;
}

//Computes the manhatton distance from the current player to all the other players on the map
int compute_MHD_players(int playr_index, Player ** playrs, int no_of_players) {

}

//Computes which action is best to take (UP,LEFT,DOWN,RIGHT,Shoot) for the current player
//based on the state of the current game
int compute_action(int playr_index, Player ** playrs, int no_of_players) {

}