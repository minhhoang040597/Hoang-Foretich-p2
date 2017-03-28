#include <ncurses.h>
#include <string>
#include <string.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#define WIDTH 30
#define HEIGHT 10 
using namespace std;

//INITIALIZATION
int startx = 0;
int starty = 0;
int x=0, y=2;
vector<string> lines;
int row, col;
int c;
//Init for the menu
string choices[4] = { 
  			"Open",
  			"Save",
  			"Save As",
  			"Exit",
  		  };
int n_choices = sizeof(choices) / sizeof(string);
int choice=0;
int highlight=1;
bool isOpen=FALSE,isProgOpen=TRUE;
char fn[80];
//Prototype of functions
void moveLeft(WINDOW* pad);
void moveRight(WINDOW* pad);
void moveUp(WINDOW* pad);
void moveDown(WINDOW*pad);
void defaultInput(WINDOW*pad);
void backSpace(WINDOW*pad);
void enter(WINDOW*pad);
//Prototype for Buffer;
void insertLine(string line, int n) ;
void appendLine(string line);
void removeLine(int n);
void printLine(WINDOW*pad, int n); 
void deleteLine();
//Prototype for Menu;
void printMenu(WINDOW *menuWin, int highlight);
void destroyMenu(WINDOW *menuWin);
void actionMenu(WINDOW* menuWin, int highlight);
//Prototype for Action Menu
void openInputMenu(WINDOW* inputWin);
void doActionMenu(WINDOW*inputWin,WINDOW*pad);
void exitProgram(WINDOW*pad);
void openFile(WINDOW*inputWin, WINDOW*pad );


int main(const int argc, const char * argv []){
  //initialize curses
  initscr();
  clear();
  noecho();
  cbreak();
  clear();
  //Create Windows and pad
  WINDOW *pad; 
  WINDOW *menu;
    WINDOW *inputWin;
  //Vector initialize;
  appendLine("");

  getmaxyx(stdscr, row, col); 
 	startx = (col - WIDTH) / 2;
	starty = (row - HEIGHT) / 2;

  pad = newpad(row,col);
  menu = newwin(HEIGHT,WIDTH, starty, startx);
  inputWin =newwin(HEIGHT,WIDTH, starty, startx);
  
  
  //To create a pad and write on it
  mvwprintw(pad,1,1, "F1: Menu");
  mvwprintw(pad,1, startx, "CSCI 1730 Editor!");
  wmove(pad,y,x);
  prefresh(pad,0,0,0,0,row,col);
  keypad(pad,TRUE); 

  //text editor part
  while(isProgOpen){
    switch(c=wgetch(pad)){
      case KEY_LEFT:
        moveLeft(pad);
        break;
      case KEY_RIGHT:
        moveRight(pad); 
        break;
      case KEY_UP:
        moveUp(pad);
        break;
      case KEY_DOWN:
        moveDown(pad);
        break;
      case 8:
      case KEY_BACKSPACE:
        backSpace(pad);
        break;  
      case 10:  
      case KEY_ENTER:
        enter(pad);
        break;  
      case KEY_F(1):
        if (!isOpen){
          keypad(pad,FALSE);
          isOpen=TRUE;
          printMenu(menu,highlight);
          actionMenu(menu,highlight);
          mvwprintw(pad,LINES-2,0,"Your choice: %s", choices[choice-1].c_str());
          doActionMenu(inputWin, pad);
          choice =0;
          prefresh(pad,0,0,0,0,row,col);
          keypad(pad,TRUE);
        }   
        menu = newwin(HEIGHT,WIDTH, starty, startx);
        break;
      default:
        defaultInput(pad);
        break;
    }
 }
  
  //To end the program
  getch();
  refresh();
  endwin();
  return EXIT_SUCCESS; 
}



/**
*FUNCTIONS FOR ACTIONS
*/
void moveLeft(WINDOW* pad){
    if(x > 0){
      --x;
      wmove(pad,y, x);
      prefresh(pad, 0, 0, 0 ,0 , row,col);
    }
}

void moveRight(WINDOW* pad){
    if(x < COLS-1 && x+1 <= (int)lines[y-2].length()){
        x++;
        wmove(pad,y, x);
        prefresh(pad, 0, 0, 0 ,0 , row,col);
    }
}
void moveUp(WINDOW* pad){
    if(y-2 > 0)
        y--;
    if(x >= (int)lines[y-2].length())
        x =lines[y-2].length();
    wmove(pad,y, x);
    prefresh(pad, 0, 0, 0 ,0 , row,col);
}

void moveDown(WINDOW*pad){
    if(y-2+1 < (int)lines.size())
        y++;
    if(x >= (int)lines[y-2].length())
        x = lines[y-2].length();
    wmove(pad,y, x);
    prefresh(pad, 0, 0, 0 ,0 , row,col);
}
void defaultInput(WINDOW*pad){
    if (x>COLS-1){y++;x=0;appendLine("");}
    mvwinsch(pad,y,x,c);
    lines[y-2].insert(x,1,(char)c);  
    x++;
    wmove(pad,y,x);
    prefresh(pad, 0, 0, 0 ,0 , row,col);
}
void backSpace(WINDOW*pad){
    if (x>0 || y-2>0){
      if(x == 0 && y > 0){
        x = lines[y-3].length();
        lines[y-3] += lines[y-2];// Bring the line down
        deleteLine();// Delete the current line
        moveUp(pad);
      }
      else{
        x--;
        mvwdelch(pad, y, x);
        prefresh(pad, 0, 0, 0 ,0 , row,col);
        lines[y-2].erase(lines[y-2].begin()+x);
      }
    }   
}
void enter(WINDOW*pad){
  if(x <(int)lines[y-2].length()){
      insertLine(lines[y-2].substr(x, lines[y-2].length() - x), y + 1 -2);
      lines[y-2].erase(x, lines[y-2].length() - x);
  }
  else{insertLine("", y+1 -2);}
  x = 0;
  moveDown(pad);
}
/**
*BUFFER CONTROL
*/
void insertLine(string line, int n)  {lines.insert(lines.begin()+n, line);}
void appendLine(string line)  {lines.push_back(line);}
void removeLine(int n)  {lines.erase(lines.begin()+n);}
void deleteLine()  {removeLine(y);}
void printLine(WINDOW*pad, int n) {
  mvwprintw(pad,n , 0, lines[n-2].c_str());
  prefresh(pad, 0, 0, 0 ,0 , row,col);
}

/**
*MENU
*/
void printMenu(WINDOW *menuWin, int highlight){
  int x1, y1, i;	
	x1 = 2;
	y1 = 2;
	box(menuWin, 0, 0);
	for(i = 0; i < n_choices; ++i)
	{	if(highlight == i + 1) /* High light the present choice */
		{	wattron(menuWin, A_REVERSE); 
			mvwprintw(menuWin, y1, x1, "%s", choices[i].c_str());
			wattroff(menuWin, A_REVERSE);
		}
		else
			mvwprintw(menuWin, y1, x1, "%s", choices[i].c_str());
		++y1;
	}
	wrefresh(menuWin);
}
void destroyMenu(WINDOW *menuWin){
  wclear(menuWin);
	wrefresh(menuWin);
}
void actionMenu(WINDOW* menuWin, int highlight){
  int c1;
  keypad(menuWin,TRUE);
     while(isOpen){	
 		  switch(c1=wgetch(menuWin)){	
        case KEY_UP:
     			if(highlight == 1)
    				highlight = n_choices;
     			else
     				--highlight;
     			break;    			
       case KEY_DOWN:
     	   	if(highlight == n_choices)
   					highlight = 1;
   				else 
  					++highlight;
    				break;
        case KEY_F(1):
           isOpen=FALSE;
           destroyMenu(menuWin);
           break;
        case 10:
        case KEY_ENTER:
             choice= highlight;
             break;         
        default:
            isOpen=FALSE;
    				break;
   		}
     		printMenu(menuWin, highlight);
     		if(choice!=0){/* User did a choice come out of the infinite loop */
     			isOpen=FALSE;  
           break;
        }  
      }	
}

/**
*THE FUNCTION OF MENU OPTION: OPEN, LOAD, SAVE, EXIT
*/
void openInputMenu(WINDOW* inputWin){
  box(inputWin,0,0);
  mvwprintw(inputWin,1,1,"Enter File Name: ");
  wrefresh(inputWin);
  keypad(inputWin,TRUE);
  char fn[80];
  getstr(fn);
}

void doActionMenu(WINDOW*inputWin,WINDOW*pad){
  if (strcmp(choices[choice-1].c_str(), "Exit")==0){exitProgram(pad);}
  //if (strcmp(choices[choice].c_str(), "Open")==0){openFile(inputWin,pad);}
}

void exitProgram(WINDOW*pad){isProgOpen =FALSE;}
/**
void openFile(WINDOW*inputWin, WINDOW*pad ){
    openInputMenu(inputWin);
    ifstream infile(fn);
    lines.clear();
    if(infile.is_open()){
        while(!infile.eof()){
            string temp;
            getline(infile, temp);
            appendLine(temp);
        }
    }
}
*/
