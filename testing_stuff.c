#include <ncurses.h>

int main(void) {
  printf("Hello world\n");

  initscr();

  wprintw(stdscr, "speak friend and enter");
  wrefresh(stdscr);


  chtype test_chtype;
  // int ch;
  // for (int i = 1; i < 10; ++i) {
  //   test_chtype = mvwinch(stdscr, 0, i);
  //   // ch = test_chtype & A_CHARTEXT;
  //   mvwprintw(stdscr, i, 1, "%c", test_chtype);
  //   wrefresh(stdscr);
  // }

  for (int i = 0; test_chtype != '\n'; ++i) {
    test_chtype = mvwinch(stdscr, 0, i);
    mvwprintw(stdscr, i, 1, "%c", test_chtype);
    wrefresh(stdscr);

    if (test_chtype == '\n') {
      wprintw(stdscr, "Found a newline char");
      wrefresh(stdscr);
    }
  }

  getch();
  delwin(stdscr);
  endwin();

  return 0;
}
