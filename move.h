#ifndef MOVE_H
#define MOVE_H
#include <time.h>

typedef struct{
    time_t startgame;
}Game;
extern Game game;

extern int TIME;
extern int colorhero;
extern int Gold;
extern int numhealth;
extern int numdamage;
extern time_t passtime;

typedef struct{
    time_t food;
}Food;
extern Food food;

typedef struct{
    time_t door;
}Doortime;
extern Doortime doortime;

typedef struct{
    int health;
}Health; 
extern Health health;

typedef struct{
    int floor;
}Floor;
extern Floor flor;

extern int ancient;
extern time_t current_time;
extern int pass;
extern int usepass;


extern int seesnake;

typedef struct{
    int num_Dagger;
    int num_Magic_Wand;
    int num_Sword;
    int num_Normal_Arrow;
}Weapon;
extern Weapon weapon;

typedef struct{
    int is_Mace;
    int is_Dagger;
    int is_Magic_Wand;
    int is_Normal_Arrow;
    int is_Sword;
}Nowweapon;
extern Nowweapon nowweapon;
void start();


#endif