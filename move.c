#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<ncurses.h>
#include<time.h>
#include"p2.h"
#include"map.h"
#include<locale.h>
#include<math.h>
#include"setting.h"
#include"p.h"
#include<string.h>
#include<wchar.h>

typedef struct{
    time_t startgame;
}Game;
Game game;

Queue saf;

typedef struct{
    time_t food;
}Food;
Food food;

typedef struct{
    time_t door;
}Doortime;
Doortime doortime;

typedef struct{
    int health;
}Health; 
Health health;

typedef struct{
    int floor;
}Floor;
Floor flor;

int TIME;
int ancient=0;
time_t passtime;
time_t current_time;
int pass;
int usepass;
int colorhero;
int Gold=0;
int numhealth=0;
int depthhealth=0;
int numdamage=0;
int depthdamage=0;

int seesnake=0;

typedef struct{
    int num_Dagger;
    int num_Magic_Wand;
    int num_Sword;
    int num_Normal_Arrow;
}Weapon;
Weapon weapon;

typedef struct{
    int is_Mace;
    int is_Dagger;
    int is_Magic_Wand;
    int is_Normal_Arrow;
    int is_Sword;
}Nowweapon;
Nowweapon nowweapon;

void savegame(FILE*mapfile,int x,int y){
    int f=flor.floor;
    fwrite(&f,sizeof(int),1,mapfile);
    
    fwrite(&x,sizeof(int),1,mapfile);
    fwrite(&y,sizeof(int),1,mapfile);
    fwrite(c,sizeof(char),300*300,mapfile);
    fwrite(mark,sizeof(int),300*300,mapfile);
    fwrite(&health,sizeof(Health),1,mapfile);
    fwrite(&Gold,sizeof(int),1,mapfile);
    //fwrite(&doortime.door,sizeof(time_t),1,mapfile);
    //fwrite(&game.startgame,sizeof(time_t),1,mapfile);
    fwrite(&passtime,sizeof(time_t),1,mapfile);
    fwrite(&TIME,sizeof(int),1,mapfile);
    fwrite(&snake,sizeof(Snake),1,mapfile);
    fwrite(&diamone,sizeof(Diamone),1,mapfile);
    fwrite(&undeed,sizeof(Undeed),1,mapfile);
    fwrite(&fire,sizeof(Fire),1,mapfile);
    fwrite(&giant,sizeof(Giant),1,mapfile);
    
    fwrite(&nowweapon,sizeof(Nowweapon),1,mapfile);
    fwrite(&weapon,sizeof(Weapon),1,mapfile);
    fwrite(&spellfood,sizeof(Spellfood),1,mapfile);
    fwrite(&damage,sizeof(Damage),1,mapfile);
    fwrite(&numhealth,sizeof(int),1,mapfile);
    fwrite(&numdamage,sizeof(int),1,mapfile);
    fwrite(hidendoor,sizeof(Hidendoor),1,mapfile);
    fwrite(trap,sizeof(Trap),1,mapfile);
    fwrite(&colorhero,sizeof(int),1,mapfile);
    fwrite(&usepass,sizeof(int),1,mapfile);
    fwrite(&ancient,sizeof(int),1,mapfile);
    fwrite(&seesnake,sizeof(int),1,mapfile);
    fwrite(room,sizeof(ROOM),1,mapfile);
    fwrite(&daggerx,sizeof(int),1,mapfile);
    fwrite(&daggery,sizeof(int),1,mapfile);
    fwrite(&normalx,sizeof(int),1,mapfile);
    fwrite(&normaly,sizeof(int),1,mapfile);
    fwrite(&magicx,sizeof(int),1,mapfile);
    fwrite(&magicy,sizeof(int),1,mapfile);
    fwrite(backup,sizeof(char),300*300,mapfile);
    
}

Point ** allocateParentArray(){
    Point ** parents=malloc(250*sizeof(Point*));
    for(int i=0 ;i<250;i++){
        parents[i]=malloc(40*sizeof(Point));
    }
    return parents;
}

void freeParentArray(Point ** parent){
    for(int i=0;i<250;i++){
        free(parent[i]);
    }
    free(parent);
}

int find_number_of_room(int x, int y, int l,ROOM*room){
    if(flor.floor==4){return 0;}
    while(1){
        if(x>=room[l].x && x<=room[l].width + room[l].x && y>=room[l].y && y<=room[l].hight + room[l].y){
            return l;
        }
        l=l+1;
    }
}

int find_diamone(int l){
    if(flor.floor==4){return 0;}
    if(diamone.room==l){
        return 1;
    }
    return 0;
}

int find_Fire(int l){
    if(flor.floor==4){return 0;}
    if(fire.room==l){
        return 1;
    }
    return 0;
}

int find_Giant(int l){
    if(flor.floor==4){return 0;}
    if(giant.room==l){
        return 1;
    }
    return 0;
}

int find_Undeed(int l){
    if(flor.floor==4){return 0;}
    if(undeed.room==l){
        return 1;
    }
    return 0;
}

int BFS(Point start, Point end, Queue saf,Point ** Parent,char c[300][300],int x, int y){

    int ** seen=(int**)malloc(200*sizeof(int*));
    for(int i=0;i<200;i++){
       seen[i]=malloc(200*sizeof(int));
    }
    for(int i=0;i<200;i++){
        for(int j=0;j<200;j++){
            seen[i][j]=0;
        }
    }
    seen[start.x][start.y]=1;

    while (saf.rear!=saf.front){
        Point current=saf.points[saf.front];
        //mvprintw(1,0,"%d %d",current.x,current.y);
        saf.front ++;
        if(current.x==end.x && current.y==end.y){
            for(int i=0;i<200;i++){
                free(seen[i]);
            }
            free(seen);
            return 1;
        }
    int ix[4]={1,0,-1,0};
    int jy[4]={0,1,0,-1};
    for(int i=0;i<4;i++){
        int lx=current.x + ix[i]; int ly= current.y +jy[i];
        if(seen[lx][ly]==0 && (c[lx][ly]=='.' || c[lx][ly]=='#' || c[lx][ly]=='+' || c[lx][ly]=='@' || c[lx][ly]=='&'||(lx==hidendoor[0].x && ly==hidendoor[0].y) || (lx==hidendoor[1].x && ly==hidendoor[1].y))
        ) {
            Parent[lx][ly]=current;
            //mvprintw(1,0,"%d %d",lx,ly);
            //if(current==NULL){mvprintw(1,0,"bi");}
            seen[lx][ly]=1;
            Point new; new.x=lx; new.y=ly;
            saf.points[saf.rear]=new;
            saf.rear++;
        }
    }
    }
    return 0;


}

Point step(Point **parent,Point start, Point end){
    Point current=end;
    if(parent[current.x][current.y].x==start.x && parent[current.x][current.y].y==start.y){
        return current;
    }
    return step(parent,start,parent[current.x][current.y]);
}

void movesnake(Snake* snake,int x, int y,char c[300][300]){
    //mvprintw(1,0,"ali");
    Queue saf; saf.rear=0; saf.front=0;
    saf.points[0].x=snake->x; saf.points[0].y=snake->y;
    saf.rear++;
    Point ** parent=allocateParentArray();
    Point psnake; psnake.x=snake->x; psnake.y=snake->y;
    Point hero; hero.x=x; hero.y=y;
    
    if(BFS(psnake,hero,saf,parent,c,155,35)){
        //mvprintw(1,0,"ali");
        Point newp=step(parent,psnake,hero);
        if(newp.x!=x || newp.y!=y){
        c[snake->x][snake->y]=snake->stablish;
        mvprintw(snake->y,snake->x,"%c",backup[snake->x][snake->y]);
        snake->stablish=c[newp.x][newp.y];
        //mvprintw(0,0,"%c",snake->stablish);
        snake->x=newp.x; snake->y=newp.y;
        c[snake->x][snake->y]='S';
        char a='S';
        mvprintw(snake->y,snake->x,"%c",a);
        }
    }
    freeParentArray(parent);
    return;
}

void takeweapon(int x, int y,char c[300][300],Weapon* weapon){
    WINDOW*win=newwin(3,60,0,0);
    box(win,0,0);

    if(c[x][y]=='d'){
    mvwprintw(win,1,1,"you take the Dagger weapon.press enter to continue");
    wrefresh(win);
    if(x==daggerx || daggery==y){
            weapon->num_Dagger=weapon->num_Dagger+10;
        }
    else{
    weapon->num_Dagger++;}
    while(1){
    char a=getch();
    if(a=='\n'){
        werase(win); wrefresh(win);
        refresh();
        //mvprintw(0,0,"%c",c[x][y]);
            break;}
        }
    }

    if(c[x][y]=='m'){
    mvwprintw(win,1,1,"you take the Magic Wand weapon.press enter to continue");
    wrefresh(win);
    if(x==magicx || magicy==y){
            weapon->num_Magic_Wand=weapon->num_Magic_Wand+8;
        }
    else{
    weapon->num_Magic_Wand++;}
    while(1){
    char a=getch();
    if(a=='\n'){
        werase(win); wrefresh(win);
        refresh();
        //mvprintw(0,0,"%c",c[x][y]);
            break;}
        }
    }
    
    if(c[x][y]=='n'){
    mvwprintw(win,1,1,"you take the Normal Arrow weapon.press any key to continue");
    wrefresh(win);
    if(x==normalx || normaly==y){
            weapon->num_Normal_Arrow=weapon->num_Normal_Arrow+20;
        }
    else{
    weapon->num_Normal_Arrow++;}
    while(1){
    char a=getch();
    if(a=='\n'){
        werase(win); wrefresh(win);
        refresh();
        //mvprintw(0,0,"%c",c[x][y]);
            break;}
        }
    
    }

    if(c[x][y]=='s'){
    mvwprintw(win,1,1,"you take the Sword weapon.press any key to continue");
    wrefresh(win);
    getch();
    werase(win); wrefresh(win);
    refresh();
    }
    


}

void menu_weapon(Nowweapon*a,Weapon weapons){
    WINDOW*win=newwin(15,60,0,0);
    box(win,0,0);
    mvwprintw(win,1,1,"1.Mace(Short range)");
    mvwprintw(win,2,1,"2.Dagger(Long range)");
    mvwprintw(win,3,1,"3.Magic Wand(Long range)");
    mvwprintw(win,4,1,"4.Normal Arrow(Long range)");
    mvwprintw(win,5,1,"5.Sword(Short range)");
    wrefresh(win);
    int x=6;
    while(x){
        mvwprintw(win,x,1,"for change the weapon press 1 to 5 ...");wrefresh(win);
        char ch=getch();
        if(ch=='1'){
            mvwprintw(win,x+1,1,"you choose Mace.Press Any key to continue");wrefresh(win);
            x=0; getch(); a->is_Mace=1; a->is_Dagger=0; a->is_Magic_Wand=0; a->is_Normal_Arrow=0; a->is_Sword=0;
            continue;
        }
        if(ch=='2'){
            if(weapons.num_Dagger>0){
            mvwprintw(win,x+1,1,"you choose Dagger.Press Any key to continue");wrefresh(win);
            x=0; getch(); a->is_Mace=0; a->is_Dagger=1; a->is_Magic_Wand=0; a->is_Normal_Arrow=0; a->is_Sword=0; continue;}
            mvwprintw(win,x+1,1,"you don't have Dagger in your weapon");wrefresh(win);
            x=x+2;
        }
        if(ch=='3'){
            if(weapons.num_Magic_Wand>0){
            mvwprintw(win,x+1,1,"you choose Magic Wand.Press Any key to continue");wrefresh(win);
            x=0; getch(); a->is_Mace=0; a->is_Dagger=0; a->is_Magic_Wand=1; a->is_Normal_Arrow=0; a->is_Sword=0; continue;}
            mvwprintw(win,x+1,1,"you don't have Magic Wand in your weapon");wrefresh(win);
            x=x+2;
        }
        if(ch=='4'){
            if(weapons.num_Normal_Arrow>0){
            mvwprintw(win,x+1,1,"you choose Normal Arrow.Press Any key to continue");wrefresh(win);
            x=0; getch(); a->is_Mace=0; a->is_Dagger=0; a->is_Magic_Wand=0; a->is_Normal_Arrow=1; a->is_Sword=0; continue;}
            mvwprintw(win,x+1,1,"you don't have Normal Arrow in your weapon");wrefresh(win);
            x=x+2;
        }
        if(ch=='5'){
            if(weapons.num_Sword>0){
            mvwprintw(win,x+1,1,"you choose Sword.Press Any key to continue");wrefresh(win);
            x=0; getch(); a->is_Mace=0; a->is_Dagger=0; a->is_Magic_Wand=0; a->is_Normal_Arrow=0; a->is_Sword=1; continue;}
            mvwprintw(win,x+1,1,"you don't have Sword in your weapon");wrefresh(win);
            x=x+2;
        }        

    }
    werase(win);
    wrefresh(win);
    for(int i=0;i<300;i++){
        for(int j=0;j<300;j++){
            if(mark[i][j]){
                mvprintw(j,i,"%c",c[i][j]);
            }
        }
      }
      refresh();

}

void result_of_shoot(char ch,int health){
    WINDOW*win=newwin(3,60,0,0);
    box(win,0,0);
    if(ch=='D'){mvwprintw(win,1,1,"You injured Diamone - health= %d",health); wrefresh(win);}
    if(ch=='U'){mvwprintw(win,1,1,"You injured Undeed - health= %d",health); wrefresh(win);}
    if(ch=='F'){mvwprintw(win,1,1,"You injured Fire breathing - health= %d",health); wrefresh(win);}
    if(ch=='G'){mvwprintw(win,1,1,"You injured Giant - health= %d",health); wrefresh(win);}
    if(ch=='S'){mvwprintw(win,1,1,"You injured Snake - health= %d",health); wrefresh(win);}
    getch();
    werase(win);
    wrefresh(win);
}

void remove_enemy(char ch, int x, int y, int health, char c[300][300]){
    if(health<=0){
        WINDOW*win=newwin(3,60,0,0);
        box(win,0,0);
      if(ch=='D'){c[x][y]='.'; mvprintw(y,x,"%c",backup[x][y]) ; mvwprintw(win,1,1,"The Dagger was killed"); wrefresh(win);}
      if(ch=='U'){c[x][y]='.'; mvprintw(y,x,"%c",backup[x][y]) ; mvwprintw(win,1,1,"The Undeed was killed"); wrefresh(win);}
      if(ch=='F'){c[x][y]='.'; mvprintw(y,x,"%c",backup[x][y]) ; mvwprintw(win,1,1,"The Fire breathing was killed"); wrefresh(win);}
      if(ch=='G'){c[x][y]='.'; mvprintw(y,x,"%c",backup[x][y]) ; mvwprintw(win,1,1,"The Giant was killed"); wrefresh(win);}
      if(ch=='S'){c[x][y]=backup[x][y]; mvprintw(y,x,"%c",backup[x][y]) ; mvwprintw(win,1,1,"The Snake was killed"); wrefresh(win);}
      getch();
      werase(win);
      wrefresh(win);
      for(int i=0;i<300;i++){
        for(int j=0;j<300;j++){
            if(mark[i][j]){
                mvprintw(j,i,"%c",c[i][j]);
            }
        }
      }
      refresh();
    
    }
}

void use_Mace(int x, int y, char c[300][300],Diamone*d,Undeed*u,Fire*f,Giant*g,Snake*s){
     int lx[8]={1,1,1,-1,-1,-1,0,0};
     int ly[8]={0,1,-1,0,1,-1,1,-1};
     for(int i=0; i<8;i++){
        if(c[x+lx[i]][y+ly[i]]=='D'){d->health = d->health - 5; result_of_shoot('D',d->health);remove_enemy('D',x+lx[i],y+ly[i],d->health,c);}
        if(c[x+lx[i]][y+ly[i]]=='U'){u->health = u->health - 5; result_of_shoot('U',u->health);remove_enemy('U',x+lx[i],y+ly[i],u->health,c);}
        if(c[x+lx[i]][y+ly[i]]=='F'){f->health=f->health - 5; result_of_shoot('F',f->health);remove_enemy('F',x+lx[i],y+ly[i],f->health,c);}
        if(c[x+lx[i]][y+ly[i]]=='G'){g->health=g->health - 5; result_of_shoot('G',g->health);remove_enemy('G',x+lx[i],y+ly[i],g->health,c);}
        if(c[x+lx[i]][y+ly[i]]=='S'){s->health=s->health - 5; result_of_shoot('S',s->health);remove_enemy('S',x+lx[i],y+ly[i],s->health,c);}

    }
}

void use_Sword(int x, int y, char c[300][300],Diamone*d,Undeed*u,Fire*f,Giant*g,Snake*s){
     int lx[8]={1,1,1,-1,-1,-1,0,0};
     int ly[8]={0,1,-1,0,1,-1,1,-1};
     for(int i=0; i<8;i++){
        if(c[x+lx[i]][y+ly[i]]=='D'){d->health = d->health - 10; result_of_shoot('D',d->health);remove_enemy('D',x+lx[i],y+ly[i],d->health,c);}
        if(c[x+lx[i]][y+ly[i]]=='U'){u->health = u->health - 10; result_of_shoot('U',u->health);remove_enemy('U',x+lx[i],y+ly[i],u->health,c);}
        if(c[x+lx[i]][y+ly[i]]=='F'){f->health= f->health - 10; result_of_shoot('F',f->health);remove_enemy('G',x+lx[i],y+ly[i],f->health,c);}
        if(c[x+lx[i]][y+ly[i]]=='G'){g->health= g->health - 10; result_of_shoot('G',g->health);remove_enemy('F',x+lx[i],y+ly[i],g->health,c);}
        if(c[x+lx[i]][y+ly[i]]=='S'){s->health=s->health - 10; result_of_shoot('S',s->health);remove_enemy('S',x+lx[i],y+ly[i],s->health,c);}

    }
}

void use_Dagger(int x, int y, char c[300][300],char ch,Diamone*d, Undeed*u, Fire*f, Giant*g, Snake*s,Weapon*w){
    if(ch=='w' && c[x][y-1]!='_' && c[x][y-1]!='|' && c[x][y-1]!=' '){
        //exit(0);
        for(int i=1;i<6;i++){
            if(c[x][y-i]=='D'){d->health = d->health - 5; result_of_shoot('D',d->health);remove_enemy('D',x,y-i,d->health,c); w->num_Dagger--;return;}
            if(c[x][y-i]=='U'){u->health = u->health - 5; result_of_shoot('U',u->health);remove_enemy('U',x,y-i,d->health,c); w->num_Dagger--;return;}
            if(c[x][y-i]=='F'){f->health = f->health - 5; result_of_shoot('F',f->health);remove_enemy('F',x,y-i,d->health,c); w->num_Dagger--;return;}
            if(c[x][y-i]=='G'){g->health = g->health - 5; result_of_shoot('G',g->health);remove_enemy('G',x,y-i,d->health,c); w->num_Dagger--;return;}
            if(c[x][y-i]=='S'){s->health = s->health - 5; result_of_shoot('S',s->health);remove_enemy('S',x,y-i,d->health,c); w->num_Dagger--;return;}
            if(c[x][y-i]=='|' || c[x][y-i]=='_' || c[x][y-i]==' '){
                 c[x][y-i+1]='d'; w->num_Dagger--; mvprintw(y-i+1,x,"%c",c[x][y-i+1]); return; 
            }
        }
        c[x][y-5]='d'; w->num_Dagger--; mvprintw(y-5,x,"%c",c[x][y-5]);return; 
    }

    if(ch=='d' && c[x+1][y]!='_' && c[x+1][y]!='|' && c[x+1][y]!=' '){
        for(int i=1;i<6;i++){
            if(c[x+i][y]=='D'){d->health = d->health - 5; result_of_shoot('D',d->health);remove_enemy('D',x+i,y,d->health,c); w->num_Dagger--;return;}
            if(c[x+i][y]=='U'){u->health = u->health - 5; result_of_shoot('U',u->health);remove_enemy('U',x+i,y,d->health,c); w->num_Dagger--;return;}
            if(c[x+i][y]=='F'){f->health = f->health - 5; result_of_shoot('F',f->health);remove_enemy('F',x+i,y,d->health,c); w->num_Dagger--;return;}
            if(c[x+i][y]=='G'){g->health = g->health - 5; result_of_shoot('G',g->health);remove_enemy('G',x+i,y,d->health,c); w->num_Dagger--;return;}
            if(c[x+i][y]=='S'){s->health = s->health - 5; result_of_shoot('S',s->health);remove_enemy('S',x+i,y,d->health,c); w->num_Dagger--;return;}
            if(c[x+i][y]=='|' || c[x+i][y]=='_' || c[x+i][y]==' '){
                 c[x+i-1][y]='d'; w->num_Dagger--; mvprintw(y,x+i-1,"%c",c[x+i-1][y]);return; 
            }
        }
        c[x+5][y]='d'; w->num_Dagger--; mvprintw(y,x+5,"%c",c[x+5][y]);return; 
    }

    if(ch=='s' && c[x][y+1]!='_' && c[x][y+1]!='|' && c[x][y+1]!=' '){
        for(int i=1;i<6;i++){
            if(c[x][y+i]=='D'){d->health = d->health - 5; result_of_shoot('D',d->health);remove_enemy('D',x,y+i,d->health,c); w->num_Dagger--;return;}
            if(c[x][y+i]=='U'){u->health = u->health - 5; result_of_shoot('U',u->health);remove_enemy('U',x,y+i,d->health,c); w->num_Dagger--;return;}
            if(c[x][y+i]=='F'){f->health = f->health - 5; result_of_shoot('F',f->health);remove_enemy('F',x,y+i,d->health,c); w->num_Dagger--;return;}
            if(c[x][y+i]=='G'){g->health = g->health - 5; result_of_shoot('G',g->health);remove_enemy('G',x,y+i,d->health,c); w->num_Dagger--;return;}
            if(c[x][y+i]=='S'){s->health = s->health - 5; result_of_shoot('S',s->health);remove_enemy('S',x,y+i,d->health,c); w->num_Dagger--;return;}
            if(c[x][y+i]=='|' || c[x][y+i]=='_' || c[x][y+i]==' '){
            c[x][y+i-1]='d'; w->num_Dagger--; mvprintw(y+i-1,x,"%c",c[x][y+i-1]);return; 
            }
        }
        c[x][y+5]='d'; w->num_Dagger--; mvprintw(y+5,x,"%c",c[x][y+5]);return; 
    }

    if(ch=='a' && c[x-1][y]!='_' && c[x-1][y]!='|' && c[x-1][y]!=' '){
        for(int i=1;i<6;i++){
            if(c[x-i][y]=='D'){d->health = d->health - 5; result_of_shoot('D',d->health);remove_enemy('D',x-i,y,d->health,c); w->num_Dagger--;return;}
            if(c[x-i][y]=='U'){u->health = u->health - 5; result_of_shoot('U',u->health);remove_enemy('U',x-i,y,d->health,c); w->num_Dagger--;return;}
            if(c[x-i][y]=='F'){f->health = f->health - 5; result_of_shoot('F',f->health);remove_enemy('F',x-i,y,d->health,c); w->num_Dagger--;return;}
            if(c[x-i][y]=='G'){g->health = g->health - 5; result_of_shoot('G',g->health);remove_enemy('G',x-i,y,d->health,c); w->num_Dagger--;return;}
            if(c[x-i][y]=='S'){s->health = s->health - 5; result_of_shoot('S',s->health);remove_enemy('S',x-i,y,d->health,c); w->num_Dagger--;return;}
            if(c[x-i][y]=='|' || c[x-i][y]=='_' || c[x-i][y]==' '){
             c[x-i+1][y]='d'; w->num_Dagger--; mvprintw(y,x-i+1,"%c",c[x-i+1][y]);return; 
            }
        }
        c[x-5][y]='d'; w->num_Dagger--; mvprintw(y,x-5,"%c",c[x-5][y]);return; 
    }

    if(ch=='e' && c[x+1][y-1]!='_' && c[x+1][y-1]!='|' && c[x+1][y-1]!=' '){
        for(int i=1;i<6;i++){
            if(c[x+i][y-i]=='D'){d->health = d->health - 5; result_of_shoot('D',d->health);remove_enemy('D',x+i,y-i,d->health,c); w->num_Dagger--;return;}
            if(c[x+i][y-i]=='U'){u->health = u->health - 5; result_of_shoot('U',u->health);remove_enemy('U',x+i,y-i,d->health,c); w->num_Dagger--;return;}
            if(c[x+i][y-i]=='F'){f->health = f->health - 5; result_of_shoot('F',f->health);remove_enemy('F',x+i,y-i,d->health,c); w->num_Dagger--;return;}
            if(c[x+i][y-i]=='G'){g->health = g->health - 5; result_of_shoot('G',g->health);remove_enemy('G',x+i,y-i,d->health,c); w->num_Dagger--;return;}
            if(c[x+i][y-i]=='S'){s->health = s->health - 5; result_of_shoot('S',s->health);remove_enemy('S',x+i,y-i,d->health,c); w->num_Dagger--;return;}
            if(c[x+i][y-i]=='|' || c[x+i][y-i]=='_' || c[x+i][y-i]==' '){
            c[x+i-1][y-i+1]='d'; w->num_Dagger--; mvprintw(y-i+1,x+i-1,"%c",c[x+i-1][y-i+1]);return; 
            }
        }
         c[x+5][y-5]='d'; w->num_Dagger--; mvprintw(y-5,x+5,"%c",c[x+5][y-5]);return; 
    }

    if(ch=='q' && c[x-1][y-1]!='_' && c[x-1][y-1]!='|' && c[x-1][y-1]!=' '){
        for(int i=1;i<6;i++){
            if(c[x-i][y-i]=='D'){d->health = d->health - 5; result_of_shoot('D',d->health);remove_enemy('D',x-i,y-i,d->health,c); w->num_Dagger--;return;}
            if(c[x-i][y-i]=='U'){u->health = u->health - 5; result_of_shoot('U',u->health);remove_enemy('U',x-i,y-i,d->health,c); w->num_Dagger--;return;}
            if(c[x-i][y-i]=='F'){f->health = f->health - 5; result_of_shoot('F',f->health);remove_enemy('F',x-i,y-i,d->health,c); w->num_Dagger--;return;}
            if(c[x-i][y-i]=='G'){g->health = g->health - 5; result_of_shoot('G',g->health);remove_enemy('G',x-i,y-i,d->health,c); w->num_Dagger--;return;}
            if(c[x-i][y-i]=='S'){s->health = s->health - 5; result_of_shoot('S',s->health);remove_enemy('S',x-i,y-i,d->health,c); w->num_Dagger--;return;}
            if(c[x-i][y-i]=='|' || c[x-i][y-i]=='_' || c[x-i][y-i]==' '){
                 c[x-i+1][y-i+1]='d'; w->num_Dagger--; mvprintw(y-i+1,x-i+1,"%c",c[x-i+1][y-i+1]);return; 
            }
        }
        c[x-5][y-5]='d'; w->num_Dagger--; mvprintw(y-5,x-5,"%c",c[x-5][y-5]);return; 
    }

    if(ch=='x' && c[x+1][y+1]!='_' && c[x+1][y+1]!='|' && c[x+1][y+1]!=' '){
        for(int i=1;i<6;i++){
            if(c[x+i][y+i]=='D'){d->health = d->health - 5; result_of_shoot('D',d->health);remove_enemy('D',x+i,y+i,d->health,c); w->num_Dagger--;return;}
            if(c[x+i][y+i]=='U'){u->health = u->health - 5; result_of_shoot('U',u->health);remove_enemy('U',x+i,y+i,d->health,c); w->num_Dagger--;return;}
            if(c[x+i][y+i]=='F'){f->health = f->health - 5; result_of_shoot('F',f->health);remove_enemy('F',x+i,y+i,d->health,c); w->num_Dagger--;return;}
            if(c[x+i][y+i]=='G'){g->health = g->health - 5; result_of_shoot('G',g->health);remove_enemy('G',x+i,y+i,d->health,c); w->num_Dagger--;return;}
            if(c[x+i][y+i]=='S'){s->health = s->health - 5; result_of_shoot('S',s->health);remove_enemy('S',x+i,y+i,d->health,c); w->num_Dagger--;return;}
            if(c[x+i][y+i]=='|' || c[x+i][y+i]=='_' || c[x][y+i]==' '){
                 c[x+i-1][y+i-1]='d'; w->num_Dagger--; mvprintw(y+i-1,x+i-1,"%c",c[x+i-1][y+i-1]);return; 
            }
        }
        c[x+5][y+5]='d'; w->num_Dagger--; mvprintw(y+5,x+5,"%c",c[x+5][y+5]);return; 
    }

    if(ch=='z' && c[x-1][y+1]!='_' && c[x-1][y+1]!='|' && c[x-1][y+1]!=' '){
        for(int i=1;i<6;i++){
            if(c[x-i][y+i]=='D'){d->health = d->health - 5; result_of_shoot('D',d->health);remove_enemy('D',x-i,y+i,d->health,c); w->num_Dagger--;return;}
            if(c[x-i][y+i]=='U'){u->health = u->health - 5; result_of_shoot('U',u->health);remove_enemy('U',x-i,y+i,d->health,c); w->num_Dagger--;return;}
            if(c[x-i][y+i]=='F'){f->health = f->health - 5; result_of_shoot('F',f->health);remove_enemy('F',x-i,y+i,d->health,c); w->num_Dagger--;return;}
            if(c[x-i][y+i]=='G'){g->health = g->health - 5; result_of_shoot('G',g->health);remove_enemy('G',x-i,y+i,d->health,c); w->num_Dagger--;return;}
            if(c[x-i][y+i]=='S'){s->health = s->health - 5; result_of_shoot('S',s->health);remove_enemy('S',x-i,y+i,d->health,c); w->num_Dagger--;return;}
            if(c[x-i][y+i]=='|' || c[x-i][y+i]=='_' || c[x-i][y+i]==' '){
            c[x-i+1][y+i-1]='d'; w->num_Dagger--; mvprintw(y+i-1,x-i+1,"%c",c[x-i+1][y+i-1]);return; 
            }
        }
        c[x-5][y+5]='d'; w->num_Dagger--; mvprintw(y+5,x-5,"%c",c[x-5][y+5]);return; 
    }
}

void use_Magic(int x, int y, char c[300][300],char ch,Diamone*d, Undeed*u, Fire*f, Giant*g, Snake*s,Weapon*w){
    if(ch=='w' && c[x][y-1]!='_' && c[x][y-1]!='|' && c[x][y-1]!=' '){
        //exit(0);
        for(int i=1;i<6;i++){
            if(c[x][y-i]=='D'){d->health = d->health - 15; result_of_shoot('D',d->health);remove_enemy('D',x,y-i,d->health,c);w->num_Magic_Wand--;return;}
            if(c[x][y-i]=='U'){u->health = u->health - 15; result_of_shoot('U',u->health);remove_enemy('U',x,y-i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x][y-i]=='F'){f->health = f->health - 15; result_of_shoot('F',f->health);remove_enemy('F',x,y-i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x][y-i]=='G'){g->health = g->health - 15; result_of_shoot('G',g->health);remove_enemy('G',x,y-i,d->health,c);w->num_Magic_Wand--;return;}
            if(c[x][y-i]=='S'){s->health = s->health - 15; result_of_shoot('S',s->health);remove_enemy('S',x,y-i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x][y-i]=='|' || c[x][y-i]=='_' || c[x][y-i]==' '){
                 c[x][y-i+1]='m';w->num_Magic_Wand--; mvprintw(y-i+1,x,"%c",c[x][y-i+1]); return; 
            }
        }
        c[x][y-5]='m'; w->num_Magic_Wand--; mvprintw(y-5,x,"%c",c[x][y-5]);return; 
    }

    if(ch=='d' && c[x+1][y]!='_' && c[x+1][y]!='|' && c[x+1][y]!=' '){
        for(int i=1;i<6;i++){
            if(c[x+i][y]=='D'){d->health = d->health - 15; result_of_shoot('D',d->health);remove_enemy('D',x+i,y,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x+i][y]=='U'){u->health = u->health - 15; result_of_shoot('U',u->health);remove_enemy('U',x+i,y,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x+i][y]=='F'){f->health = f->health - 15; result_of_shoot('F',f->health);remove_enemy('F',x+i,y,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x+i][y]=='G'){g->health = g->health - 15; result_of_shoot('G',g->health);remove_enemy('G',x+i,y,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x+i][y]=='S'){s->health = s->health - 15; result_of_shoot('S',s->health);remove_enemy('S',x+i,y,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x+i][y]=='|' || c[x+i][y]=='_' || c[x+i][y]==' '){
                 c[x+i-1][y]='m'; w->num_Magic_Wand--; mvprintw(y,x+i-1,"%c",c[x+i-1][y]);return; 
            }
        }
        c[x+5][y]='m'; w->num_Magic_Wand--; mvprintw(y,x+5,"%c",c[x+5][y]);return; 
    }

    if(ch=='s' && c[x][y+1]!='_' && c[x][y+1]!='|' && c[x][y+1]!=' '){
        for(int i=1;i<6;i++){
            if(c[x][y+i]=='D'){d->health = d->health - 15; result_of_shoot('D',d->health);remove_enemy('D',x,y+i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x][y+i]=='U'){u->health = u->health - 15; result_of_shoot('U',u->health);remove_enemy('U',x,y+i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x][y+i]=='F'){f->health = f->health - 15; result_of_shoot('F',f->health);remove_enemy('F',x,y+i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x][y+i]=='G'){g->health = g->health - 15; result_of_shoot('G',g->health);remove_enemy('G',x,y+i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x][y+i]=='S'){s->health = s->health - 15; result_of_shoot('S',s->health);remove_enemy('S',x,y+i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x][y+i]=='|' || c[x][y+i]=='_' || c[x][y+i]==' '){
            c[x][y+i-1]='m'; w->num_Magic_Wand--; mvprintw(y+i-1,x,"%c",c[x][y+i-1]);return; 
            }
        }
        c[x][y+5]='m'; w->num_Magic_Wand--; mvprintw(y+5,x,"%c",c[x][y+5]);return; 
    }

    if(ch=='a' && c[x-1][y]!='_' && c[x-1][y]!='|' && c[x-1][y]!=' '){
        for(int i=1;i<6;i++){
            if(c[x-i][y]=='D'){d->health = d->health - 15; result_of_shoot('D',d->health);remove_enemy('D',x-i,y,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x-i][y]=='U'){u->health = u->health - 15; result_of_shoot('U',u->health);remove_enemy('U',x-i,y,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x-i][y]=='F'){f->health = f->health - 15; result_of_shoot('F',f->health);remove_enemy('F',x-i,y,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x-i][y]=='G'){g->health = g->health - 15; result_of_shoot('G',g->health);remove_enemy('G',x-i,y,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x-i][y]=='S'){s->health = s->health - 15; result_of_shoot('S',s->health);remove_enemy('S',x-i,y,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x-i][y]=='|' || c[x-i][y]=='_' || c[x-i][y]==' '){
             c[x-i+1][y]='m'; w->num_Magic_Wand--; mvprintw(y,x-i+1,"%c",c[x-i+1][y]);return; 
            }
        }
        c[x-5][y]='m'; w->num_Magic_Wand--; mvprintw(y,x-5,"%c",c[x-5][y]);return; 
    }

    if(ch=='e' && c[x+1][y-1]!='_' && c[x+1][y-1]!='|' && c[x+1][y-1]!=' '){
        for(int i=1;i<6;i++){
            if(c[x+i][y-i]=='D'){d->health = d->health - 15; result_of_shoot('D',d->health);remove_enemy('D',x+i,y-i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x+i][y-i]=='U'){u->health = u->health - 15; result_of_shoot('U',u->health);remove_enemy('U',x+i,y-i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x+i][y-i]=='F'){f->health = f->health - 15; result_of_shoot('F',f->health);remove_enemy('F',x+i,y-i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x+i][y-i]=='G'){g->health = g->health - 15; result_of_shoot('G',g->health);remove_enemy('G',x+i,y-i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x+i][y-i]=='S'){s->health = s->health - 15; result_of_shoot('S',s->health);remove_enemy('S',x+i,y-i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x+i][y-i]=='|' || c[x+i][y-i]=='_' || c[x+i][y-i]==' '){
            c[x+i-1][y-i+1]='m'; w->num_Magic_Wand--; mvprintw(y-i+1,x+i-1,"%c",c[x+i-1][y-i+1]);return; 
            }
        }
         c[x+5][y-5]='m'; w->num_Magic_Wand--; mvprintw(y-5,x+5,"%c",c[x+5][y-5]);return; 
    }

    if(ch=='q' && c[x-1][y-1]!='_' && c[x-1][y-1]!='|' && c[x-1][y-1]!=' '){
        for(int i=1;i<6;i++){
            if(c[x-i][y-i]=='D'){d->health = d->health - 15; result_of_shoot('D',d->health);remove_enemy('D',x-i,y-i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x-i][y-i]=='U'){u->health = u->health - 15; result_of_shoot('U',u->health);remove_enemy('U',x-i,y-i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x-i][y-i]=='F'){f->health = f->health - 15; result_of_shoot('F',f->health);remove_enemy('F',x-i,y-i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x-i][y-i]=='G'){g->health = g->health - 15; result_of_shoot('G',g->health);remove_enemy('G',x-i,y-i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x-i][y-i]=='S'){s->health = s->health - 15; result_of_shoot('S',s->health);remove_enemy('S',x-i,y-i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x-i][y-i]=='|' || c[x-i][y-i]=='_' || c[x-i][y-i]==' '){
                 c[x-i+1][y-i+1]='m'; w->num_Magic_Wand--; mvprintw(y-i+1,x-i+1,"%c",c[x-i+1][y-i+1]);return; 
            }
        }
        c[x-5][y-5]='m'; w->num_Magic_Wand--; mvprintw(y-5,x-5,"%c",c[x-5][y-5]);return; 
    }

    if(ch=='x' && c[x+1][y+1]!='_' && c[x+1][y+1]!='|' && c[x+1][y+1]!=' '){
        for(int i=1;i<6;i++){
            if(c[x+i][y+i]=='D'){d->health = d->health - 15; result_of_shoot('D',d->health);remove_enemy('D',x+i,y+i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x+i][y+i]=='U'){u->health = u->health - 15; result_of_shoot('U',u->health);remove_enemy('U',x+i,y+i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x+i][y+i]=='F'){f->health = f->health - 15; result_of_shoot('F',f->health);remove_enemy('F',x+i,y+i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x+i][y+i]=='G'){g->health = g->health - 15; result_of_shoot('G',g->health);remove_enemy('G',x+i,y+i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x+i][y+i]=='S'){s->health = s->health - 15; result_of_shoot('S',s->health);remove_enemy('S',x+i,y+i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x+i][y+i]=='|' || c[x+i][y+i]=='_' || c[x][y+i]==' '){
                 c[x+i-1][y+i-1]='m'; w->num_Magic_Wand--; mvprintw(y+i-1,x+i-1,"%c",c[x+i-1][y+i-1]);return; 
            }
        }
        c[x+5][y+5]='m'; w->num_Magic_Wand--; mvprintw(y+5,x+5,"%c",c[x+5][y+5]);return; 
    }

    if(ch=='z' && c[x-1][y+1]!='_' && c[x-1][y+1]!='|' && c[x-1][y+1]!=' '){
        for(int i=1;i<6;i++){
            if(c[x-i][y+i]=='D'){d->health = d->health - 15; result_of_shoot('D',d->health);remove_enemy('D',x-i,y+i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x-i][y+i]=='U'){u->health = u->health - 15; result_of_shoot('U',u->health);remove_enemy('U',x-i,y+i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x-i][y+i]=='F'){f->health = f->health - 15; result_of_shoot('F',f->health);remove_enemy('F',x-i,y+i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x-i][y+i]=='G'){g->health = g->health - 15; result_of_shoot('G',g->health);remove_enemy('G',x-i,y+i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x-i][y+i]=='S'){s->health = s->health - 15; result_of_shoot('S',s->health);remove_enemy('S',x-i,y+i,d->health,c); w->num_Magic_Wand--;return;}
            if(c[x-i][y+i]=='|' || c[x-i][y+i]=='_' || c[x-i][y+i]==' '){
            c[x-i+1][y+i-1]='m'; w->num_Magic_Wand--; mvprintw(y+i-1,x-i+1,"%c",c[x-i+1][y+i-1]);return; 
            }
        }
        c[x-5][y+5]='m'; w->num_Magic_Wand--; mvprintw(y+5,x-5,"%c",c[x-5][y+5]);return; 
    }
}

void use_NormalArrow(int x, int y, char c[300][300],char ch,Diamone*d, Undeed*u, Fire*f, Giant*g, Snake*s,Weapon*w){
    if(ch=='w' && c[x][y-1]!='_' && c[x][y-1]!='|' && c[x][y-1]!=' '){
        //exit(0);
        for(int i=1;i<6;i++){
            if(c[x][y-i]=='D'){d->health = d->health - 5; result_of_shoot('D',d->health);remove_enemy('D',x,y-i,d->health,c);w->num_Normal_Arrow--;return;}
            if(c[x][y-i]=='U'){u->health = u->health - 5; result_of_shoot('U',u->health);remove_enemy('U',x,y-i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x][y-i]=='F'){f->health = f->health - 5; result_of_shoot('F',f->health);remove_enemy('F',x,y-i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x][y-i]=='G'){g->health = g->health - 5; result_of_shoot('G',g->health);remove_enemy('G',x,y-i,d->health,c);w->num_Normal_Arrow--;return;}
            if(c[x][y-i]=='S'){s->health = s->health - 5; result_of_shoot('S',s->health);remove_enemy('S',x,y-i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x][y-i]=='|' || c[x][y-i]=='_' || c[x][y-i]==' '){
                 c[x][y-i+1]='n';w->num_Normal_Arrow--; mvprintw(y-i+1,x,"%c",c[x][y-i+1]); return; 
            }
        }
        c[x][y-5]='n'; w->num_Normal_Arrow--; mvprintw(y-5,x,"%c",c[x][y-5]);return; 
    }

    if(ch=='d' && c[x+1][y]!='_' && c[x+1][y]!='|' && c[x+1][y]!=' '){
        for(int i=1;i<6;i++){
            if(c[x+i][y]=='D'){d->health = d->health - 5; result_of_shoot('D',d->health);remove_enemy('D',x+i,y,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x+i][y]=='U'){u->health = u->health - 5; result_of_shoot('U',u->health);remove_enemy('U',x+i,y,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x+i][y]=='F'){f->health = f->health - 5; result_of_shoot('F',f->health);remove_enemy('F',x+i,y,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x+i][y]=='G'){g->health = g->health - 5; result_of_shoot('G',g->health);remove_enemy('G',x+i,y,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x+i][y]=='S'){s->health = s->health - 5; result_of_shoot('S',s->health);remove_enemy('S',x+i,y,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x+i][y]=='|' || c[x+i][y]=='_' || c[x+i][y]==' '){
                 c[x+i-1][y]='n'; w->num_Normal_Arrow--; mvprintw(y,x+i-1,"%c",c[x+i-1][y]);return; 
            }
        }
        c[x+5][y]='n'; w->num_Normal_Arrow--; mvprintw(y,x+5,"%c",c[x+5][y]);return; 
    }

    if(ch=='s' && c[x][y+1]!='_' && c[x][y+1]!='|' && c[x][y+1]!=' '){
        for(int i=1;i<6;i++){
            if(c[x][y+i]=='D'){d->health = d->health - 5; result_of_shoot('D',d->health);remove_enemy('D',x,y+i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x][y+i]=='U'){u->health = u->health - 5; result_of_shoot('U',u->health);remove_enemy('U',x,y+i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x][y+i]=='F'){f->health = f->health - 5; result_of_shoot('F',f->health);remove_enemy('F',x,y+i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x][y+i]=='G'){g->health = g->health - 5; result_of_shoot('G',g->health);remove_enemy('G',x,y+i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x][y+i]=='S'){s->health = s->health - 5; result_of_shoot('S',s->health);remove_enemy('S',x,y+i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x][y+i]=='|' || c[x][y+i]=='_' || c[x][y+i]==' '){
            c[x][y+i-1]='n'; w->num_Normal_Arrow--; mvprintw(y+i-1,x,"%c",c[x][y+i-1]);return; 
            }
        }
        c[x][y+5]='n'; w->num_Normal_Arrow--; mvprintw(y+5,x,"%c",c[x][y+5]);return; 
    }

    if(ch=='a' && c[x-1][y]!='_' && c[x-1][y]!='|' && c[x-1][y]!=' '){
        for(int i=1;i<6;i++){
            if(c[x-i][y]=='D'){d->health = d->health - 5; result_of_shoot('D',d->health);remove_enemy('D',x-i,y,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x-i][y]=='U'){u->health = u->health - 5; result_of_shoot('U',u->health);remove_enemy('U',x-i,y,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x-i][y]=='F'){f->health = f->health - 5; result_of_shoot('F',f->health);remove_enemy('F',x-i,y,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x-i][y]=='G'){g->health = g->health - 5; result_of_shoot('G',g->health);remove_enemy('G',x-i,y,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x-i][y]=='S'){s->health = s->health - 5; result_of_shoot('S',s->health);remove_enemy('S',x-i,y,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x-i][y]=='|' || c[x-i][y]=='_' || c[x-i][y]==' '){
             c[x-i+1][y]='n'; w->num_Normal_Arrow--; mvprintw(y,x-i+1,"%c",c[x-i+1][y]);return; 
            }
        }
        c[x-5][y]='n'; w->num_Normal_Arrow--; mvprintw(y,x-5,"%c",c[x-5][y]);return; 
    }

    if(ch=='e' && c[x+1][y-1]!='_' && c[x+1][y-1]!='|' && c[x+1][y-1]!=' '){
        for(int i=1;i<6;i++){
            if(c[x+i][y-i]=='D'){d->health = d->health - 5; result_of_shoot('D',d->health);remove_enemy('D',x+i,y-i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x+i][y-i]=='U'){u->health = u->health - 5; result_of_shoot('U',u->health);remove_enemy('U',x+i,y-i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x+i][y-i]=='F'){f->health = f->health - 5; result_of_shoot('F',f->health);remove_enemy('F',x+i,y-i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x+i][y-i]=='G'){g->health = g->health - 5; result_of_shoot('G',g->health);remove_enemy('G',x+i,y-i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x+i][y-i]=='S'){s->health = s->health - 5; result_of_shoot('S',s->health);remove_enemy('S',x+i,y-i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x+i][y-i]=='|' || c[x+i][y-i]=='_' || c[x+i][y-i]==' '){
            c[x+i-1][y-i+1]='n'; w->num_Normal_Arrow--; mvprintw(y-i+1,x+i-1,"%c",c[x+i-1][y-i+1]);return; 
            }
        }
         c[x+5][y-5]='n'; w->num_Normal_Arrow--; mvprintw(y-5,x+5,"%c",c[x+5][y-5]);return; 
    }

    if(ch=='q' && c[x-1][y-1]!='_' && c[x-1][y-1]!='|' && c[x-1][y-1]!=' '){
        for(int i=1;i<6;i++){
            if(c[x-i][y-i]=='D'){d->health = d->health - 5; result_of_shoot('D',d->health);remove_enemy('D',x-i,y-i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x-i][y-i]=='U'){u->health = u->health - 5; result_of_shoot('U',u->health);remove_enemy('U',x-i,y-i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x-i][y-i]=='F'){f->health = f->health - 5; result_of_shoot('F',f->health);remove_enemy('F',x-i,y-i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x-i][y-i]=='G'){g->health = g->health - 5; result_of_shoot('G',g->health);remove_enemy('G',x-i,y-i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x-i][y-i]=='S'){s->health = s->health - 5; result_of_shoot('S',s->health);remove_enemy('S',x-i,y-i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x-i][y-i]=='|' || c[x-i][y-i]=='_' || c[x-i][y-i]==' '){
                 c[x-i+1][y-i+1]='n'; w->num_Normal_Arrow--; mvprintw(y-i+1,x-i+1,"%c",c[x-i+1][y-i+1]);return; 
            }
        }
        c[x-5][y-5]='n'; w->num_Normal_Arrow--; mvprintw(y-5,x-5,"%c",c[x-5][y-5]);return; 
    }

    if(ch=='x' && c[x+1][y+1]!='_' && c[x+1][y+1]!='|' && c[x+1][y+1]!=' '){
        for(int i=1;i<6;i++){
            if(c[x+i][y+i]=='D'){d->health = d->health - 5; result_of_shoot('D',d->health);remove_enemy('D',x+i,y+i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x+i][y+i]=='U'){u->health = u->health - 5; result_of_shoot('U',u->health);remove_enemy('U',x+i,y+i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x+i][y+i]=='F'){f->health = f->health - 5; result_of_shoot('F',f->health);remove_enemy('F',x+i,y+i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x+i][y+i]=='G'){g->health = g->health - 5; result_of_shoot('G',g->health);remove_enemy('G',x+i,y+i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x+i][y+i]=='S'){s->health = s->health - 5; result_of_shoot('S',s->health);remove_enemy('S',x+i,y+i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x+i][y+i]=='|' || c[x+i][y+i]=='_' || c[x][y+i]==' '){
                 c[x+i-1][y+i-1]='n'; w->num_Normal_Arrow--; mvprintw(y+i-1,x+i-1,"%c",c[x+i-1][y+i-1]);return; 
            }
        }
        c[x+5][y+5]='n'; w->num_Normal_Arrow--; mvprintw(y+5,x+5,"%c",c[x+5][y+5]);return; 
    }

    if(ch=='z' && c[x-1][y+1]!='_' && c[x-1][y+1]!='|' && c[x-1][y+1]!=' '){
        for(int i=1;i<6;i++){
            if(c[x-i][y+i]=='D'){d->health = d->health - 5; result_of_shoot('D',d->health);remove_enemy('D',x-i,y+i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x-i][y+i]=='U'){u->health = u->health - 5; result_of_shoot('U',u->health);remove_enemy('U',x-i,y+i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x-i][y+i]=='F'){f->health = f->health - 5; result_of_shoot('F',f->health);remove_enemy('F',x-i,y+i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x-i][y+i]=='G'){g->health = g->health - 5; result_of_shoot('G',g->health);remove_enemy('G',x-i,y+i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x-i][y+i]=='S'){s->health = s->health - 5; result_of_shoot('S',s->health);remove_enemy('S',x-i,y+i,d->health,c); w->num_Normal_Arrow--;return;}
            if(c[x-i][y+i]=='|' || c[x-i][y+i]=='_' || c[x-i][y+i]==' '){
            c[x-i+1][y+i-1]='n'; w->num_Normal_Arrow--; mvprintw(y+i-1,x-i+1,"%c",c[x-i+1][y+i-1]);return; 
            }
        }
        c[x-5][y+5]='n'; w->num_Normal_Arrow--; mvprintw(y+5,x-5,"%c",c[x-5][y+5]);return; 
    }
}

void injured(char ch){
    //mvprintw(0,0,"bbbbbbbb");
    WINDOW*win=newwin(3,60,0,0);
    box(win,0,0);
    if(ch=='S'){
        mvwprintw(win,1,1,"Snake hit you");wrefresh(win);
    }
    if(ch=='U'){
        mvwprintw(win,1,1,"Undeed hit you");wrefresh(win);
    }
    if(ch=='G'){
        mvwprintw(win,1,1,"Giant hit you");wrefresh(win);
    }
    if(ch=='F'){
        mvwprintw(win,1,1,"Fire Breathing hit you");wrefresh(win);
    }
    if(ch=='D'){
        mvwprintw(win,1,1,"Deamon hit you");wrefresh(win);
    }
    
    for(int i=0;i<300;i++){
        for(int j=0;j<300;j++){
            if(mark[i][j]){
                mvprintw(j,i,"%c",c[i][j]);
            }
        }
    }

}

void enemystrike(int x, int y, char c[300][300],Health*health){
    char ch;
    int lx[4]={1,-1,0,0};
    int ly[4]={0,0,1,-1};
    for(int i=0;i<4;i++){
        if(c[x+lx[i]][y+ly[i]]=='S'){
            health->health=health->health-5;
            ch='S';
            injured(ch);
        }
        if(c[x+lx[i]][y+ly[i]]=='U'){
            health->health=health->health-4;
            ch='U';
            injured(ch);
        }
        if(c[x+lx[i]][y+ly[i]]=='G'){
            health->health=health->health-3;
            ch='G';
            injured(ch);
        }
        if(c[x+lx[i]][y+ly[i]]=='F'){
            health->health=health->health-2;
            ch='F';
            injured(ch);
        }
        if(c[x+lx[i]][y+ly[i]]=='D'){
            health->health=health->health-1;
            ch='D';
            injured(ch);
        }
        //mvprintw(0,0,"%d",health->health);
    }
}

void takecoin(){
    WINDOW*win=newwin(3,60,0,0);
    box(win,0,0);
    start_color();
    init_pair(4,COLOR_YELLOW,COLOR_BLACK);
    attron(COLOR_PAIR(4));
    wbkgd(win,COLOR_BLACK);
    mvwprintw(win,1,1,"you took 5 coin"); wrefresh(win);
    attroff(COLOR_PAIR(4));
    werase(win);

}

void sortplayers(FILE*file){
    file=fopen("temp.txt","r");
    char l[100];
    char **User=(char **)malloc(2000*sizeof(char*));
    //char **GOLD=(char **)malloc(2000*sizeof(char*));

    char*C=(char*)malloc(10*sizeof(char));
    char*id=(char*)malloc(50*sizeof(char));
    char*n=(char*)malloc(10*sizeof(char));

    for(int i=0;i<2000;i++){
        User[i]=(char*)malloc(50*sizeof(char));
        //GOLD[i]=(char*)malloc(50*sizeof(char));
    }
    int *G=(int *)malloc(2000*sizeof(int));
    int *N=(int*)malloc(2000*sizeof(int));
    int line_cursor=0;
    while(fgets(l,sizeof(l),file)!=NULL){
        l[strcspn(l,"\n")]='\0';
        id=strtok(l,":\n");
        strcpy(User[line_cursor],id);
        C=strtok(NULL,":\n");
        n=strtok(NULL,":\n");
        //printf("%s %s\n",User[line_cursor], GOLD[line_cursor]);
        sscanf(C,"%d",G+line_cursor);
        sscanf(n,"%d",N+line_cursor);
    
        line_cursor++;
    }
    
    fclose(file);
     file=fopen("gold.txt","w");
     int competition1;
     int competition2;
     char sharp[1][50];

      for(int i=0;i<line_cursor;i++){
          for(int j=i+1;j<line_cursor;j++){
              if(G[i]>G[j]){
                  competition1=G[i];
                  G[i]=G[j];
                  G[j]=competition1;
                  strcpy(sharp[0],User[i]);
                  strcpy(User[i],User[j]);
                  strcpy(User[j],sharp[0]);
                  competition2=N[i];
                  N[i]=N[j];
                  N[j]=competition2;


              }

          }
      }
       for(int i=line_cursor-1;i>=0;i--){
           fprintf(file,"%s:%d:%d\n",User[i],G[i],N[i]);
       }
       fclose(file);
      //for(int i=0;i<2000;i++){
          //free(User[i]);
      //}
      //free(User); free(G);
}

void creatlastflor(int x, int y, int width, int hight,char c[300][300]){
    //exit(0);
    for(int i=x;i<x+width;i++){
            for(int j=y;j<y+hight;j++){
                if(i==x){
                    if(j!=y){
                    c[i][j]='|';}
                }
                else if(i==x+width-1){
                    if(j!=y){
                    c[i][j]='|';}
                }
                else{
                    if(j==y || j==y+hight-1){
                        c[i][j]='_';
                    }
                    else{                        
                        c[i][j]='.';
                    }
                }
                mvprintw(j,i,"%c",c[i][j]);


            }
        }
}

int findenemy(char c[300][300]){
    for(int i=0;i<300;i++){
        for(int j=0;j<300;j++){
            if(c[i][j]=='S' || c[i][j]=='U' || c[i][j]=='F' || c[i][j]=='G' || c[i][j]=='D'){
            return 1;}
        }
    }
    return 0;
}

void showmap(int x, int y){
    for(int i=0;i<x;i++){
        for(int j=0;j<y;j++){
            mvprintw(j,i,"%c",c[i][j]);
        }
    }
}

void color(int x, int y,int k, int l,int number){
    init_pair(1,COLOR_YELLOW,COLOR_BLACK);
    init_pair(2,COLOR_RED,COLOR_BLACK);
    init_pair(3,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(4,COLOR_CYAN,COLOR_BLACK);
     if(number==0){
         init_pair(5,COLOR_BLUE,COLOR_BLACK);
     }
     if(number==1){
         init_pair(5,COLOR_GREEN,COLOR_BLACK);
     }



    for(int i=0;i<x;i++){
        for(int j=0;j<y;j++){
            if(mark[i][j]){
                    if(c[i][j]=='C' || c[i][j]=='k'){
                       attron(COLOR_PAIR(1));
                       mvprintw(j,i,"%c",c[i][j]);refresh();
                       attroff(COLOR_PAIR(1));
                   }
                   else if(c[i][j]=='.' || c[i][j]=='|' || c[i][j]=='_'){
                       attron(COLOR_PAIR(4));
                       mvprintw(j,i,"%c",c[i][j]);refresh();
                       attroff(COLOR_PAIR(4));
                   }
                   else if(c[i][j]=='$' || c[i][j]=='&'){
                       attron(COLOR_PAIR(3));
                       mvprintw(j,i,"%c",c[i][j]);refresh();
                       attroff(COLOR_PAIR(3));
                   }
                   else if(c[i][j]=='O'){
                       attron(COLOR_PAIR(2));
                       mvprintw(j,i,"%c",c[i][j]);refresh();
                       attroff(COLOR_PAIR(2));
                   }
                   else if(c[i][j]=='#'){
                    mvprintw(j,i,"\u2592");
                   }
                   else if(c[i][j]=='('){
                        if((i==spellfood[0].x && j==spellfood[0].y) || (i==spellfood[1].x && j==spellfood[1].y)){ 
                            setlocale(LC_ALL,"");
                            attron(COLOR_PAIR(2));
                            mvprintw(j,i,"\xE2\x9D\xA4");
                            attroff(COLOR_PAIR(2));}
                        if((i==damage[0].x && j==damage[0].y) || (i==damage[1].x && j==damage[1].y)){ 
                            setlocale(LC_ALL,"");
                            attron(COLOR_PAIR(1));

                            attroff(COLOR_PAIR(1));} 
                   }
                    else if(i==k && j==l){
                     attron(COLOR_PAIR(5));
                     //char a='*';
                     mvprintw(j,i,"\u2362");
                     attroff(COLOR_PAIR(5));
                    }

            }
        }
    }

}

void spellmenu(){
    WINDOW*win=newwin(7,60,0,0);
    box(win,0,0);
    mvwprintw(win,1,1," Health num:%d",numhealth);wrefresh(win);
    mvwprintw(win,3,1," Damage num:%d",numdamage);wrefresh(win);
    mvwprintw(win,5,1,"Press any key to continue ...");wrefresh(win);
    getch();
    werase(win); wrefresh(win);

}

void hidemap(){
    clear(); refresh();
    start_color();
    init_pair(4,COLOR_YELLOW,COLOR_BLACK);

    for(int i=0;i<300;i++){
        for(int j=0;j<300;j++){
            if(mark[i][j]){
                if(c[i][j]=='C'){
                   attron(COLOR_PAIR(4));
                   mvprintw(j,i,"%c",c[i][j]);
                   attroff(COLOR_PAIR(4));
                }
                else{
                    mvprintw(j,i,"%c",c[i][j]);
                }
            }
        }
    }
}

void creatnewfloor(char c[300][300]){

     srandom(time(NULL));
     while(flor.floor!=4){
        //mvprintw(0,0,"%d",flor.floor);
        map(TIME-current_time);
     int s= random();
     int X=s%(room[0].width-2)+room[0].x+1;
     int Y=s%(room[0].hight -2)+room[0].y+1;
     if(c[X][Y]=='.'){
        c[X][Y]='*';
         //++flor.floor;
     //moveplayer(X,Y,c);
     return;
     }
     }
     if(flor.floor==4){
        for(int i=0;i<300;i++){
            for(int j=0;j<300;j++){
                c[i][j]=' ';
            }
        }
        creatlastflor(40,10,8,8,c);
        char evil[6]={'S','D','U','F','G'};
        for(int i=0;i<5;i++){
            while(1){
                int x=rand()%7 +40;
                int y=rand()%7 +10;
                if(c[x][y]=='.'){
                    c[x][y]=evil[i];
                    mvprintw(y,x,"%c",c[x][y]);refresh();
                    break;
                }
            }
        }
        while(1){
            int x=rand()%7 +40;
            int y=rand()%7 +10;
            if(c[x][y]=='.'){
                c[x][y]='*';
                //flor.floor=-1;
                break;

            } 
        }
       
       
    }
     

}

void printhall(char ch,char c[300][300],int x,int y){

    if(ch=='w'){
        for(int i=y; i>=y-5 && i>=0 && (c[x][i]=='#' || c[x][i]=='S');i--){
            mvprintw(i,x,"%c",c[x][i]); mark[x][i]=1;
           
        }
         return;
    }
    if(ch=='s'){
        for(int i=y; i<=y+5 && i>=0 && (c[x][i]=='#' || c[x][i]=='S');i++){
            mvprintw(i,x,"%c",c[x][i]); mark[x][i]=1;
        }
        return;
    }
    if(ch=='d'){
        for(int i=x; i<=x+5 && i>=0 && (c[i][y]=='#' || c[i][y]=='S');i++){
            mvprintw(y,i,"%c",c[i][y]); mark[i][y]=1;
        }
         return;
    }
    if(ch=='a'){
        for(int i=x; i>=x-5 && i>=0 && (c[i][y]=='#' || c[i][y]=='S');i--){
            mvprintw(y,i,"%c",c[i][y]); mark[i][y]=1;
        }
         return;
    }
    if(ch=='e'){
        for(int i=y; i>=y-5 && x-i+y>=0 && c[x-i+y][i]=='#';i--){
            mvprintw(i,x-i+y,"%c",c[x-i+y][i]); mark[x-i+y][i]=1;
        }
         return;
    }
    if(ch=='q'){
        for(int i=y; i>=y-5 && x+i-y>=0 && c[x+i-y][i]=='#';i--){
            mvprintw(i,x+i-y,"%c",c[x+i-y][i]); mark[x+i-y][i]=1;
        }
         return;
    }
    if(ch=='x'){
        for(int i=y; i<=y+5 && i>=0 && c[x+i-y][i]=='#';i++){
            mvprintw(i,x+i-y,"%c",c[x+i-y][i]); mark[x+i-y][i]=1;
        }
         return;
    }
    if(ch=='z'){
        for(int i=y; i<=y+5 && x-i+y>=0 && c[x-i+y][i]=='#';i++){
            mvprintw(i,x-i+y,"%c",c[x-i+y][i]); mark[x-i+y][i]=1;
        }
         return;
    }
}

void printroom(char ch,char c[300][300], int x, int y,DOOR door[40],ROOM room[20]){

    int l=0;
    start_color();
    init_pair(1,COLOR_YELLOW,COLOR_BLACK);
    init_pair(2,COLOR_RED,COLOR_BLACK);
    init_pair(3,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(4,COLOR_CYAN,COLOR_BLACK);

    if(can_change_color()){
        init_color(10,1000,500,0);
    }
    init_pair(5,10,COLOR_BLACK);

   while(1){
       if(find_number_of_room(x,y,l,room)==l){
           for(int i=room[l].x; i<room[l].x + room[l].width;i++){
               for(int j=room[l].y; j<room[l].y + room[l].hight;j++){
                   if(c[i][j]=='S'){
                       seesnake=1;
                   }
                   else if(c[i][j]=='C' || c[i][j]=='k'){
                       attron(COLOR_PAIR(1));
                       mvprintw(j,i,"%c",c[i][j]);
                       attroff(COLOR_PAIR(1));
                   }
                   else if(c[i][j]=='.' || c[i][j]=='|' || c[i][j]=='_'){
                       attron(COLOR_PAIR(4));
                       mvprintw(j,i,"%c",c[i][j]);
                       attroff(COLOR_PAIR(4));
                   }
                   else if(c[i][j]=='$' || c[i][j]=='&'){
                       attron(COLOR_PAIR(3));
                       mvprintw(j,i,"%c",c[i][j]);
                       attroff(COLOR_PAIR(3));
                   }
                   else if(c[i][j]=='O'){
                       attron(COLOR_PAIR(2));
                       mvprintw(j,i,"%c",c[i][j]);
                       attroff(COLOR_PAIR(2));
                   }
                   if(c[i][j]=='('){
                        if((i==spellfood[0].x && j==spellfood[0].y) || (i==spellfood[1].x && j==spellfood[1].y)){ 
                            setlocale(LC_ALL,"");
                            attron(COLOR_PAIR(2));
                            mvprintw(j,i,"\xE2\x9D\xA4");
                            attroff(COLOR_PAIR(2));}
                        if((i==damage[0].x && j==damage[0].y) || (i==damage[1].x && j==damage[1].y)){ 
                            setlocale(LC_ALL,"");
                            attron(COLOR_PAIR(5));
                            mvprintw(j,i,"\xE2\x9A\x94");
                            attroff(COLOR_PAIR(5));}    



                   }
                   else{
                   if(c[i][j]!='#'){
                   mvprintw(j,i,"%c",c[i][j]);

                   mark[i][j]=1;}

               }

           }
       }
        return;
     }
     l++;

 }
}

void moveplayer(int x, int y, char c[300][300]){
    
    int r,s; getmaxyx(stdscr,s,r);

     WINDOW*message=newwin(4,50,s-5,7);
     box(message,0,0);
     mvwprintw(message,1,1, "Health: %d Time remaining: %ld ", health.health,TIME-current_time+game.startgame);
     for(int i=1;i<health.health/4 +1;i++){
        mvwprintw(message,2,i,"#");wrefresh(message);
     }
     wrefresh(message);

    time(&current_time);
    passtime=current_time-game.startgame;
    mvprintw(0,0,"%ld",current_time-game.startgame);refresh();

     if(current_time-game.startgame>TIME || health.health<=0 || (flor.floor==4 && findenemy(c)==0)){
        clear(); refresh();
        WINDOW*win=newwin(10,60,5,40);
        box(win,0,0);
        if(current_time-game.startgame>TIME || health.health<=0){
        mvwprintw(win,1,1,"Rest in peice");wrefresh(win);
        mvwprintw(win,3,1,"GOLD=%d",Gold/2);wrefresh(win);
        mvwprintw(win,5,1,"press any key to continue...");
        mvwprintw(win,7,1,"%ld",current_time-game.startgame);wrefresh(win);}
        
        else if(flor.floor==4 && findenemy(c)==0){
        mvwprintw(win,1,1,"You win");wrefresh(win);
        mvwprintw(win,3,1,"GOLD=%d",Gold);wrefresh(win);
        mvwprintw(win,5,1,"press any key to continue...");wrefresh(win);}

        if(strcmp(hero.name,"Guest")){
         FILE*file=fopen("gold.txt","r");
         FILE*temp=fopen("temp.txt","w");
         char line[100];
         char*C=(char*)malloc(10*sizeof(char));
         char*id=(char*)malloc(50*sizeof(char));
         char*numgame=(char*)malloc(10*sizeof(char));
          while(fgets(line,sizeof(line),file) !=NULL){
            line[strcspn(line,"\r\n")]='\0';
             id=strtok(line,":");
             C=strtok(NULL,":");
             numgame=strtok(NULL,":");
             if(id==NULL){exit(0);}
            int *g=(int*)malloc(sizeof(int));
            int *a=(int*)malloc(sizeof(int));
             //if(C==NULL || id==NULL || g==NULL){menu();}
        //     mvprintw(0,0,"%s",id);
               sscanf(C,"%d",g);
               sscanf(numgame,"%d",a);
                    if(strcmp(id,hero.name)==0){
                        if(flor.floor==4 && findenemy(c)==0){
                            fprintf(temp,"%s:%d:%d\n",id,*g+Gold,*a+1);}
                        if(current_time-game.startgame>TIME || health.health<=0){
                            fprintf(temp,"%s:%d:%d\n",id,*g+Gold/2,*a+1);
                        }

                  continue;
                    }
                   fprintf(temp,"%s:%d:%d\n",id,*g,*a+1);
          }
          
           fclose(file); fclose(temp);           
           sortplayers(file);
           //fclose(file);
    

         getch();
         werase(win); wrefresh(win);
         getch();
         clear();
         setting();    
    }
    }
    

    if(current_time-food.food>=4){
        health.health--;
        time(&food.food);
    }

    if(c[x][y]=='$'){
        health.health=health.health+2;
        //mvprintw(0,0,"%d",health.health);
        c[x][y]='.';
    }
    if(c[x][y]=='<'){

        clear();
        seesnake=0;
        usepass=0;
        // for(int i=0;i<300;i++){
        //     for(int j=0;j<300;j++){
        //         c[i][j]=' ';
        //     }
        // }
        ++flor.floor;
        mvprintw(0,0,"%d",flor.floor);

        creatnewfloor(c);

         for(int i=0;i<300;i++){
             for(int j=0;j<300;j++){
                 if(c[i][j]=='*'){
                    c[i][j]='.'; x=i;y=j;
                 }
             }
         }
        if(flor.floor==4){
        giant.depth=0; diamone.depth=0; fire.depth=0; undeed.depth=0;
        giant.health=15; diamone.health=5; fire.health=10; undeed.health=30; snake.health=20;}
        
        if(flor.floor==4){
        for(int i=40;i<47;i++){
            for(int j=10;j<17;j++){
                if(c[i][j]=='S'){
                    snake.x=i;snake.y=j;
                }
                if(c[i][j]=='D'){
                    diamone.x=i;diamone.y=j;
                }
                if(c[i][j]=='U'){
                    undeed.x=i;undeed.y=j;
                }
                if(c[i][j]=='F'){
                    fire.x=i;fire.y=j;
                }
                if(c[i][j]=='G'){
                    giant.x=i;giant.y=j;
                }
                if(c[i][j]=='*'){
                    c[i][j]='.'; backup[i][j]=c[i][j];
                    x=i;y=j;
                }                
            }
        }
    }
    }
   
    noecho();
    start_color();
    if(colorhero==0){
        init_pair(5,COLOR_BLUE,COLOR_BLACK);
        color(r,s,x,y,colorhero);
        attron(COLOR_PAIR(5));
        //char a='*';
        mvprintw(y,x,"\u2362");refresh();
        //attroff(COLOR_PAIR(5));
        
    }
    if(colorhero==1){
        init_pair(5,COLOR_GREEN,COLOR_BLACK);
        color(r,s,x,y,colorhero);
        attron(COLOR_PAIR(5));
        char a='*';
        mvprintw(y,x,"\u2362");refresh();
        //attroff(COLOR_PAIR(5));
    }

    if(c[x][y]=='('){
        c[x][y]='.';
        if((x==spellfood[0].x && y==spellfood[0].y  || x==spellfood[1].x && y==spellfood[1].y)){
            WINDOW*win=newwin(3,60,0,0);
            mvwprintw(win,1,1,"You took the health spell");wrefresh(win); ++numhealth;
        }
        if((x==damage[0].x && y==damage[0].y  || x==damage[1].x && y==damage[1].y)){
            WINDOW*win=newwin(3,60,0,0);
            mvwprintw(win,1,1,"You took the damage spell");wrefresh(win); ++numdamage;
        }
    }

    // color(r,s,x,y,colorhero);
    // attron(COLOR_PAIR(5));
    // mvprintw(y,x,"%c",'*');refresh();
    // attroff(COLOR_PAIR(5));
    //color(r,s,x,y,colorhero);

    if(c[x][y]=='k'){
        if(ancient==0){
            ancient++;
        }            
                }
    if(c[x][y]=='&'){
        time(&doortime.door);
        srandom(time(NULL));
        pass=random()%9000+1000;
        WINDOW*win=newwin(3,6,s-4,0);
        box(win,0,0);
        mvwprintw(win,1,1,"%d",pass);wrefresh(win);
        usepass=1;
    }

    if((x==hidendoor[0].x && y==hidendoor[0].y) || (x==hidendoor[1].x && y==hidendoor[1].y)){
        WINDOW*win=newwin(3,60,0,0);
        box(win,0,0);
        mvwprintw(win,1,1,"you are in the spellroom");
        health.health=health.health-10;
    }

    if( ((x==trap[0].x && y==trap[0].y) || (x==trap[1].x && y==trap[1].y)) && c[x][y]=='.' ){
        health.health=health.health-10;
        WINDOW*Trap=newwin(3,60,0,0); box(Trap,0,0);
        mvwprintw(Trap,1,1,"You Are in trap.Scape quicly ..."); wrefresh(Trap);
                
    }
    
     if(c[x][y]!='#'){
         int N=find_number_of_room(x,y,0,room);
        
            if((find_diamone(N)|| flor.floor==4) && c[x][y]!='+' && diamone.depth<9 ){
                

                Point position_start; position_start.x=diamone.x; position_start.y=diamone.y;
                 Point position_end; position_end.x=x; position_end.y=y;
                 Point **Parent=allocateParentArray();   
            
                 saf.rear=0; saf.front=0;
                 saf.points[saf.rear]=position_start;
                 saf.rear=saf.rear+1;
              
            if(abs(position_end.x-position_start.x)>=1 || abs(position_end.y-position_start.y)>=1){
                  int exist=BFS(position_start,position_end,saf,Parent,c,r,s);
                  if(exist && diamone.health>0){
                    
                  Point new_position=step(Parent,position_start,position_end);

                  if(new_position.x!=x || new_position.y!=y){
                    diamone.depth++;
                   c[new_position.x][new_position.y]='D';
                   diamone.x=new_position.x; diamone.y=new_position.y;
                   c[position_start.x][position_start.y]='.';
                   mvprintw(new_position.y, new_position.x,"%c",c[new_position.x][new_position.y]);
                   refresh();
                   mvprintw(position_start.y, position_start.x,"%c",c[position_start.x][position_start.y]);
                   refresh();}
                  }
            }
                   freeParentArray(Parent);
                   //color(r,s);

            }

            if((find_Fire(N)||flor.floor==4) && c[x][y]!='+' && fire.depth<9 ){
                

                Point position_start; position_start.x=fire.x; position_start.y=fire.y;
                 Point position_end; position_end.x=x; position_end.y=y;
                 Point **Parent=allocateParentArray();   
            
                 saf.rear=0; saf.front=0;
                 saf.points[saf.rear]=position_start;
                 saf.rear=saf.rear+1;
              
            if(abs(position_end.x-position_start.x)>=1 || abs(position_end.y-position_start.y)>=1){
                  int exist=BFS(position_start,position_end,saf,Parent,c,r,s);
                  if(exist && fire.health>0){
                    
                  Point new_position=step(Parent,position_start,position_end);
                  if(new_position.x!=x || new_position.y!=y){
                    fire.depth++;
                   c[new_position.x][new_position.y]='F';
                   fire.x=new_position.x; fire.y=new_position.y;
                   c[position_start.x][position_start.y]='.';
                   mvprintw(new_position.y, new_position.x,"%c",c[new_position.x][new_position.y]);
                   refresh();
                   mvprintw(position_start.y, position_start.x,"%c",c[position_start.x][position_start.y]);
                   refresh();}
                  }
            }
                   freeParentArray(Parent);
                   //color(r,s);

            }

            if((find_Giant(N)|| flor.floor==4) && c[x][y]!='+' && giant.depth<9 ){
                

                Point position_start; position_start.x=giant.x; position_start.y=giant.y;
                 Point position_end; position_end.x=x; position_end.y=y;
                 Point **Parent=allocateParentArray();   
            
                 saf.rear=0; saf.front=0;
                 saf.points[saf.rear]=position_start;
                 saf.rear=saf.rear+1;
              
            if(abs(position_end.x-position_start.x)>=1 || abs(position_end.y-position_start.y)>=1){
                  int exist=BFS(position_start,position_end,saf,Parent,c,r,s);
                  if(exist && giant.health>0){
                    
                  Point new_position=step(Parent,position_start,position_end);
                 
                 if(new_position.x!=x || new_position.y!=y){
                    giant.depth++;
                   c[new_position.x][new_position.y]='G';
                   giant.x=new_position.x; giant.y=new_position.y;
                   c[position_start.x][position_start.y]='.';
                   mvprintw(new_position.y, new_position.x,"%c",c[new_position.x][new_position.y]);
                   refresh();
                   mvprintw(position_start.y, position_start.x,"%c",c[position_start.x][position_start.y]);
                   refresh();}
                  }
            }
                   freeParentArray(Parent);
                   //color(r,s);

            }

            if((find_Undeed(N)|| flor.floor==4) && c[x][y]!='+' && undeed.depth<9 ){
                

                Point position_start; position_start.x=undeed.x; position_start.y=undeed.y;
                 Point position_end; position_end.x=x; position_end.y=y;
                 Point **Parent=allocateParentArray();   
            
                 saf.rear=0; saf.front=0;
                 saf.points[saf.rear]=position_start;
                 saf.rear=saf.rear+1;
              
            if(abs(position_end.x-position_start.x)>=1 || abs(position_end.y-position_start.y)>=1){
                  int exist=BFS(position_start,position_end,saf,Parent,c,r,s);
                  if(exist && undeed.health>0){
                    
                  Point new_position=step(Parent,position_start,position_end);

                  if(new_position.x!=x || new_position.y!=y){
                    undeed.depth++;
                   c[new_position.x][new_position.y]='U';
                   undeed.x=new_position.x; undeed.y=new_position.y;
                   c[position_start.x][position_start.y]='.';
                   mvprintw(new_position.y, new_position.x,"%c",c[new_position.x][new_position.y]);
                   refresh();
                   mvprintw(position_start.y, position_start.x,"%c",c[position_start.x][position_start.y]);
                   refresh();}
                  }
            }
                   freeParentArray(Parent);
                   //color(r,s);

            }

    }
    
    if((seesnake || flor.floor==4) && snake.health>0 ){
    Snake* addrssnake=malloc(sizeof(Snake));
    *addrssnake=snake;
    movesnake(addrssnake,x,y,c);
    snake=*addrssnake;
    free(addrssnake);
    //color(r,s,x,y,colorhero);
    }

    if(c[x][y]=='d' || c[x][y]=='m' || c[x][y]=='n' || c[x][y]=='s'){
        Weapon*w=malloc(sizeof(Weapon));
        *w=weapon;
        takeweapon(x,y,c,w);
        if(c[x][y]=='d'){   
            if(x==daggerx && y==daggery){
                c[x][y]='.'; mvprintw(y,x,"%c",c[x][y]);
            }
            else if(x!=daggerx || y!=daggery){       
            c[x][y]=backup[x][y]; mvprintw(y,x,"%c",c[x][y]);}            
        }

        if(c[x][y]=='m'){   
            if(x==magicx && y==magicy){
                c[x][y]='.'; mvprintw(y,x,"%c",c[x][y]);
            }
            else if(x!=magicx || y!=magicy){       
            c[x][y]=backup[x][y]; mvprintw(y,x,"%c",c[x][y]);}            
        }

        if(c[x][y]=='n'){   
            if(x==normalx && y==normaly){
                c[x][y]='.'; mvprintw(y,x,"%c",c[x][y]);
            }
            else if(x!=normalx || y!=normaly){       
            c[x][y]=backup[x][y]; mvprintw(y,x,"%c",c[x][y]);}            
        }
        if(c[x][y]=='s'){
            c[x][y]='.';mvprintw(y,x,"%c",c[x][y]);
        }

        weapon=*w;
        free(w);
    }
    if(c[x][y]=='C'){
        c[x][y]='.';
        takecoin();
        Gold=Gold+5;
    }
    
    if( (x==trap[0].x && y==trap[0].y) || (x==trap[1].x && y==trap[1].y) ){
        c[x][y]='^';
    }
    
    char ch=getch();

    if(ch=='l' || ch=='L'){
        WINDOW*win=newwin(17,60,10,40);
        box(win,0,0);
        mvwprintw(win,4,4,"Do you want to leave? 1-YES 2-NO");wrefresh(win);
        char t=getch();
        if(t==49){
        FILE*mapfile=fopen(hero.name,"wb");
        if(mapfile==NULL){perror("Error opening file");}
        savegame(mapfile,x,y);
        //fflush(mapfile);
        //fclose(mapfile);
        if(fclose(mapfile)!=0){perror("Error closing file");}
        clear();refresh();//exit(0);
        menu();}
        if(t==50){
            wclear(win); wrefresh(win);
            refresh();
            color(r,s,x,y,colorhero);
        }
        ch=getch();
    }

    if(numhealth>0){
        ++depthhealth;
        if(depthhealth<11){
            health.health=health.health+3;
        }
        else{
            numhealth --;
        }

    }
    if(numdamage>0){
        ++depthdamage;
        if(depthdamage<11){
    
        }
        else{
            numdamage --;
        }

    }
    if(ch=='k'){
        spellmenu();
    }
    

    if(ch=='m'){
        showmap(r,s);
        ch=getch();
        hidemap();
    }


    Health*h=malloc(sizeof(Health));
    *h=health;
    enemystrike(x,y,c,h);
    health=*h;
    free(h);

    

    if(ch=='i'){
        Nowweapon*a=malloc(sizeof(Nowweapon));
        *a=nowweapon;
        menu_weapon(a,weapon);
        nowweapon=*a;
        free(a);
    }

    while(ch==' '){
        Diamone *d=malloc(sizeof(Diamone)); *d=diamone;
            Undeed *u=malloc(sizeof(Undeed)); *u=undeed;
            Fire *f=malloc(sizeof(Fire)); *f=fire;
            Giant *g=malloc(sizeof(Giant)); *g=giant;
            Snake *s=malloc(sizeof(Snake)); *s=snake;

        if(nowweapon.is_Mace==1 || nowweapon.is_Sword==1){
            
            if(nowweapon.is_Mace){
                use_Mace(x,y,c,d,u,f,g,s);
                diamone=*d; undeed=*u; fire=*f; giant=*g; snake=*s;
                free(d); free(u);free(f);free(g);free(s);
            }
            if(nowweapon.is_Sword && weapon.num_Sword){
                use_Sword(x,y,c,d,u,f,g,s);
                diamone=*d; undeed=*u; fire=*f; giant=*g; snake=*s;
                free(d); free(u);free(f);free(g);free(s);
            }

        }
        if(nowweapon.is_Dagger){
            ch=getch();
            if(weapon.num_Dagger==0){
                WINDOW*win=newwin(3,60,0,0);
                box(win,0,0);
                mvwprintw(win,1,1,"You don't have Dagger in your weapon");wrefresh(win);
                getch(); werase(win); wrefresh(win);
            }
            else{
            Weapon*w=malloc(sizeof(Weapon));
            *w=weapon;
            use_Dagger(x,y,c,ch,d,u,f,g,s,w);
            weapon=*w;
            //mvprintw(7,0,w->)
            diamone=*d; undeed=*u; fire=*f; giant=*g; snake=*s;
            free(w);free(d);free(u);free(f);free(g);free(s);
            }
        }
            if(nowweapon.is_Magic_Wand){
            ch=getch();
            if(weapon.num_Magic_Wand==0){
                WINDOW*win=newwin(3,60,0,0);
                box(win,0,0);
                mvwprintw(win,1,1,"You don't have Magic Wand in your weapon");wrefresh(win);
                getch(); werase(win); wrefresh(win);
            }
            else{
            Weapon*w=malloc(sizeof(Weapon));
            *w=weapon;
            use_Magic(x,y,c,ch,d,u,f,g,s,w);
            weapon=*w;
            //mvprintw(7,0,w->)
            diamone=*d; undeed=*u; fire=*f; giant=*g; snake=*s;
            free(w);free(d);free(u);free(f);free(g);free(s);
            }
        }

        if(nowweapon.is_Normal_Arrow){
            ch=getch();
            if(weapon.num_Normal_Arrow==0){
                WINDOW*win=newwin(3,60,0,0);
                box(win,0,0);
                mvwprintw(win,1,1,"You don't have Normal Arrow in your weapon");wrefresh(win);
                getch(); werase(win); wrefresh(win);
            }
            else{
            Weapon*w=malloc(sizeof(Weapon));
            *w=weapon;
            use_NormalArrow(x,y,c,ch,d,u,f,g,s,w);
            weapon=*w;
            //mvprintw(7,0,w->)
            diamone=*d; undeed=*u; fire=*f; giant=*g; snake=*s;
            free(w);free(d);free(u);free(f);free(g);free(s);
            }
        }


        ch=getch();
    }

    if(c[x][y]=='#'){
        printhall(ch,c,x,y);
    }

    if(c[x][y]=='+' || c[x][y]=='@' || c[x][y]=='.'|| (x==hidendoor[0].x && y==hidendoor[0].y) || (x==hidendoor[1].x && y==hidendoor[1].y)){
        if(flor.floor!=-1){
        printroom(ch,c,x,y,door,room);}
    }
    
    
    init_pair(3,COLOR_WHITE,COLOR_BLACK);
    attron(COLOR_PAIR(3));
    
   
    switch (ch){

        case 'w':
                //mvprintw(0,0,"di");
                if(c[x][y-1]=='|' || c[x][y-1]=='_' || c[x][y-1]=='O'){
                    if((x==hidendoor[0].x && y-1==hidendoor[0].y)||(x==hidendoor[1].x && y-1==hidendoor[1].y)){
                        c[x][y-1]='?';
                        mvprintw(y,x,"%c",c[x][y]);
                        moveplayer(x,y-1,c);
                    }
                    else{
                        moveplayer(x,y,c);
                    }
                }
                if(c[x][y-1]==' ' ||c[x][y-1]=='D' || c[x][y-1]=='U' || c[x][y-1]=='G' || c[x][y-1]=='S' || c[x][y-1]=='F'){
                    moveplayer(x,y,c);
                }
                
                if(c[x][y-1]=='@'){
                    if(c[x][y]=='#'|| c[x][y]=='+'){
                        mvprintw(y,x,"%c",c[x][y]);
                        moveplayer(x,y-1,c);
                    }
                    else if(ancient!=0){
                        ancient=0;
                        mvprintw(y,x,"%c",c[x][y]);
                        moveplayer(x,y-1,c);
                    }
                    else{
                        time(&current_time);
                        if(current_time-doortime.door >30 || usepass==0){
                            WINDOW*win=newwin(3,60,0,0);
                            mvwprintw(win,1,1,"Your pass is expired"); wrefresh(win);
                            WINDOW*new=newwin(3,6,s-4,0); box(new,0,0);
                            moveplayer(x,y,c);
                        }
                        else{
                            int number=0;   
                            int pass1;
                             WINDOW*win=newwin(3,60,0,0);
                             box(win,0,0);
                            mvwprintw(win,1,1,"enter your pass: ");
                            while(number<3){
                            echo();
                            wscanw(win,"%d",&pass1);
                            noecho();
                            
                                if(pass==pass1){
                                    attroff(COLOR_PAIR(1));attroff(COLOR_PAIR(3));
                                    mvprintw(y,x,"%c",c[x][y]);
                                    usepass=0;
                                    moveplayer(x,y-1,c);
                                    werase(win);wrefresh(win);WINDOW*new=newwin(3,6,s-4,0);wrefresh(new);
                                }
                                else{
                                    mvwprintw(win,0,0,"                       ");
                                    number++;
                                    if(number==1){
                                        start_color();
                                        init_pair(3,COLOR_YELLOW,COLOR_BLACK);
                                        wattron(win,COLOR_PAIR(3));
                                        mvwprintw(win,1,1,"pass is wrong.try another: ");wrefresh(win);
                                        wattroff(win,COLOR_PAIR(3)); 
                                        
                                    }
                                    if(number==2){
                                        start_color();
                                        init_pair(1,COLOR_RED,COLOR_BLACK);
                                        wattron(win,COLOR_PAIR(1));
                                        mvwprintw(win,1,1,"pass is wrong.try another: ");wrefresh(win);
                                        wattroff(win,COLOR_PAIR(1));
                                    }
                                    if(number==3){
                                        //mvprintw(0,0,"                                    ");
                                        mvwprintw(win,1,1,"the door is shot");wrefresh(win);
                                        WINDOW*new=newwin(3,6,s-4,0);wrefresh(new);
                                        //attroff(COLOR_PAIR(1));attroff(COLOR_PAIR(3));
                                        if(c[x][y-2]==' ' || c[x][y-2]=='#'){
                                            c[x][y-1]='_';mvprintw(y-1,x,"_");
                                        }
                                        else{
                                            c[x][y-1]='|';mvprintw(y-1,x,"|");
                                        }
                                        moveplayer(x,y,c);
                                    }
                                }

                            
                            }  
                        }
                    }
                }
                //attron(COLOR_PAIR(5));
                mvprintw(y,x,"%c",c[x][y]);
                //attroff(COLOR_PAIR(5));
                moveplayer(x,y-1,c);

                //if(c[x][y-1]=='>')
        case 's':
                if(c[x][y+1]=='|' || c[x][y+1]=='_' || c[x][y+1]=='O'){
                    if((x==hidendoor[0].x && y+1==hidendoor[0].y)||(x==hidendoor[1].x && y+1==hidendoor[1].y)){
                        c[x][y+1]='?';
                        mvprintw(y,x,"%c",c[x][y]);
                        moveplayer(x,y+1,c);
                    }
                    else{
                        moveplayer(x,y,c);
                    }
                }
                if(c[x][y+1]==' ' ||c[x][y+1]=='D' || c[x][y+1]=='U' || c[x][y+1]=='G' || c[x][y+1]=='S' || c[x][y+1]=='F'){
                    moveplayer(x,y,c);
                }
                if(c[x][y+1]=='@'){
                    if(c[x][y]=='#'|| c[x][y]=='+'){
                        mvprintw(y,x,"%c",c[x][y]);
                        moveplayer(x,y+1,c);
                    }
                    else if(ancient!=0){
                        ancient=0;
                        mvprintw(y,x,"%c",c[x][y]);
                        moveplayer(x,y+1,c);
                    }
                    else{
                        time(&current_time);
                        if(current_time-doortime.door >30 || usepass==0){
                            WINDOW*win=newwin(3,60,0,0);
                            mvwprintw(win,1,1,"Your pass is expired"); wrefresh(win);
                            WINDOW*new=newwin(3,6,s-4,0); box(new,0,0);
                            moveplayer(x,y,c);
                        }
                        else{
                            int number=0;   
                            int pass1;
                             WINDOW*win=newwin(3,60,0,0);
                             box(win,0,0);
                            mvwprintw(win,1,1,"enter your pass: ");
                            while(number<3){
                            echo();
                            wscanw(win,"%d",&pass1);
                            noecho();
                            
                                if(pass==pass1){
                                    attroff(COLOR_PAIR(1));attroff(COLOR_PAIR(3));
                                    mvprintw(y,x,"%c",c[x][y]);
                                    usepass=0;
                                    moveplayer(x,y+1,c);
                                    werase(win);wrefresh(win);WINDOW*new=newwin(3,6,s-4,0);wrefresh(new);
                                }
                                else{
                                    mvwprintw(win,0,0,"                       ");
                                    number++;
                                    if(number==1){
                                        start_color();
                                        init_pair(3,COLOR_YELLOW,COLOR_BLACK);
                                        wattron(win,COLOR_PAIR(3));
                                        mvwprintw(win,1,1,"pass is wrong.try another: ");wrefresh(win);
                                        wattroff(win,COLOR_PAIR(3)); 
                                        
                                    }
                                    if(number==2){
                                        start_color();
                                        init_pair(1,COLOR_RED,COLOR_BLACK);
                                        wattron(win,COLOR_PAIR(1));
                                        mvwprintw(win,1,1,"pass is wrong.try another: ");wrefresh(win);
                                        wattroff(win,COLOR_PAIR(1));
                                    }
                                    if(number==3){
                                         //mvprintw(0,0,"                                    ");
                                        mvwprintw(win,1,1,"the door is shot");
                                        WINDOW*new=newwin(3,6,s-4,0);wrefresh(new);
                                        
                                        if(c[x][y+2]==' ' || c[x][y+2]=='#'){
                                            c[x][y+1]='_';
                                            mvprintw(y+1,x,"_");
                                        }
                                        else{
                                            c[x][y+1]='|';mvprintw(y+1,x,"|");
                                        }
                                        moveplayer(x,y,c);
                                    }
                                }

                            
                            }  
                        }
                    }
                }
                mvprintw(y,x,"%c",c[x][y]);
                moveplayer(x,y+1,c);  
                                         
                                        
                                    
                                  
                //if(c[x][y-1]=='>') 
        case 'd':
                if(c[x+1][y]=='|' || c[x+1][y]=='_' || c[x+1][y]=='O'){
                    if((x+1==hidendoor[0].x && y==hidendoor[0].y)||(x+1==hidendoor[1].x && y==hidendoor[1].y)){
                        c[x+1][y]='?';
                        mvprintw(y,x,"%c",c[x][y]);
                        moveplayer(x+1,y,c);
                    }
                    else{
                        moveplayer(x,y,c);
                    }
                }
                if(c[x+1][y]==' '||c[x+1][y]=='D' || c[x+1][y]=='U' || c[x+1][y]=='G' || c[x+1][y]=='S' || c[x+1][y]=='F'){
                    moveplayer(x,y,c);
                }

                if(c[x+1][y]=='@'){
                    if(c[x][y]=='#'|| c[x][y]=='+'){
                        mvprintw(y,x,"%c",c[x][y]);
                        moveplayer(x+1,y,c);
                    }
                    else if(ancient!=0){
                        ancient=0;
                        mvprintw(y,x,"%c",c[x][y]);
                        moveplayer(x+1,y,c);
                    }
                    else{
                        time(&current_time);
                        if(current_time-doortime.door >30 || usepass==0){
                            WINDOW*win=newwin(3,60,0,0);
                            mvwprintw(win,1,1,"Your pass is expired"); wrefresh(win);
                            WINDOW*new=newwin(3,6,s-4,0); box(new,0,0);
                            moveplayer(x,y,c);
                        }
                        else{
                            int number=0;   
                            int pass1;
                             WINDOW*win=newwin(3,60,0,0);
                             box(win,0,0);
                            mvwprintw(win,1,1,"enter your pass: ");
                            while(number<3){
                            echo();
                            wscanw(win,"%d",&pass1);
                            noecho();
                            
                                if(pass==pass1){
                                    attroff(COLOR_PAIR(1));attroff(COLOR_PAIR(3));
                                    mvprintw(y,x,"%c",c[x][y]);
                                    usepass=0;
                                    moveplayer(x+1,y,c);
                                    werase(win);wrefresh(win);WINDOW*new=newwin(3,6,s-4,0);wrefresh(new);
                                }
                                else{
                                    mvwprintw(win,0,0,"                       ");
                                    number++;
                                    if(number==1){
                                        start_color();
                                        init_pair(3,COLOR_YELLOW,COLOR_BLACK);
                                        wattron(win,COLOR_PAIR(3));
                                        mvwprintw(win,1,1,"pass is wrong.try another: ");wrefresh(win);
                                        wattroff(win,COLOR_PAIR(3)); 
                                        
                                    }
                                    if(number==2){
                                        start_color();
                                        init_pair(1,COLOR_RED,COLOR_BLACK);
                                        wattron(win,COLOR_PAIR(1));
                                        mvwprintw(win,1,1,"pass is wrong.try another: ");wrefresh(win);
                                        wattroff(win,COLOR_PAIR(1));
                                    }
                                    if(number==3){
                                        //mvprintw(0,0,"                                    ");
                                        mvwprintw(win,1,1,"the door is shot");
                                        WINDOW*new=newwin(3,6,s-4,0);wrefresh(new);
                                        if(c[x+1][y+1]=='|' || c[x+1][y-1]=='|' || c[x+1][y+1]==' ' || c[x+1][y-1]==' ' || c[x+1][y+1]=='+'|| c[x+1][y-1]=='+'){
                                            c[x+1][y]='|';mvprintw(y,x+1,"|");
                                        }
                                        else{
                                            c[x+1][y]='_';mvprintw(y,x+1,"_");
                                        }
                                        moveplayer(x,y,c);
                                    }
                                }

                            
                            }  
                        }
                    }
                }
                mvprintw(y,x,"%c",c[x][y]);
                moveplayer(x+1,y,c);

                //if(c[x][y-1]=='>')                        
        case 'a':
                if(c[x-1][y]=='|' || c[x-1][y]=='_' || c[x-1][y]=='O'){
                    if((x-1==hidendoor[0].x && y==hidendoor[0].y)||(x-1==hidendoor[1].x && y==hidendoor[1].y)){
                        c[x-1][y]='?';
                        mvprintw(y,x,"%c",c[x][y]);
                        moveplayer(x-1,y,c);
                    }
                    else{
                        moveplayer(x,y,c);
                    }
                }
                if(c[x-1][y]==' ' ||c[x-1][y]=='D' || c[x-1][y]=='U' || c[x-1][y]=='G' || c[x-1][y]=='S' || c[x-1][y]=='F'){
                    moveplayer(x,y,c);
                }
            
                if(c[x-1][y]=='@'){
                    if(c[x][y]=='#'|| c[x][y]=='+'){
                        mvprintw(y,x,"%c",c[x][y]);
                        moveplayer(x-1,y,c);
                    }
                    else if(ancient!=0){
                        ancient=0;
                        mvprintw(y,x,"%c",c[x][y]);
                        moveplayer(x-1,y,c);
                    }
                    else{
                        time(&current_time);
                        if(current_time-doortime.door >30 || usepass==0){
                            WINDOW*win=newwin(3,60,0,0);
                            mvwprintw(win,1,1,"Your pass is expired"); wrefresh(win);
                            WINDOW*new=newwin(3,6,s-4,0); box(new,0,0);
                            moveplayer(x,y,c);
                        }
                        else{
                            int number=0;   
                            int pass1;
                             WINDOW*win=newwin(3,60,0,0);
                             box(win,0,0);
                            mvwprintw(win,1,1,"enter your pass: ");
                            while(number<3){
                            echo();
                            wscanw(win,"%d",&pass1);
                            noecho();
                            
                                if(pass==pass1){
                                    attroff(COLOR_PAIR(1));attroff(COLOR_PAIR(3));
                                    mvprintw(y,x,"%c",c[x][y]);
                                    usepass=0;
                                    moveplayer(x-1,y,c);
                                    werase(win);wrefresh(win);WINDOW*new=newwin(3,6,s-4,0);wrefresh(new);
                                }
                                else{
                                    mvwprintw(win,0,0,"                       ");
                                    number++;
                                    if(number==1){
                                        start_color();
                                        init_pair(3,COLOR_YELLOW,COLOR_BLACK);
                                        wattron(win,COLOR_PAIR(3));
                                        mvwprintw(win,1,1,"pass is wrong.try another: ");wrefresh(win);
                                        wattroff(win,COLOR_PAIR(3)); 
                                        
                                    }
                                    if(number==2){
                                        start_color();
                                        init_pair(1,COLOR_RED,COLOR_BLACK);
                                        wattron(win,COLOR_PAIR(1));
                                        mvwprintw(win,1,1,"pass is wrong.try another: ");wrefresh(win);
                                        wattroff(win,COLOR_PAIR(1));
                                    }
                                    if(number==3){
                                        // mvprintw(0,0,"                                    ");
                                        mvwprintw(win,1,1,"the door is shot");wrefresh(win);
                                        WINDOW*new=newwin(3,6,s-4,0);wrefresh(new);
                                        //attroff(COLOR_PAIR(1));attroff(COLOR_PAIR(3));
                                        if(c[x-1][y+1]==' ' || c[x-1][y+1]=='|' || c[x-1][y+1]=='+'|| c[x-1][y-1]==' ' || c[x-1][y-1]=='|' || c[x-1][y-1]=='+'){
                                            c[x-1][y]='|';mvprintw(y,x-1,"|");
                                        }
                                        else{
                                            c[x-1][y]='_';mvprintw(y,x-1,"_");
                                        }
                                        moveplayer(x,y,c);
                                    }
                                }

                            
                            }  
                        }
                    }
                }
                mvprintw(y,x,"%c",c[x][y]);
                moveplayer(x-1,y,c);

                //if(c[x][y-1]=='>') 
        case 'e':
                if(c[x+1][y-1]=='|' || c[x+1][y-1]=='_' || c[x+1][y-1]=='O'){
                    if((x+1==hidendoor[0].x && y-1==hidendoor[0].y)||(x+1==hidendoor[1].x && y-1==hidendoor[1].y)){
                        c[x+1][y-1]='?';
                        mvprintw(y,x,"%c",c[x][y]);
                        moveplayer(x+1,y-1,c);
                    }
                    else{
                        moveplayer(x,y,c);
                    }
                }
                if(c[x+1][y-1]==' ' ||c[x+1][y-1]=='D' || c[x+1][y-1]=='U' || c[x+1][y-1]=='G' || c[x+1][y-1]=='S' || c[x+1][y-1]=='F' ||
                    (c[x+1][y]=='|' && c[x][y-1]=='_')){
                    moveplayer(x,y,c);
                }
                mvprintw(y,x,"%c",c[x][y]);
                moveplayer(x+1,y-1,c);

                //if(c[x][y-1]=='@')
                //if(c[x][y-1]=='key')
                //if(c[x][y-1]=='>') 
        case 'q':
                if(c[x-1][y-1]=='|' || c[x-1][y-1]=='_' || c[x-1][y-1]=='O'){
                    if((x-1==hidendoor[0].x && y-1==hidendoor[0].y)||(x-1==hidendoor[1].x && y-1==hidendoor[1].y)){
                        c[x-1][y-1]='?';
                        mvprintw(y,x,"%c",c[x][y]);
                        moveplayer(x-1,y-1,c);
                    }
                    else{
                        moveplayer(x,y,c);
                    }
                }
                if(c[x-1][y-1]==' ' ||c[x-1][y-1]=='D' || c[x-1][y-1]=='U' || c[x-1][y-1]=='G' || c[x-1][y-1]=='S' || c[x-1][y-1]=='F'
                   || (c[x-1][y]=='|' && c[x][y-1]=='_')){
                    moveplayer(x,y,c);
                }
                mvprintw(y,x,"%c",c[x][y]);
                moveplayer(x-1,y-1,c);

                //if(c[x][y-1]=='@')
                //if(c[x][y-1]=='key')
                //if(c[x][y-1]=='>') 
        case 'x':
                if(c[x+1][y+1]=='|' || c[x+1][y+1]=='_' || c[x+1][y+1]=='O'){
                    if((x+1==hidendoor[0].x && y+1==hidendoor[0].y)||(x+1==hidendoor[1].x && y+1==hidendoor[1].y)){
                        c[x][y-1]='?';
                        mvprintw(y,x,"%c",c[x][y]);
                        moveplayer(x+1,y+1,c);
                    }
                    else{
                        moveplayer(x,y,c);
                    }
                }
                if(c[x+1][y+1]==' ' ||c[x+1][y+1]=='D' || c[x+1][y+1]=='U' || c[x+1][y+1]=='G' || c[x+1][y+1]=='S' || c[x+1][y+1]=='F'
                || ( c[x+1][y]=='|' && c[x][y+1]=='_')){
                    moveplayer(x,y,c);
                }
                mvprintw(y,x,"%c",c[x][y]);
                moveplayer(x+1,y+1,c);

                //if(c[x][y-1]=='@')
                //if(c[x][y-1]=='key')
                //if(c[x][y-1]=='>') 
        case 'z':
                if(c[x-1][y+1]=='|' || c[x-1][y+1]=='_' || c[x-1][y+1]=='O'){
                    if((x-1==hidendoor[0].x && y+1==hidendoor[0].y)||(x-1==hidendoor[1].x && y+1==hidendoor[1].y)){
                        c[x-1][y+1]='?';
                        mvprintw(y,x,"%c",c[x][y]);
                        moveplayer(x-1,y+1,c);
                    }
                    else{
                        moveplayer(x,y,c);
                    }
                }
                if(c[x-1][y+1]==' ' ||c[x-1][y+1]=='D' || c[x-1][y+1]=='U' || c[x-1][y+1]=='G' || c[x-1][y+1]=='S' || c[x-1][y+1]=='F'
                    || (c[x-1][y]=='|' && c[x][y+1]=='_')){
                    moveplayer(x,y,c);
                }
                mvprintw(y,x,"%c",c[x][y]);
                moveplayer(x-1,y+1,c);

                //if(c[x][y-1]=='@')
                //if(c[x][y-1]=='key')
                //if(c[x][y-1]=='>')
            default:
            moveplayer(x,y,c);
            break;    
                
                               
    }
           
            
                         

}



void start(){
    refresh();
    int t=4;
    //Nowweapon nowweapon;
    nowweapon.is_Mace=1;
    weapon.num_Dagger=0; nowweapon.is_Dagger=0;
    weapon.num_Magic_Wand=0; nowweapon.is_Magic_Wand=0;
    weapon.num_Normal_Arrow=0; nowweapon.is_Normal_Arrow=0;
    weapon.num_Sword=0; nowweapon.is_Sword=0;
    
    srandom(time(NULL));
    while(t!=0){
    int s= random();
    int X=s%(room[0].width-2)+room[0].x+1;
    int Y=s%(room[0].hight -2)+room[0].y+1;
    if(c[X][Y]=='.'){
        time(&game.startgame);
        flor.floor=1;
        health.health=150;
        //getch();
        //clear(); refresh();
        //getch();
    moveplayer(X,Y,c);
    t--;
    }
    }
}