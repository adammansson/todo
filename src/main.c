#include <curses.h>
#include <ncurses.h>
#include <string.h>

int main(int argc, char **argv) {
  int quit, ch, input_ch, uncompleted_count, completed_count, selected_row,
      current_row, render_row, row, col, i;
  char uncompleted[64][64];
  char completed[64][64];
  char buffer[64];

  quit = 0;
  uncompleted_count = 0;
  completed_count = 0;
  selected_row = 0;

  initscr();
  getmaxyx(stdscr, row, col);
  raw();
  noecho();
  curs_set(0);

  while (!quit) {
    render_row = 0;
    current_row = 0;

    move(render_row, 0);
    printw("Uncompleted:");
    render_row++;
    for (int i = 0; i < uncompleted_count; i++) {
      move(render_row, 0);
      if (current_row == selected_row) {
        attron(A_STANDOUT);
        printw("%i. %s", current_row, uncompleted[i]);
        attroff(A_STANDOUT);
      } else {
        printw("%i. %s", current_row, uncompleted[i]);
      }
      render_row++;
      current_row++;
    }

    move(render_row, 0);
    printw("-------------------------");
    render_row++;

    move(render_row, 0);
    printw("Completed:");
    render_row++;
    for (int i = 0; i < completed_count; i++) {
      move(render_row, 0);
      if (current_row == selected_row) {
        attron(A_STANDOUT);
        printw("%i. %s", current_row, completed[i]);
        attroff(A_STANDOUT);
      } else {
        printw("%i. %s", current_row, completed[i]);
      }
      render_row++;
      current_row++;
    }
    refresh();

    ch = getch();
    if (ch == 'q') {
      quit = 1;
    } else if (ch == 'j') {
      if (selected_row < uncompleted_count + completed_count - 1) {
        selected_row += 1;
      }
    } else if (ch == 'k') {
      if (selected_row > 0) {
        selected_row -= 1;
      }
    } else if (ch == 'a') {
      memset(buffer, 0, 64);

      i = 0;
      while (i < 64) {
        move(row - 1, 0);
        attron(A_BOLD);
        printw("Adding todo: %s\n", buffer);
        attroff(A_BOLD);

        input_ch = getch();
        if (input_ch == '\n') {
          buffer[i] = '\0';
          break;
        } else if (input_ch == 127) {
          i--;
          buffer[i] = 0;
        } else {
          buffer[i] = input_ch;
          i++;
        }
      }
      clear();
      strcpy(uncompleted[uncompleted_count], buffer);
      uncompleted_count++;
    } else if (ch == '\n') {
      if (selected_row < uncompleted_count) {
        strcpy(completed[completed_count], uncompleted[selected_row]);
        completed_count++;
        strcpy(uncompleted[selected_row], "");
        uncompleted_count--;
        clear();
      }
    } else {
      move(row - 1, 0);
      attron(A_BOLD);
      printw("Unknown command: %c\n", ch);
      attroff(A_BOLD);
    }
  }

  endwin();
  return 0;
}
