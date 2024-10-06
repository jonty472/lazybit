#include <curses.h>

const int KEY_ESCAPE = 27;
WINDOW *create_new_box_window(int height, int width, int starty, int startx);
void clear_window(WINDOW *window);
void enter_input_window(WINDOW *input_window, WINDOW *todo_window);
void enter_todo_item_window(WINDOW *todo_item_window);
void activate_window(WINDOW *window);
void deactivate_window(WINDOW *window);
void add_todo_item(WINDOW *todo_window, char input[100]);

int main(void) {

  WINDOW *todo_window;
  WINDOW *todo_input_window;

  initscr();
  start_color();
  noecho();

  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);

  attron(COLOR_PAIR(1));
  refresh();

  // noecho(); this disables the diplay of characters of the screen
  cbreak(); // tells the terminal 'bash' to pass us the input directly without
            // delay or having the enter key pressed
  keypad(stdscr,
         TRUE); // enables function keys, but only for the window passed in

  printw("LINES = %d\n", LINES); // LINES is a macro that returns the number of
                                 // lines for the stdscr
  printw("COLS = %d", COLS);     // same as above but cols
  refresh(); // need to always call refresh or wrefresh after we've done
             // something else later calls won't do shit...

  todo_input_window = create_new_box_window(10, 100, 2, 3);
  todo_window = create_new_box_window(10, 100, todo_input_window->_maxy + 3, 3);

  // turn colours on for differing windows
  // wattron(todo_window, COLOR_PAIR(1));
  // wprintw(todo_window, "testing1234");
  // wrefresh(todo_window);
  // wattroff(todo_window, COLOR_PAIR(1));

  // use KEY_ESCAPE in the windows
  int ch;
  while ((ch = getch()) != 27) {
    if (ch == '1') {
      enter_input_window(todo_input_window, todo_window);

    } else if (ch == '2') {
      enter_todo_item_window(todo_window);
    }
  }

  delwin(todo_window);
  delwin(todo_input_window);
  endwin();
}

WINDOW *create_new_box_window(int height, int width, int starty, int startx) {
  WINDOW *local_window;

  local_window = newwin(height, width, starty, startx);
  wattron(local_window, COLOR_PAIR(2));
  box(local_window, 0, 0);
  wrefresh(local_window);
  wattroff(local_window, COLOR_PAIR(2));
  return local_window;
}

void clear_window(WINDOW *window) {
  int window_lines = window->_maxy;
  int window_columns = window->_maxx;

  for (int y = 1; y < window_lines; ++y) {
    for (int x = 1; x < window_columns; ++x) {
      mvwprintw(window, y, x, " ");
    }
  }
  wrefresh(window);
}

void enter_input_window(WINDOW *input_window, WINDOW *todo_window) {
  char todo_item[1024];
  int ch, y, x, input_index, todo_window_y;

  activate_window(input_window);
  input_index = 0;
  y = 1;
  x = 1;
  todo_window_y = 1;
  while ((ch = getch()) != 27) {

    if (ch == '\n') {
      todo_item[input_index++] = '\0';
      mvwprintw(todo_window, todo_window_y, 1, "%s", todo_item);
      input_index = 0;
      clear_window(input_window);
      wrefresh(todo_window);
      x = 1;
      y = 1;
      ++todo_window_y;

    } else if (input_index < sizeof(todo_item) - 1 && ch != 27) {

      if (x == input_window->_maxx - 1) {
        x = 1;
        ++y;
      }
      todo_item[input_index++] = ch;
      mvwprintw(input_window, y, x, "%c", ch);
      wrefresh(input_window);
      ++x;
    }
  }
  deactivate_window(input_window);
}

void activate_window(WINDOW *window) {
  // draw box around window with green colour
  wattron(window, COLOR_PAIR(3));
  box(window, 0, 0);
  wrefresh(window);
  wattroff(window, COLOR_PAIR(3));
}

void add_todo_item(WINDOW *todo_window, char input[100]) {
  mvwprintw(todo_window, 1, 1, "testing %s", input);
}

void enter_todo_item_window(WINDOW *todo_window) {
  // activate window
  // while loop to handle key down/up to move through the items
  // hide cursor
  // highlight current line

  activate_window(todo_window);

  wmove(todo_window, 1, 1);
  wrefresh(todo_window);

  int ch, cursor_current_y, cursor_current_x;
  cursor_current_y = 1;
  cursor_current_x = 1;
  while ((ch = getch()) != 27) {
    switch (ch) {
    case KEY_DOWN:
      ++cursor_current_y;
      wmove(todo_window, cursor_current_y, cursor_current_x);
      wrefresh(todo_window);
      break;
    case KEY_UP:
      --cursor_current_y;
      wmove(todo_window, cursor_current_y, cursor_current_x);
      wrefresh(todo_window);
      break;
    }
  }
  deactivate_window(todo_window);
}

void deactivate_window(WINDOW *window) {
  wattron(window, COLOR_PAIR(2));
  box(window, 0, 0);
  wrefresh(window);
  wattroff(window, COLOR_PAIR(2));
}
