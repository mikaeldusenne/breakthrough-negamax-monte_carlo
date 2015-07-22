//
//  view.h
//  Breakthrough
//
//  Created by Mikaël Dusenne on 23/07/2015.
//
//


#ifndef __Breakthrough__view__
#define __Breakthrough__view__



/* includes */

#include <ncurses.h>

#include "lib.h"
#include "game_model.h"



/* defines */

#define GAMEMODE_NCURSES 1
#define GAMEMODE_CONSOLE 2


#define timeout_val -1

#define HIGHLIGHT_CURSOR A_STANDOUT
#define SELECTED_CURSOR (A_BOLD | A_UNDERLINE)


/* variables */


extern int ui_cursor,ui_selected,ui_from,ui_to,quit;
extern int ui_possible_selection[3],uipsn;




/* functions */

void init_view(int gmmd);

void render(UI64 bits,int shall_draw_board);
void render_to_file(FILE *f);

void print_all_values(UI32 l[],int all_values[]);

void quit_view();
void consolog(char* str);


void print_tournament_scores(int scores[],int max);

void pause_wait_key();
int get_user_input();
int press_enter();
void press_esc();
void press_arrow(int dir);
void consolog(char * str);
void refresh_me();

#endif /* defined(__Breakthrough__view__) */
