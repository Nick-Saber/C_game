typedef struct Player {
	int x_pos;
	int y_pos;
	char * character;
	int ammo_size;
	bullet ** ammo;
	int friendly;


} Player;

void init_ammo(int size. Player * plyr){
	Bullet ** bullets = malloc(size*sizeof(Bullet *));
	for(int i =0; i<size;i++){
		*(ammo + i*sizeof(Bullet *)) = malloc(sizeof(Bullet));
	}
	plyr->ammo=bullets

}
