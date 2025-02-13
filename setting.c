#include<ncurses.h>
#include"move.h"
#include "p2.h"
#include"p.h"

int r=0;

void setting(){
    initscr();
    noecho();
    keypad(stdscr,TRUE);
    curs_set(0);
    char c[2][100]={"Game Mode Menu","color of hero"};
    for(int i=0;i<2;i++){
        printw("%s\n",c[i]);
    }
    char ch='q';
    start_color();
    init_pair(1,COLOR_RED,COLOR_WHITE);
    attron(COLOR_PAIR(1));
    mvprintw(0,0,"%s",c[0]);
    attroff(COLOR_PAIR(1));
    
     while(ch!='\n'){
        ch=getch();
     
     if((ch=='w'||ch=='W') && r==0){
        attron(COLOR_PAIR(1));
        mvprintw(1,0,"%s",c[1]);
        attroff(COLOR_PAIR(1));
        mvprintw(0,0,"%s",c[0]);
        r=1;
        continue;
     }
     if((ch=='s'||ch=='S') && r==1){
        attron(COLOR_PAIR(1));
        mvprintw(0,0,"%s",c[0]);
        attroff(COLOR_PAIR(1));
        mvprintw(1,0,"%s",c[1]);
        r=0;
        continue;
    }
     if(ch=='w'||ch=='W'){
        attron(COLOR_PAIR(1));
        mvprintw(r-1,0,"%s",c[r-1]);
        refresh();
        attroff(COLOR_PAIR(1));
        mvprintw(r,0,"%s",c[r]);
        r=r-1;
        continue;
    }
     if(ch=='s'||ch=='S'){
        attron(COLOR_PAIR(1));
        mvprintw(r+1,0,"%s",c[r+1]);
        refresh();
        attroff(COLOR_PAIR(1));
        mvprintw(r,0,"%s",c[r]);
        r=r+1;
        continue;
    }
    }
    clear();


if(r==0){
        char c[3][10]={"Easy","Normal","Hard"};
        for(int i=0;i<3;i++){
            printw("%s\n",c[i]);
        }
        ch='q';
        int cursor1=0;  
    while(ch!='\n'){

        ch=getch();    
    if((ch=='w'||ch=='W') && cursor1==0){
        attron(COLOR_PAIR(1));
        mvprintw(2,0,"%s",c[2]);
        attroff(COLOR_PAIR(1));
        mvprintw(0,0,"%s",c[0]);
        cursor1=2;
        continue;
     }
     if((ch=='s'||ch=='S') && cursor1==2){
        attron(COLOR_PAIR(1));
        mvprintw(0,0,"%s",c[0]);
        attroff(COLOR_PAIR(1));
        mvprintw(2,0,"%s",c[2]);
        cursor1=0;
        continue;
    }
     if(ch=='w'||ch=='W'){
        attron(COLOR_PAIR(1));
        mvprintw(cursor1-1,0,"%s",c[cursor1-1]);
        refresh();
        attroff(COLOR_PAIR(1));
        mvprintw(cursor1,0,"%s",c[cursor1]);
        cursor1=cursor1-1;
        continue;
    }
     if(ch=='s'||ch=='S'){
        attron(COLOR_PAIR(1));
        mvprintw(cursor1+1,0,"%s",c[cursor1+1]);
        refresh();
        attroff(COLOR_PAIR(1));
        mvprintw(cursor1,0,"%s",c[cursor1]);
        cursor1=cursor1+1;
        continue;
    }
    }
    
    if(cursor1==0){
        TIME=1000;
        clear();
        menu();
    }
    if(cursor1==1){
        TIME=1000;
        clear();
        menu();
    }
    if(cursor1==2){
        TIME=1000;
        clear();
        menu();
    }

    }


if(r==1){
    char c[2][10]={"Blue","green"};
    for(int i=0;i<2;i++){
        printw("%s\n",c[i]);
    }
    ch='q';
    int cursor2=0;
    while(ch!='\n'){

        ch=getch();    
    if((ch=='w'||ch=='W') && cursor2==0){
        attron(COLOR_PAIR(1));
        mvprintw(1,0,"%s",c[1]);
        attroff(COLOR_PAIR(1));
        mvprintw(0,0,"%s",c[0]);
        cursor2=1;
        continue;
     }
     if((ch=='s'||ch=='S') && cursor2==1){
        attron(COLOR_PAIR(1));
        mvprintw(0,0,"%s",c[0]);
        attroff(COLOR_PAIR(1));
        mvprintw(1,0,"%s",c[1]);
        cursor2=0;
        continue;
    }
     if(ch=='w'||ch=='W'){
        attron(COLOR_PAIR(1));
        mvprintw(cursor2-1,0,"%s",c[cursor2-1]);
        refresh();
        attroff(COLOR_PAIR(1));
        mvprintw(cursor2,0,"%s",c[cursor2]);
        cursor2=cursor2-1;
        continue;
    }
     if(ch=='s'||ch=='S'){
        attron(COLOR_PAIR(1));
        mvprintw(cursor2+1,0,"%s",c[cursor2+1]);
        refresh();
        attroff(COLOR_PAIR(1));
        mvprintw(cursor2,0,"%s",c[cursor2]);
        cursor2=cursor2+1;
        continue;
    }
    }
    
    colorhero=cursor2;
    clear();
    menu();

}

}
