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
char const * choices[] = { 
			"Open",
			"Save",
			"Save As",
			"Exit",
		  };
int n_choices = sizeof(choices) / sizeof(char *);
void print_menu(WINDOW *menu_win, int highlight);


int main(const int argc, const char * argv []){
  //initialize curses
  initscr();
  clear();
  noecho();
  cbreak();
  clear();
  WINDOW *pad;
  WINDOW *win1;
  WINDOW *win2;
  WINDOW *win3;
  int row, col;
  int c1,c3;
  int highlight=1, choice=0;
  bool isNotOpen = TRUE;
  getmaxyx(stdscr, row, col); 
 	startx = (col - WIDTH) / 2;
	starty = (row - HEIGHT) / 2;

  //the outer window
  win1 = newwin(row,col,0,0);
  pad = newpad(row,col);
  mvwprintw(win1,1,1, "F1: Menu");
  mvwprintw(win1,1, startx, "CSCI 1730 Editor!");
  wrefresh(win1);
  
  //to create inner box
  win2 = newwin(row-4, col-2, 2, 2);
  box(win2,0,0);
  wrefresh(win2);
  
  win3 = newwin(HEIGHT,WIDTH,starty,startx);  
  //To use keypad
  keypad(pad, TRUE);
  keypad(win1, TRUE);
  keypad(win3,TRUE);
  
  //To create menu
  c1 = wgetch(win1);
  c3 = wgetch(win3);
  if (c1== KEY_F(1) && isNotOpen){
    isNotOpen=FALSE;
    print_menu(win3,highlight);
    while(1){	
  		switch(c3)
  		{	case KEY_UP:
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
  			case 10:
  				choice = highlight;
  				break;
  		}
  		print_menu(win3, highlight);
  		if(choice != 0)	/* User did a choice come out of the infinite loop */
  			break;
  	}	
  }
  else if (c1 == KEY_F(1) && !isNotOpen){
    isNotOpen=TRUE;
    wclear(win3);
  }


  //To end the program
  wgetch(pad);
  endwin();
  return EXIT_SUCCESS;
}  




/**
*to print the menu out 
*
*/

void print_menu(WINDOW *menu_win, int highlight)
{
	int x, y, i;	
	x = 2;
	y = 2;
	box(menu_win, 0, 0);
	for(i = 0; i < n_choices; ++i)
	{	if(highlight == i + 1) /* High light the present choice */
		{	wattron(menu_win, A_REVERSE); 
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}
	wrefresh(menu_win);
}
