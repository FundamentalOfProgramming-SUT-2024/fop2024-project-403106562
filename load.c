#include<stdio.h>
#include"move.h"
#include"p2.h"
#include"p.h"
#include"map.h"
#include<ncurses.h>
#include<stdlib.h>

void load(){    
    Floor flor;

        clear();
    refresh();
    FILE*mapfile=fopen(hero.name,"rb");
    fread(&flor.floor,sizeof(int),1,mapfile);
    int x,y;
    fread(&x,sizeof(int),1,mapfile);
    fread(&y,sizeof(int),1,mapfile);
    fread(c,sizeof(char),300*300,mapfile);
    fread(mark,sizeof(int),300*300,mapfile);
    fread(&health,sizeof(Health),1,mapfile);
    fread(&Gold,sizeof(int),1,mapfile);
    //fwrite(&doortime.door,sizeof(time_t),1,mapfile);
    //fwrite(&game.startgame,sizeof(time_t),1,mapfile);
    fread(&passtime,sizeof(time_t),1,mapfile);
    fread(&TIME,sizeof(int),1,mapfile);
    fread(&snake,sizeof(Snake),1,mapfile);
    fread(&diamone,sizeof(Diamone),1,mapfile);
    fread(&undeed,sizeof(Undeed),1,mapfile);
    fread(&fire,sizeof(Fire),1,mapfile);
    fread(&giant,sizeof(Giant),1,mapfile);
    
    fread(&nowweapon,sizeof(Nowweapon),1,mapfile);
    fread(&weapon,sizeof(Weapon),1,mapfile);
    fread(&spellfood,sizeof(Spellfood),1,mapfile);
    fread(&damage,sizeof(Damage),1,mapfile);
    fread(&numhealth,sizeof(int),1,mapfile);
    fread(&numdamage,sizeof(int),1,mapfile);
    fread(&hidendoor,sizeof(Hidendoor),1,mapfile);
    fread(&trap,sizeof(Trap),1,mapfile);
    fread(&colorhero,sizeof(int),1,mapfile);
    fread(&usepass,sizeof(int),1,mapfile);
    fread(&ancient,sizeof(int),1,mapfile);
    fread(&seesnake,sizeof(int),1,mapfile);
    fread(room,sizeof(ROOM),1,mapfile);
    fread(&daggerx,sizeof(int),1,mapfile);
    fread(&daggery,sizeof(int),1,mapfile);
    fread(&normalx,sizeof(int),1,mapfile);
    fread(&normaly,sizeof(int),1,mapfile);
    fread(&magicx,sizeof(int),1,mapfile);
    fread(&magicy,sizeof(int),1,mapfile);
    fread(backup,sizeof(char),300*300,mapfile);
    fflush(mapfile);
    fclose(mapfile);
    for(int i=0;i<300;i++){
        for(int j=0;j<300;j++){
            if(mark[i][j]==1){
                mvprintw(j,i,"%c",c[i][j]);refresh();
            }
        }
    }
    void moveplayer(int x, int y, char c[300][300]);
    getch();
    TIME=TIME-passtime;
    time(&game.startgame);
    mvprintw(y,x,"%c",c[x][y]);refresh();
    moveplayer(x,y,c);


}