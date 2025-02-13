#include<ncurses.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include"move.h"


typedef struct{
    int x;int y;
}Point;

typedef struct{
    int rear;
    int front;
    Point points[7000];
}Queue;

typedef struct{
    int x;
    int y;
    int width;//x
    int hight;//y
}ROOM;
ROOM room[20];

typedef struct{
    int x1,y1;//door1
    int x2,y2;//door2
}DOOR;
DOOR door[40];

typedef struct{
    int x;
    int y;
}Hidendoor;
Hidendoor hidendoor[2];

typedef struct{
    int n;
}Spellroom;
Spellroom spellroom;

typedef struct{
    int x; int y;
}Trap;
Trap trap[2];

typedef struct{
    int x;int y;int width;int hight;
}Scaperoom;
Scaperoom scaperoom;

typedef struct{
    int x;int y;
}LockedDoor;
LockedDoor  lockeddoor;

typedef struct{
    int health;
    int depth;
    int room;
    int x;
    int y;
}Diamone;
Diamone diamone;

typedef struct{
    int health;
    int depth;
    int room;
    int x;
    int y;
}Fire;
Fire fire;

typedef struct{
    int health;
    int depth;
    int room;
    int x;
    int y;
}Giant;
Giant giant;

typedef struct{
    int health;
    char stablish;
    int room;
    int x;
    int y;
}Snake;
Snake snake;


typedef struct{
    int health;
    int depth;
    int room;
    int x;
    int y;
}Undeed;
Undeed undeed;

typedef struct{
    int x;
    int y;
    int num;
}Spellfood;
Spellfood spellfood[2];

typedef struct{
    int x;
    int y;
    int num;
}Damage;
Damage damage[2];

int daggerx;int daggery;
int magicx; int magicy;
int normalx;int normaly;

void Coordinates_doors(int l,DOOR*door,ROOM*room){
    srand(time(NULL));
    int o=rand();
    
    int t_x=o%(room[l].width -2)+room[l].x+1;
    door[l].x1=t_x;

    if(o%2==0){
    door[l].y1=room[l].y;}
    else{
    door[l].y1=room[l].y+room[l].hight-1;
    }

    //srand(time(NULL));
    o=rand();
    
    int t_y=o%(room[l].hight -2)+room[l].y+1;
    door[l].y2=t_y;

    if(o%2==0){
    door[l].x2=room[l].x;}
    else{
    door[l].x2=room[l].x+room[l].width-1;
    }

}


int Buildhall(int x,int y,char c[300][300],Point start,Point end,Queue queue,Point parent[300][300]){
    
    int seen[300][300];
    for(int i=0;i<300;i++){
       for(int j=0;j<300;j++){
        seen[i][j]=0;
       }
    }
  
    seen[start.x][start.y]=1;

    while(queue.rear!=queue.front){
        Point current=queue.points[queue.front];
        queue.front++;
        if(current.x==end.x && current.y==end.y){
            //mvprintw(1,0,"%d",start.x);
            return 1;
        }

        int ix[4]={1,0,0,-1};
        int jy[4]={0,1,-1,0};
        for(int i=0;i<4;i++){
            int lx=current.x +ix[i];
            int ly=current.y +jy[i];
            if(lx>=0 && ly>=0){
            if(seen[lx][ly]==0  && (lx<=x) && (ly>=7 || lx>62) &&(ly<=y) && (c[lx][ly]==' ' || c[lx][ly]=='#')){
            parent[lx][ly]=current;
            seen[lx][ly]=1;
            Point new; new.x=lx; new.y=ly;
            queue.points[queue.rear]=new;
            queue.rear++;}
            }
        }


    }
    return 0;

   
}

void drawhall(Point parent[300][300], Point start, Point end,char c[300][300],int x,int y){
    Point current;
    current.x=end.x;current.y=end.y;
    //mvprintw(1,0,"%d",start.x);
    //mvprintw(2,0,"%d %d",end.x,end.y);

    while(current.x!=start.x || current.y!=start.y){
        c[current.x][current.y]='#';
        current=parent[current.x][current.y];
    }
    c[start.x][start.y]='#';
}

void hiden_door(int l,char c[300][300], DOOR*door){
    srandom(time(NULL));
    int m=random()%(l-3)+1;
    c[door[m].x2][door[m].y2]='|';
    c[door[m].x1][door[m].y1]='_';

    hidendoor[0].x=door[m].x2; hidendoor[0].y=door[m].y2;
    hidendoor[1].x=door[m].x1; hidendoor[1].y=door[m].y1;

    spellroom.n=m+1;
}

void trap_(int l,char c[300][300],ROOM*room){
    srandom(time(NULL));
    while(1){
    int trap1=random()%(l-1)+1;//the room that trap exist
    int coordinate=random();
    int x=coordinate%(room[trap1].width-1) + room[trap1].x;
    int y=coordinate%(room[trap1].hight-1) + room[trap1].y;
    if(c[x][y]=='.'){
        trap[0].x=x; trap[0].y=y;
        c[x][y]='.';
         break;}
    continue;         
    }
    while(1){
    int trap2=random()%l;//the room that trap exist
    int coordinate=random();
    int x=coordinate%(room[trap2].width-1) + room[trap2].x;
    int y=coordinate%(room[trap2].hight-1) + room[trap2].y;
    if(c[x][y]=='.'){
        trap[1].x=x; trap[1].y=y;
        c[x][y]='$';
         break;}
    continue;         
    }

}

void Ancient_Key(int l,char c[300][300],ROOM*room){
    srand(time(NULL));
    while(1){
    int key=random()%l;//the room that key exist in it
    int coordinate=random();
    int x=coordinate%(room[key].width-1) + room[key].x;
    int y=coordinate%(room[key].hight-1) + room[key].y;
    if(c[x][y]=='.'){
        c[x][y]='k';
        break;
    }
}
}

void stair(int l,char c[300][300],ROOM*room){
    srand(time(NULL));
    while(1){
    int st=random()%(l-2)+1;//the room that key exist in it
    int coordinate=random();
    int x=coordinate%(room[st].width-1) + room[st].x;
    int y=coordinate%(room[st].hight-1) + room[st].y;
    if(c[x][y]=='.'){
        c[x][y]='<';
        scaperoom.x=room[st].x;scaperoom.y=room[st].y;
        scaperoom.width=room[st].width; scaperoom.hight=room[st].hight;
        break;
    }
}
}

void passroom(int m, char c[300][300],ROOM*room){
    srandom(time(NULL));
    while(1){
    int j=rand();
    int x=j%(room[m].width -2)+room[m].x;
    int y=j%(room[m].hight-2)+room[m].y;
    if(c[x][y]=='.'){
        c[x][y]='&';
        break;
    }
}
}

void encrypteddoor(int l, char c[300][300],DOOR*door){
    srandom(time(NULL));
    while(1){
        int m=random()%(l-2)+1;//the room that the door is locked
        int h=random();
        if(h%2==0){
            if(c[door[m].x1][door[m].y1]=='+'){
                c[door[m].x1][door[m].y1]='@';
                passroom(m,c,room);
                break;
            }
            else if(c[door[m].x2][door[m].y2]=='+'){
                c[door[m].x2][door[m].y2]='@';
                passroom(m,c,room);
                break;
            }
        }
        if(h%2==1){
            if(c[door[m].x2][door[m].y2]=='+'){
                c[door[m].x2][door[m].y2]='@';
                passroom(m,c,room);
                break;
            }
            else if(c[door[m].x1][door[m].y1]=='+'){
                c[door[m].x1][door[m].y1]='@';
                passroom(m,c,room);
                break;
            }
        }
    }
    while(1){
        int m=random()%l;//the room that the door is locked
        int h=random();
        if(h%2==0){
            if(c[door[m].x1][door[m].y1]=='+'){
                c[door[m].x1][door[m].y1]='@';
                passroom(m,c,room);
                break;
            }
            else if(c[door[m].x2][door[m].y2]=='+'){
                c[door[m].x2][door[m].y2]='@';
                passroom(m,c,room);
                break;
            }
        }
        if(h%2==1){
            if(c[door[m].x2][door[m].y2]=='+'){
                c[door[m].x2][door[m].y2]='@';
                passroom(m,c,room);
                break;
            }
            else if(c[door[m].x1][door[m].y1]=='+'){
                c[door[m].x1][door[m].y1]='@';
                passroom(m,c,room);
                break;
            }
        }
    }


}

void FOOD(int l,char c[300][300],ROOM*room){
    int number =0;
    srand(time(NULL));
    while(number<3){
        int m=rand()%(l-2)+1;// the room that food exist
        int x=rand()%(room[m].width-2)+room[m].x;
        int y=rand()%(room[m].hight-2)+room[m].y;
        if(c[x][y]=='.'){
            c[x][y]='$';
            number++;
        }

    }
}

void Diamonposition(int l, char c[300][300], ROOM*room, char backup[300][300]){
    srandom(time(NULL));

    while(1){
        int position_d=random()%(l-1)+1;
        int x=rand()%(room[position_d].width-1) + room[position_d].x;
        int y=rand()%(room[position_d].hight-1) + room[position_d].y;
        if(c[x][y]=='.'){
            c[x][y]='D';
            backup[x][y]='.';
            diamone.room=position_d;
            diamone.x=x; diamone.y=y;
            diamone.depth=0;
            diamone.health=5;
            break;
        }
        
    }
}

void Fire_Breathing_position(int l, char c[300][300],ROOM*room, char backup[300][300]){
     srandom(time(NULL));
    
    while(1){
        int position_d=random()%(l-1)+1;
        int x=rand()%(room[position_d].width-1) + room[position_d].x;
        int y=rand()%(room[position_d].hight-1) + room[position_d].y;
        if(c[x][y]=='.'){
            c[x][y]='F';
            backup[x][y]='.';
            fire.room=position_d;
            fire.x=x; fire.y=y;
            fire.depth=0;
            fire.health=10;
            break;
        }        
    }

}

void Giantposition(int l, char c[300][300],ROOM*room, char backup[300][300]){
     srandom(time(NULL));

    while(1){
        int position_d=random()%(l-1)+1;
        int x=rand()%(room[position_d].width-1) + room[position_d].x;
        int y=rand()%(room[position_d].hight-1) + room[position_d].y;
        if(c[x][y]=='.'){
            c[x][y]='G';
            backup[x][y]='.';
            giant.room=position_d;
            giant.x=x; giant.y=y;
            giant.depth=0;
            giant.health=15;
            break;
        }        
    }

}

void Snakeposition(int l, char c[300][300],ROOM*room, char backup[300][300]){
     srandom(time(NULL));
    
    while(1){
        int position_d=random()%(l-1)+1;
        int x=rand()%(room[position_d].width-1) + room[position_d].x;
        int y=rand()%(room[position_d].hight-1) + room[position_d].y;
        if(c[x][y]=='.'){
            c[x][y]='S';
            backup[x][y]='.';
            //Snake *snake=(Snake*)malloc(sizeof(Snake));
            snake.room=position_d;
            snake.x=x; snake.y=y;
            snake.stablish='.';
            snake.health=20;
            break;
        }        
    }

}

void Undeedposition(int l, char c[300][300],ROOM*room, char backup[300][300]){
     srandom(time(NULL));

    while(1){
        int position_d=random()%(l-1)+1;
        int x=rand()%(room[position_d].width-1) + room[position_d].x;
        int y=rand()%(room[position_d].hight-1) + room[position_d].y;
        if(c[x][y]=='.'){
            c[x][y]='U';
            backup[x][y]='.';
            undeed.room=position_d;
            undeed.x=x; undeed.y=y;
            undeed.depth=0;
            undeed.health=30;
            break;
        }        
    }

}

void Dagger(int l,char c[300][300],ROOM*room){
    srandom(time(NULL));
    while(1){
        int daggerp=random()%(l-1)+1;
        int x=rand()%(room[daggerp].width-1) + room[daggerp].x;
        int y=rand()%(room[daggerp].hight-1) + room[daggerp].y;
        if(c[x][y]=='.'){
            daggerx=x; daggery=y;
            c[x][y]='d';            
            break;
        }        
    }
}

void Magic_Wand(int l,char c[300][300],ROOM*room){
    srandom(time(NULL));
    while(1){
        int Magicp=random()%(l-1)+1;
        int x=rand()%(room[Magicp].width-1) + room[Magicp].x;
        int y=rand()%(room[Magicp].hight-1) + room[Magicp].y;
        if(c[x][y]=='.'){
            magicx=x;magicy=y;
            c[x][y]='m';            
            break;
        }        
    }
}

void Normal_Arrow(int l,char c[300][300],ROOM*room){
    srandom(time(NULL));
    while(1){
        int Normalp=random()%(l-1)+1;
        int x=rand()%(room[Normalp].width-1) + room[Normalp].x;
        int y=rand()%(room[Normalp].hight-1) + room[Normalp].y;
        if(c[x][y]=='.'){
            normalx=x; normaly=y;
            c[x][y]='n';            
            break;
        }        
    }
}

void Sword(int l,char c[300][300],ROOM*room){
    srandom(time(NULL));
    while(1){
        int swordp=random()%(l-1)+1;
        int x=rand()%(room[swordp].width-1) + room[swordp].x;
        int y=rand()%(room[swordp].hight-1) + room[swordp].y;
        if(c[x][y]=='.'){
            c[x][y]='s';            
            break;
        }        
    }
}

void Coin(int l, char c[300][300],ROOM*room){
    int number =0;
    srand(time(NULL));
    while(number<5){
        int m=rand()%(l-2)+1;// the room that food exist
        int x=rand()%(room[m].width-2)+room[m].x;
        int y=rand()%(room[m].hight-2)+room[m].y;
        if(c[x][y]=='.'){
            c[x][y]='C';
            number++;
        }

    }

}

void column(int l, char c[300][300],ROOM*room){
int number =0;
    srand(time(NULL));
    while(number<5){
        int m=rand()%(l-2)+1;// the room that food exist
        int x=rand()%(room[m].width-2)+room[m].x;
        int y=rand()%(room[m].hight-2)+room[m].y;
        if(c[x][y]=='.' && c[x+1][y]!='+' && c[x+1][y]!='@' && c[x][y-1]!='+' && c[x][y-1]!='@' && c[x-1][y]!='+' && c[x-1][y]!='@' && c[x][y+1]!='+' && c[x][y+1]!='@'){
            c[x][y]='O';
            number++;
        }

    }
} 

void spell(int l,char c[300][300],ROOM*room){
     int number =0;
    srand(time(NULL));
    while(number<4){
        int m=rand()%(l-2)+1;
        int x=rand()%(room[m].width-2)+room[m].x;
        int y=rand()%(room[m].hight-2)+room[m].y;
        if(c[x][y]=='.'){
            if(number==0 || number==1){
                spellfood[number].x=x; spellfood[number].y=y;
            }
            if(number==2 || number==3){
                damage[number-2].x=x; damage[number-2].y=y;
            }
            c[x][y]='(';
            number++;
        }

    }
}

char c[300][300];
char backup[300][300];
int mark[300][300];

void map( time_t TIME){

    
    refresh();
    int x,y;
    getmaxyx(stdscr,y,x);
    //getch();
    
    for(int i=0;i<x;i++){
        for(int j=0;j<y;j++){
            c[i][j]=' ';
            mark[i][j]=0;
        }
    }
    srand(time(NULL));
    int random_m=rand()%4 +6;//number of rooms
    
    int randomcpy=random_m;

   
    int l=0;
    //getch();
    
    while(random_m!=0){   
        int random_x=rand()%4 +6;
        int random_y=rand()%4 +5;
        //mvprintw(0,0,"%d %d" ,random_x,random_y);
        //getch();
        int row_x=rand()%(x-5)+2;

        int column_y=rand()%(y-3)+2;
        //mvprintw(0,0,"%d %d %d",random_m,random_x,random_y);
        if(row_x <63 && column_y<8){
            continue;
        }
        if(random_x+row_x>=x-2){
            continue;
        }
        if(random_y+column_y>=y-4){
            continue;
        }
        int t=0;
        for(int i=row_x;i<random_x+row_x;i++){
            for(int j=column_y;j<random_y+column_y;j++){
                if(c[i][j]=='.' || c[i][j]=='|' || c[i][j]=='_' || c[i][j]=='+' || c[i][j]=='#'){
                  t=1;
                  break;
                }
            }
        }
        if(t==1){
            continue;// for not print;
        }
        for(int i=row_x;i<random_x+row_x;i++){
            for(int j=column_y;j<random_y+column_y;j++){
                if(i==row_x){
                    if(j!=column_y){
                    c[i][j]='|';}
                }
                else if(i==row_x+random_x-1){
                    if(j!=column_y){
                    c[i][j]='|';}
                }
                else{
                    if(j==column_y || j==random_y+column_y-1){
                        c[i][j]='_';
                    }
                    else{                        
                        c[i][j]='.';
                    }
                }


            }
        }
        room[l].x=row_x;
        room[l].y=column_y;
        room[l].width=random_x;
        room[l].hight=random_y;
        Coordinates_doors(l,door,room);
      

        c[door[l].x1][door[l].y1]='+';
        c[door[l].x2][door[l].y2]='+';
        

        //mvprintw(door[l].y1,door[l].x1,"%c",'+');
        //mvprintw(door[l].y2,door[l].x2,"%c",'+');
        if(l>=1){

            if(door[l-1].x2==room[l-1].x && door[l].y1==room[l].y){
                
                Point parent[300][300]={{-1,-1}};
                Point start; start.x=door[l-1].x2-1; start.y=door[l-1].y2;
                //mvprintw(0,0,"%d",start.x);
                Point end; end.x=door[l].x1; end.y=door[l].y1-1;
                Queue queue;
                queue.rear=0;queue.front=0;
                queue.points[queue.rear]=start;
                queue.rear++;
                
                if(Buildhall(x,y,c,start,end,queue,parent)){ 
                drawhall(parent,start,end,c,x,y);}
                else{
                     for(int i=0;i<300;i++){
                        for(int j=0;j<300;j++){
                            c[i][j]=' ';
                        }
                     }
                     random_m=randomcpy; l=0;
                     continue;
                }
                
                }
                   
            
            else if(door[l-1].x2==room[l-1].x && door[l].y1==room[l].y+room[l].hight-1){
               
                Point parent[300][300]={{-1,-1}};
                Point start;start.x=door[l-1].x2-1;start.y=door[l-1].y2;
                //mvprintw(0,0,"%d",start.x);
                Point end; end.x=door[l].x1 ;end.y=door[l].y1+1;
                Queue queue;
                queue.rear=0;queue.front=0;
                queue.points[queue.rear]=start;
                queue.rear++;
                if(Buildhall(x,y,c,start,end,queue,parent)){
                drawhall(parent,start,end,c,x,y);}
                else{
                     for(int i=0;i<300;i++){
                        for(int j=0;j<300;j++){
                            c[i][j]=' ';
                        }
                     }
                     random_m=randomcpy; l=0;
                     continue;
                }
                
            }
            else if(door[l-1].x2==room[l-1].x+room[l-1].width-1 && door[l].y1==room[l].y){
               
                Point parent[300][300]={{-1,-1}};
                Point start; start.x=door[l-1].x2+1 ; start.y=door[l-1].y2;
                //mvprintw(0,0,"%d",start.x);
                Point end; end.x=door[l].x1; end.y=door[l].y1-1;
                Queue queue;
                queue.rear=0;queue.front=0;
                queue.points[queue.rear]=start;
                queue.rear++;
                if(Buildhall(x,y,c,start,end,queue,parent)){
                drawhall(parent,start,end,c,x,y);}
                else{
                     for(int i=0;i<300;i++){
                        for(int j=0;j<300;j++){
                            c[i][j]=' ';
                        }
                     }
                     random_m=randomcpy; l=0;
                     continue;
                }
            }
            else if(door[l-1].x2==room[l-1].x+room[l-1].width-1 && door[l].y1==room[l].y+room[l].hight-1){
                Point parent[300][300]={{-1,-1}};
                Point start; start.x=door[l-1].x2+1; start.y=door[l-1].y2;
                //mvprintw(0,0,"%d",start.x);
                Point end; end.x=door[l].x1; end.y=door[l].y1+1;
                Queue queue;
                queue.rear=0;queue.front=0;
                queue.points[queue.rear]=start;
                queue.rear++;
                if(Buildhall(x,y,c,start,end,queue,parent)){
                drawhall(parent,start,end,c,x,y);}
                else{
                     for(int i=0;i<300;i++){
                        for(int j=0;j<300;j++){
                            c[i][j]=' ';
                        }
                     }
                     random_m=randomcpy; l=0;
                     continue;
                }
            }
        }
        
             
        random_m--;
        l++;}




c[door[0].x1][door[0].y1]='_';
c[door[l-1].x2][door[l-1].y2]='|';

hiden_door(l,c,door);
Ancient_Key(l,c,room);
stair(l,c,room);
encrypteddoor(l,c,door);
FOOD(l,c,room);

Dagger(l,c,room);

Magic_Wand(l,c,room);

Normal_Arrow(l,c,room);

Sword(l,c,room);

Coin(l,c,room);

column(l,c,room);

trap_(l,c,room);

spell(l,c,room);

 for(int i=room[0].x ; i<room[0].x+room[0].width; i++){
     for(int j=room[0].y ; j<room[0].y+room[0].hight; j++){
         mvprintw(j,i,"%c",c[i][j]);
         mark[i][j]=1;
     }

 }
for(int i=0;i<x;i++){
    for(int j=0;j<y;j++){
        //mvprintw(j,i,"%c",c[i][j]);
        backup[i][j]=c[i][j];
    }
}

Diamonposition(l,c,room,backup);

Fire_Breathing_position(l,c,room,backup);

Giantposition(l,c,room,backup);

Snakeposition(l,c,room,backup);

Undeedposition(l,c,room,backup);




}