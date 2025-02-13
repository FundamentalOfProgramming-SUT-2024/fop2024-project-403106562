#include<ncurses.h>
#include<string.h>
#include"map.h"
#include"move.h"
#include"setting.h"
#include"scoreboard.h"
#include"load.h"
#include"p.h"

int choice=0;

void menu(){
    initscr();
    noecho();
    keypad(stdscr,TRUE);
    curs_set(0);

    char c[4][100]={"Play a new game","Play last game","Settings","Scoreboard"};
    for(int i=0;i<4;i++){
        printw("%s\n",c[i]);
    }
    //printw("%d",KEY_UP);
    char ch='q';
    
    start_color();
    init_pair(1,COLOR_RED,COLOR_WHITE);
    attron(COLOR_PAIR(1));
    mvprintw(0,0,"%s",c[0]);
    attroff(COLOR_PAIR(1));
    
    while(ch!='\n'){
        ch=getch();
     
     if((ch=='w'||ch=='W') && choice==0){
        attron(COLOR_PAIR(1));
        mvprintw(3,0,"%s",c[3]);
        attroff(COLOR_PAIR(1));
        mvprintw(0,0,"%s",c[0]);
        choice=3;
        continue;
     }
     if((ch=='s'||ch=='S') && choice==3){
        attron(COLOR_PAIR(1));
        mvprintw(0,0,"%s",c[0]);
        attroff(COLOR_PAIR(1));
        mvprintw(3,0,"%s",c[3]);
        choice=0;
        continue;
     }
     if(ch=='w'||ch=='W'){
        attron(COLOR_PAIR(1));
        mvprintw(choice-1,0,"%s",c[choice-1]);
        refresh();
        attroff(COLOR_PAIR(1));
        mvprintw(choice,0,"%s",c[choice]);
        choice=choice-1;
        continue;
     }
     if(ch=='s'||ch=='S'){
        attron(COLOR_PAIR(1));
        mvprintw(choice+1,0,"%s",c[choice+1]);
        refresh();
        attroff(COLOR_PAIR(1));
        mvprintw(choice,0,"%s",c[choice]);
        choice=choice+1;
        continue;
     }


    }
   if(choice==0){
      clear();
      map(TIME);
      start();
   }
   if(choice==1){
      load();
   }

   if(choice==2){
      clear();
      setting();
   }
   if(choice==3){
      clear();
      refresh();
      scoreboard();
   } 
    

}