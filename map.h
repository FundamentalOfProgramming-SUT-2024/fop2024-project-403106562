#ifndef MAP_H
#define MAP_H
#include<time.h>


extern int r;
extern int daggerx;
extern int daggery;
extern int magicx;
extern int magicy;
extern int normalx;
extern int normaly;
extern char c[300][300];
extern char backup[300][300];
extern int mark[300][300];
typedef struct{
    int x;
    int y;
    int width;//x
    int hight;//y
}ROOM;
extern ROOM room[20];

typedef struct{
    int x1,y1;//door1
    int x2,y2;//door2
}DOOR;
extern DOOR door[40];

typedef struct{
    int x;
    int y;
}Hidendoor;
extern Hidendoor hidendoor[2];

typedef struct{
    int n;
}Spellroom;
extern Spellroom spellroom;

typedef struct{
    int x;int y;
}Point;

typedef struct{
    int rear;
    int front;
    Point points[7000];
}Queue;

typedef struct{
    int x; int y;
}Trap;
extern Trap trap[2];

typedef struct{
    int x;int y;int width;int hight;
}Scaperoom;
extern Scaperoom scaperoom;

typedef struct{
    int x;int y;
}LockedDoor;
extern LockedDoor  lockeddoor;

typedef struct{
    int health;
    int depth;
    int room;
    int x;
    int y;
}Diamone;
extern Diamone diamone;

typedef struct{
    int health;
    int depth;
    int room;
    int x;
    int y;
}Fire;
extern Fire fire;

typedef struct{
    int health;
    int depth;
    int room;
    int x;
    int y;
}Giant;
extern Giant giant;

typedef struct{
    int health;
    char stablish;
    int room;
    int x;
    int y;
}Snake;
extern Snake snake;

typedef struct{
    int health;
    int depth;
    int room;
    int x;
    int y;
}Undeed;
extern Undeed undeed;

typedef struct{
    int x;
    int y;
}Spellfood;
extern Spellfood spellfood[2];

typedef struct{
    int x;
    int y;
}Damage;
extern Damage damage[2];


void coordinates_doors(int l,DOOR*door,ROOM*room);
void Buildhall(int x2,int y2,int x1,int y1,int x,int y,char c[300][300]);
void map(time_t TIME);




#endif