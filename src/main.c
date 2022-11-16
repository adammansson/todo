#include <curses.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

char *todos[256];
int current_count, history_count;

void add_todo(char *str) {
  int i;

  for (i = current_count + history_count; i >= current_count; i--) {
    todos[i + 1] = todos[i];
  }
  todos[current_count] = str;
  current_count++;
}

void complete_todo(int selected_index) {
  int i;

  todos[current_count + history_count] = todos[selected_index];
  for (i = selected_index; i < current_count + history_count; i++) {
    todos[i] = todos[i + 1];
  }
  current_count--;
  history_count++;
}

int main(int argc, char **argv) {
  int quit, row, col, i;
  int command_ch, input_ch;
  int selected_row;
  int render_row, todo_row;
  char input_buffer[256];
  char *input_allocated;

  quit = 0;
  current_count = 0;
  history_count = 0;
  selected_row = 0;

  initscr();
  getmaxyx(stdscr, row, col);
  raw();
  noecho();
  curs_set(0);

  while (!quit) {
    render_row = 0;
    todo_row = 0;

    move(render_row, 0);
    printw("Uncompleted (%d):", current_count);
    render_row++;
    for (int i = 0; i < current_count; i++) {
      move(render_row, 0);
      if (todo_row == selected_row) {
        attron(A_STANDOUT);
        printw("%i. %s", todo_row, todos[i]);
        attroff(A_STANDOUT);
      } else {
        printw("%i. %s", todo_row, todos[i]);
      }
      render_row++;
      todo_row++;
    }

    move(render_row, 0);
    printw("-------------------------");
    render_row++;

    move(render_row, 0);
    printw("Completed (%d):", history_count);
    render_row++;
    for (int i = current_count; i < current_count + history_count; i++) {
      move(render_row, 0);
      if (todo_row == selected_row) {
        attron(A_STANDOUT);
        printw("%i. %s", todo_row, todos[i]);
        attroff(A_STANDOUT);
      } else {
        printw("%i. %s", todo_row, todos[i]);
      }
      render_row++;
      todo_row++;
    }
    refresh();

    command_ch = getch();
    if (command_ch == 'q') {
      quit = 1;
      continue;
    } else if (command_ch == 'j') {
      if (selected_row < current_count + history_count - 1) {
        selected_row += 1;
      }
    } else if (command_ch == 'k') {
      if (selected_row > 0) {
        selected_row -= 1;
      }
    } else if (command_ch == 'a') {
      memset(input_buffer, 0, 64);

      i = 0;
      while (i < 255) {
        move(row - 1, 0);
        attron(A_BOLD);
        printw("Adding todo: %s\n", input_buffer);
        attroff(A_BOLD);

        input_ch = getch();
        if (input_ch == '\n') {
          break;
        } else if (input_ch == 127 && i > 0) {
          i--;
          input_buffer[i] = 0;
        } else {
          input_buffer[i] = input_ch;
          i++;
        }
      }

      input_buffer[i] = '\0';
      input_allocated = malloc(i);
      if (input_allocated == NULL) {
        quit = 1;
        continue;
      }

      strcpy(input_allocated, input_buffer);
      add_todo(input_allocated);

      clear();
    } else if (command_ch == '\n') {
      if (current_count > selected_row) {
        complete_todo(selected_row);
        clear();
      }
    } else if (command_ch == 'e') {
      strcpy(input_buffer, todos[selected_row]);

      i = strlen(input_buffer);
      while (i < 255) {
        move(row - 1, 0);
        attron(A_BOLD);
        printw("Editing todo: %s\n", input_buffer);
        attroff(A_BOLD);

        input_ch = getch();
        if (input_ch == '\n') {
          break;
        } else if (input_ch == 127 && i > 0) {
          i--;
          input_buffer[i] = 0;
        } else {
          input_buffer[i] = input_ch;
          i++;
        }
      }
      input_buffer[i] = '\0';

      clear();
      strcpy(todos[selected_row], input_buffer);
    } else if ('0' <= command_ch && command_ch <= '9') {
      if (current_count + history_count > command_ch - '0') {
        selected_row = command_ch - '0';
      }
    } else {
      move(row - 1, 0);
      attron(A_BOLD);
      printw("Unknown command: %c\n", command_ch);
      attroff(A_BOLD);
    }
  }

  endwin();
  return 0;
}
