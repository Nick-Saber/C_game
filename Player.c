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
	plyr->ammo=bullets
}

	//function that displays bullets of players to stdscr using ncurses library
void display_bullets{Player * plyr}{
	for(int i =0;i<plyr->ammo_size;i++)
		{
		Bullet * bullt= plyr->ammo + i*sizeof(Bullet *);
		mvprintw(bullt->y_pos,bullt->x_pos,bullt->character);
		}
}
