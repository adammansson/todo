#include "ui.h"
#include "vla.h"
#include <curses.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_BUFFER_LENGTH (256)

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

static char *UI_get_input(ui_t *self, char *prompt, char *initial) {
  char input_buffer[INPUT_BUFFER_LENGTH], *input_allocated;
  int ch, current_length;

  strcpy(input_buffer, initial);

  attron(A_BOLD);
  curs_set(1);

  current_length = strlen(input_buffer);
  while (current_length < INPUT_BUFFER_LENGTH - 1) {
    move(self->nbr_rows - 1, 0);
    clrtoeol();
    printw("%s %s", prompt, input_buffer + 4);
    ch = getch();

    if (ch == 27) {
      attroff(A_BOLD);
      curs_set(0);
      return NULL;
    } else if (ch == '\n') {
      break;
    } else if (ch == 127 && current_length > 0) {
      current_length--;
      input_buffer[current_length] = '\0';
    } else if (32 <= ch && ch <= 127) {
      input_buffer[current_length] = ch;
      input_buffer[current_length + 1] = '\0';
      current_length++;
    }
  }

  input_allocated = malloc(strlen(input_buffer) + 1);

  strcpy(input_allocated, input_buffer);

  attroff(A_BOLD);
  curs_set(0);
  return input_allocated;
}

void UI_add_todo(ui_t *self) {
  char *res;

  res = UI_get_input(self, "Adding todo:", "[ ] ");
  if (res == NULL) {
    return;
  }
  VLA_prepend(self->elements, res);
}

void UI_edit_todo(ui_t *self) {
  char *initial, *res;

  initial = VLA_get(self->elements, self->selected_row);
  res = UI_get_input(self, "Editing todo:", initial);
  if (res == NULL) {
    return;
  }
  free(initial);
  VLA_set(self->elements, self->selected_row, res);
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

void UI_move_selection(ui_t *self, int amount) { self->selected_row += amount; }
