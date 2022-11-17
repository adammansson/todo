#ifndef TODO_UI_H
#define TODO_UI_H

#include "vla.h"

typedef struct ui {
  // the vla keeping track of the todos
  vla_t *elements;

  // the row to render next
  unsigned int row_to_render;

  // the selected row
  unsigned int selected_row;

  // number of rows and cols of the window
  unsigned int nbr_rows, nbr_cols;
} ui_t;

// display the todos in the vla, different appearance for selected_row
void UI_display_todos(ui_t *self);

// let the user input the text for a new todo and prepend it to the vla
void UI_add_todo(ui_t *self);

// mark a todo as completed or the opposite
void UI_toggle_todo(ui_t *self);

void UI_display_label(ui_t *self, char *str);

void UI_display_error(ui_t *self, char *str);

// change the value of selected_row
void UI_move_selection(ui_t *self, int amount);

#endif // TODO_UI_H
