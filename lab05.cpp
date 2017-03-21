#include <cstdlib>
#include <ncurses.h>

int main() 
{
  initscr();                 // (A)
  printw("Hello World !!!"); // (B)
  refresh();                 // (C)
  getch();                   // (D)
  endwin();                  // (E)
  return EXIT_SUCCESS;
}