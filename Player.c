typedef struct Player {
	int x_pos;
	int y_pos;
	char * character;
	int ammo_size;
	Bullet ** ammo;
	bool friendly;


} Player;

void init_ammo(int size. Player * plyr){
	Bullet ** bullets = malloc(size*sizeof(Bullet *));
	for(int i =0; i<size;i++){
		*(ammo + i*sizeof(Bullet *)) = malloc(sizeof(Bullet));
	}
	plyr->ammo=bullets;
}

// This function handles the shooting of ammo 
bool shoot(Player *player) {

	// Iterate through ammo array and find the first free bullet. 
	for(int i = 0; i < sizeOf(player->ammo_size); i++) 
	{
		Bullet * bullet = player->ammo + (i*sizeOf(Bullet *));

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
