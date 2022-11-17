#include "ui.h"
#include "vla.h"
#include <curses.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  int quit;
  int command_ch;
  char label_buffer[64];

  ui_t ui;
  ui.elements = VLA_new(1);
  ui.row_to_render = 0;
  ui.selected_row = 0;

  initscr();
  getmaxyx(stdscr, ui.nbr_rows, ui.nbr_cols);
  raw();
  noecho();
  curs_set(0);

  quit = 0;
  while (!quit) {
    ui.row_to_render = 0;

    UI_display_label(&ui, "Todo");

    UI_display_elements(&ui);

    refresh();

    command_ch = getch();
    if (command_ch == 'q') {
      quit = 1;
    } else if (command_ch == 'j') {
      if (ui.selected_row < ui.elements->length - 1) {
        UI_move_selection(&ui, 1);
      }
    } else if (command_ch == 'k') {
      if (ui.selected_row > 0) {
        UI_move_selection(&ui, -1);
      }
    } else if (command_ch == 'a') {
      UI_add_todo(&ui);
    } else if (command_ch == '\n') {
      UI_toggle_todo(&ui);
    } else {
      sprintf(label_buffer, "Unknown command: %c", command_ch);
      UI_display_error(&ui, label_buffer);
    }
    clear();
  }

  endwin();
  VLA_free(ui.elements);
  return 0;
}
