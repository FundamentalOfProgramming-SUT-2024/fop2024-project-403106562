#include<ncurses.h>
#include<string.h>
#include"p.h"
#include"p2.h"
#include<locale.h>
#include<wchar.h>

void scoreboard(){
    //initscr();
    refresh();
    WINDOW*win=newwin(17,60,10,50);
    box(win,0,0);
    FILE*file=fopen("gold.txt","r");
    int rotbe=1;
    char line[100];
    setlocale(LC_ALL, "");
    keypad(stdscr,TRUE);
    
    

    while(fgets(line,sizeof(line),file)!=NULL){
        start_color();
        init_pair(1,COLOR_YELLOW,COLOR_BLACK);
        init_pair(2, COLOR_WHITE,COLOR_BLACK);
        init_pair(3,COLOR_BLUE,COLOR_BLACK);
        line[strcspn(line,"\n")]='\0';
        char *word=strtok(line,":");
        char *g=strtok(NULL,":");
        char*Nh=strtok(NULL,":");

        if(rotbe<11){
            if(rotbe==1){
                wattron(win,COLOR_PAIR(1)); 
            }
            else if(rotbe==2){
                wattron(win,COLOR_PAIR(2));
            }
            else if(rotbe==3){
                wattron(win,COLOR_PAIR(3));
            }
        if(rotbe<4){mvwprintw(win,rotbe,3,"%d- Name: %s  Gold:%s  Games:%s \U0001F3C5",rotbe,word,g,Nh);wrefresh(win);}
        else{mvwprintw(win,rotbe,3,"%d- Name: %s  Gold:%s Games:%s",rotbe,word,g,Nh);wrefresh(win);}
        wattroff(win,COLOR_PAIR(rotbe));
        }
        if(!strcmp(hero.name,word)){
         if(rotbe<11){mvwprintw(win,rotbe,3,"%d- Name: %s  Gold:%s Games:%s ",rotbe,word,g,Nh);wrefresh(win);}
         mvwprintw(win,12,3,"%d- Name: %s  Gold:%s  Games:%s  %c",rotbe,word,g,Nh,'&');wrefresh(win);
        }
        rotbe++;

    }
    fclose(file);
    mvwprintw(win,14,3,"press enter for continue...");wrefresh(win);
    getch();
    werase(win); wrefresh(win);
    clear();
    menu();
}