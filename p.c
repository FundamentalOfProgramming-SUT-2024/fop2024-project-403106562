#include<ncurses.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"p2.h"
#include"map.h"
#include"move.h"
#include"setting.h"
#include<locale.h>
#include<SDL2/SDL_mixer.h>
#include<time.h>

typedef struct{
    char name[50];
    int gold;
}Hero;
Hero hero;


int check_User( char User[100],FILE*file){
      
      char a[100];
	  //char*b=(char*)malloc(100*sizeof(char));
      while(fgets(a,sizeof(a),file) !=NULL){
		char*b=strtok(a, ":");
        if(strcmp(User,b)==0){
            
            return 1;
        }

      }
      return 0;
}

int check_Pass(char Pass[100]){
    if(strlen(Pass)<7){
        return 0;
    }
    int small=0; int big=0; int number=0;
    for(int i=0;i<strlen(Pass);i++){
        if(Pass[i]>47 && Pass[i]<58){
            number++;
        }
        if(Pass[i]>64 && Pass[i]<91){
            big=big+1;
        }
        if(Pass[i]>96 && Pass[i]<123){
            small++;
        }
    }
    if(small==0 || big==0 || number==0){
        return 0;
    }

    return 1;
}

int check_Email(char email[100]){
    int point =0; int adsighn=0;
    for(int i=0;i<strlen(email);i++){
        if(email[i]=='@'){
            adsighn=i;
        }
        if(email[i]=='.'){
            point=i;
        }
    }
    if(adsighn==0 || point==0){
        return 1;
    }
    if(adsighn-point>=-1){
        return 1;
    }
    if(strlen(email)-point!=4){
        return 1;
    }

    return 0;
}

void Write_file(FILE*file,char User[100],char Pass[100],char email[100]){
    //fprintf(file,"\n");

    fprintf(file,"%s:%s:%s\n",User,Pass,email);
    return;
}

int check_Pass_User(char User[100],char Pass[100],FILE*file){
    char line[100];
   

    while(fgets(line,sizeof(line),file)!=NULL){
        char*u=strtok(line, ":");
        char*p=strtok(NULL, ":");
        if(strcmp(User,u)==0 && strcmp(Pass,p)==0){
            return 1;
        }
    }
    

    return 0;

}





void main(){
     
    srand(time(NULL));
    setlocale(LC_ALL,"");
    setlocale(LC_CTYPE,"");
    raw(); keypad(stdscr, TRUE);
    FILE*GOld=fopen("gold.txt","a");
    
    FILE*file=fopen("data.txt","a");
    if(file==NULL && GOld==NULL){
	char User1[100]="mrk1384";
	char Pass1[100]="275Messi";
	char email1[100]="koohestanimohamadreza0@gmail.com";
	fprintf(file,"%s:%s:%s",User1,Pass1,email1);
    fprintf(GOld,"%s:%d\n",User1,0);
    fclose(GOld);
	fclose(file);}

	
     
    initscr();
    //SDL_Init(SDL_INIT_AUDIO);
    // Mix_Init(MIX_INIT_MP3);
    // Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);

    // Mix_Music *music=Mix_LoadMUS("Rihanna - Diamonds.mp3");
    // Mix_PlayMusic(music,-1);
    


    mvprintw(10,60,"1.Log in  2.Sign up 3.Guest");
    char c;
    noecho();
    while(1){
        c=getch();
        if(c==49 || c==50 || c==51){
            break;
        }
    }
    echo();
    if(c==49){
        //Hero hero;
        clear();refresh();
        char User[100];
        char Pass[100];
        

        while(1){
            file=fopen("data.txt","r");
            mvprintw(10,60,"Enter your Username: ");
            scanw("%s",User);
            mvprintw(12,60,"Enter your Password: ");
            scanw("%s",Pass);
            //mvprintw(3,0,"%s",Pass);

            if(check_Pass_User(User,Pass,file)==1){
                mvprintw(14,60,"User and Pass found!. Press any key to continue");
                fclose(file);
                getch();
                clear();
                break;
            }
            if(check_Pass_User(User,Pass,file)==0){
                mvprintw(14,60,"User or Pass is not correct. 1-random password  2-back");
                fclose(file);
                char a=getch();
                while(1){
                    echo();
                if(a==50){
                clear(); refresh();
                continue;}
                else if(a==49){
                    clear(); refresh();
                    while(1){
                    WINDOW*win=newwin(17,60,10,50);
                    box(win,0,0);wrefresh(win);
                    mvwprintw(win,1,1,"for get the random password press Enter");wrefresh(win);
                    char a=getch();
                    if(a=='\n'){
                       start_color();
                       init_pair(1,COLOR_RED,COLOR_BLACK);
                       int n=rand()%9000 +1000;
                       wattron(win,COLOR_PAIR(1));
                       mvwprintw(win,3,1,"Pass: %d",n);wrefresh(win);
                       wattroff(win,COLOR_PAIR(1));
                       getch(); werase(win);wrefresh(win);
                       mvprintw(10,60,"Enter your Username: "); refresh();
                       scanw("%s",User);
                       FILE*file=fopen("data.txt","r");
                        //scanw("%s",User);
                        while(check_User(User,file)==0){
                          clear();
                          mvprintw(10,60,"Enter your Username: "); refresh();
                          scanw("%s",User);
                       }
                       refresh();
                       while(1){
                        mvprintw(12,60,"Enter yor random password: ");
                        int Rand;
                        scanw("%d",&Rand);
                        if(Rand==n){
                            clear(); refresh();
                            break;
                        }
                        clear();refresh();
                       }
                       fclose(file);
                       break;


                    }
                }
            }
            if(a!=49 && a!=50){
                noecho();
                a=getch();
                continue;
            }
            
    
        } 
        strcpy(hero.name,User);
        break;  }
        }
    }
    else if(c==50){
        clear();
        refresh();

		file=fopen("data.txt","r");

        mvprintw(10,60,"Enter your Username: ");
        refresh();
         char User[100];
        while(1){
            scanw("%s",User);
            if(check_User(User,file)==1){
                mvprintw(11,60,"Username token. try another Username: ");
                continue;
            }
            if(check_User(User,file)==0){
                break;
            }
        }

        mvprintw(12,60,"Enter your Password: ");
        char Pass[100];
        while(1){
            scanw("%s",Pass);
            if(check_Pass(Pass)==0){
                mvprintw(13,60,"Password is not strong. try another Password: ");
                continue;

                }
            if(check_Pass(Pass)==1){
                break;
            }    
        }

        printw("Enter your Email Address: ");
        char email[100];
        while(1){

            scanw("%s",email);
            if(check_Email(email)==1){
                printw("Email is not correct. try again: ");
                continue;

            }
            if(check_Email(email)==0){
                break;
            }
        }
        FILE*Gold=fopen("gold.txt","a");
        fprintf(Gold,"%s:%d:%d\n",User,0,0);
        fclose(Gold);

        FILE*file=fopen("data.txt","a");
        Write_file(file,User,Pass,email);
        fclose(file);
        strcpy(hero.name,User);

    }
    else if(c==51){
        char User[1][10];
        strcpy(User[0],"Guest");
        strcpy(hero.name,User[0]);
    }
    clear();
    setting();
    clear();
      
    getch();
    endwin();

}