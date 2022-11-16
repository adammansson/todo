#include "vla.h"
#include <curses.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_BUFFER_LENGTH (256)

vla_t *completed, *uncompleted;
int render_row, todo_row;
int selected_row;

void display_elements(vla_t *vla) {
  for (int i = 0; i < vla->length; i++) {
    move(render_row, 0);
    if (todo_row == selected_row) {
      attron(A_STANDOUT);
      printw("%i. %s", todo_row, vla->data[i]);
      attroff(A_STANDOUT);
    } else {
      printw("%i. %s", todo_row, vla->data[i]);
    }
    render_row++;
    todo_row++;
  }
}

void display_label(char *str) {
  move(render_row, 0);
  printw(str);
  render_row++;
}

int main(int argc, char **argv) {
  int quit, row, col, i;
  int command_ch, input_ch;
  char input_buffer[INPUT_BUFFER_LENGTH];
  char *input_allocated;

  printf("Initialising program\n");
  uncompleted = VLA_new(8);
  completed = VLA_new(8);

  quit = 0;
  selected_row = 0;

  initscr();
  getmaxyx(stdscr, row, col);
  raw();

  noecho();
  curs_set(0);

  while (!quit) {
    render_row = 0;
    todo_row = 0;

    display_label("Uncompleted:");
    display_elements(uncompleted);

    display_label("------------------------");

    display_label("Completed:");
    display_elements(completed);

    refresh();

    command_ch = getch();
    if (command_ch == 'q') {
      quit = 1;
      continue;
    } else if (command_ch == 'j') {
      if (selected_row < completed->length + uncompleted->length - 1) {
        selected_row += 1;
      }
    } else if (command_ch == 'k') {
      if (selected_row > 0) {
        selected_row -= 1;
      }
    } else if (command_ch == 'a') {
      memset(input_buffer, 0, INPUT_BUFFER_LENGTH);

      move(row - 1, 0);
      printw("Adding todo: ");
      echo();
      curs_set(1);
      getstr(input_buffer);
      curs_set(0);
      noecho();

      input_allocated = malloc(strlen(input_buffer) + 1);
      strcpy(input_allocated, input_buffer);
      VLA_append(uncompleted, input_allocated);
      clear();
    } else if (command_ch == '\n') {
      VLA_append(completed, uncompleted->data[0]);
      VLA_remove(uncompleted, 0);
    } else if (command_ch == 'e') {
      strcpy(input_buffer, uncompleted->data[selected_row]);

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

      strcpy(uncompleted->data[selected_row], input_buffer);
    } else if (command_ch == 'd') {
      VLA_remove(uncompleted, selected_row);
    } else if ('0' <= command_ch && command_ch <= '9') {
      if (uncompleted->length + completed->length > command_ch - '0') {
        selected_row = command_ch - '0';
      }
    } else {
      move(row - 1, 0);
      attron(A_BOLD);
      printw("Unknown command: %c\n", command_ch);
      attroff(A_BOLD);
    }
  }

  for (i = 0; i < uncompleted->length; i++) {
    free(uncompleted->data[i]);
  }
  for (i = 0; i < completed->length; i++) {
    free(completed->data[i]);
  }
  endwin();
  VLA_free(uncompleted);
  VLA_free(completed);
  return 0;
}
