#include "ui.h"
#include "vla.h"
#include <curses.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

void UI_display_todos(ui_t *self) {
  unsigned int i;

  for (i = 0; i < self->elements->length; i++) {
    move(self->row_to_render, 0);
    if (i == self->selected_row) {
      attron(A_STANDOUT);
      printw("%s", VLA_get(self->elements, i));
      attroff(A_STANDOUT);
    } else {
      printw("%s", VLA_get(self->elements, i));
    }
    self->row_to_render++;
  }
}

void UI_add_todo(ui_t *self) {
  char input_buffer[256];
  char *input_allocated;

  move(self->nbr_rows - 1, 0);
  attron(A_BOLD);
  printw("Adding todo: ");
  echo();
  curs_set(1);
  getstr(input_buffer);
  attroff(A_BOLD);
  curs_set(0);
  noecho();

  // 4 chars for the brackets, strlen + 1 for length of string + \0
  input_allocated = malloc(4 + strlen(input_buffer) + 1);

  // dest as input_allocated + 4 to fit the brackets in the beginning
  strcpy(input_allocated + 4, input_buffer);

  input_allocated[0] = '[';
  input_allocated[1] = ' ';
  input_allocated[2] = ']';
  input_allocated[3] = ' ';
  VLA_prepend(self->elements, input_allocated);
}

void UI_edit_todo(ui_t *self) {
  char input_buffer[256];
  char *input_allocated;
  int ch;
  int i;

  strcpy(input_buffer, VLA_get(self->elements, self->selected_row) + 4);

  move(self->nbr_rows - 1, 0);
  attron(A_BOLD);
  printw("Editing todo: %s", input_buffer);
  curs_set(1);

  i = strlen(input_buffer);
  while (i < 255) {
    move(self->nbr_rows - 1, 0);
    clrtoeol();
    printw("%i Editing todo: %s", i, input_buffer);
    ch = getch();

    if (ch == '\n') {
      break;
    } else if (ch == 127) {
      if (i > 0) {
        i--;
        input_buffer[i] = '\0';
      }
    } else {
      input_buffer[i] = ch;
      input_buffer[i + 1] = '\0';
      i++;
    }
  }

  // 4 chars for the brackets, strlen + 1 for length of string + \0
  input_allocated = malloc(4 + strlen(input_buffer) + 1);

  // dest as input_allocated + 4 to fit the brackets in the beginning
  strcpy(input_allocated + 4, input_buffer);
  input_allocated[0] = '[';
  input_allocated[1] = ' ';
  input_allocated[2] = ']';
  input_allocated[3] = ' ';

  free(VLA_get(self->elements, self->selected_row));
  self->elements->data[self->selected_row] = input_allocated;

  attroff(A_BOLD);
  curs_set(0);
}

void UI_toggle_todo(ui_t *self) {
  char *str;

  str = VLA_get(self->elements, self->selected_row);
  VLA_remove(self->elements, self->selected_row);
  if (str[1] == ' ') {
    str[1] = 'X';
    VLA_append(self->elements, str);
  } else {
    str[1] = ' ';
    VLA_prepend(self->elements, str);
  }
  clear();
}

void UI_display_label(ui_t *self, char *str) {
  move(self->row_to_render, 0);
  printw("%s", str);
  self->row_to_render++;
}

void UI_display_error(ui_t *self, char *str) {
  move(self->nbr_rows - 1, 0);
  attron(A_BOLD);
  printw("%s", str);
  attroff(A_BOLD);
}

void UI_move_selection(ui_t *self, int amount) {
  self->selected_row += amount;
  clear();
}
