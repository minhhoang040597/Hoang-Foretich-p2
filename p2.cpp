#include <ncurses.h>
#include <string.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#define WIDTH 30
#define HEIGHT 10 
using namespace std;
int startx = 0;
int starty = 0;

int main(const int argc, const char * argv []){
      //initialize curses
  initscr();
  clear();
  noecho();
  cbreak();
  clear();
  //Create Windows and pad
  WINDOW *pad;
  //WINDOW *win1;
  int row, col;
  int x=0, y=2;
  int c;
  //int highlight=1;
  getmaxyx(stdscr, row, col); 
 	startx = (col - WIDTH) / 2;
	starty = (row - HEIGHT) / 2;
  //menu Menu;
    
  pad = newpad(row,col);
  //To create a pad and write on it
  mvwprintw(pad,1,1, "F1: Menu");
  mvwprintw(pad,1, startx, "CSCI 1730 Editor!");
  wmove(pad,y,x);
  prefresh(pad,0,0,0,0,row,col);
  keypad(pad,TRUE); 
  
  //text editor part
  while(1){
   c=wgetch(pad);
   x++;
   mvwprintw(pad,y,x,"%c",c);
   wmove(pad,y,x);
   prefresh(pad, 0, 0, 0 ,0 , row,col);
 }
  

  //WINDOW* win3 = Menu.createMenu(HEIGHT, WIDTH, starty, startx);  
  //Menu.actionMenu(win3, highlight, HEIGHT, WIDTH, starty, startx);



  //To end the program
  getch();
  refresh();
  endwin();
  return EXIT_SUCCESS;
}  


