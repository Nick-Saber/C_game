
typedef struct Bullet {
	int x_pos;
	int y_pos;
	char * character;
	bool is_shot;
} Bullet;

bool update_Bullets(Player * plyr){
	//boolean value returned at end, TRUE if one bullet is freed, FALSE otherwise
	bool freed = FALSE;

	for(int i =0; i<plyr->ammo_size;i++){
		//get i'th bullet pointer in ammo array
		Bullet * bullt = (plyr->ammo + i*sizeof(Bullet *));
		if( plyr->friendly){
			bullt
		}



	}

}