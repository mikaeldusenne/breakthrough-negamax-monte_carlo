//
//  game_model.h
//  Breakthrough
//
//  Created by Mikaël Dusenne on 23/07/2015.
//
//



#ifndef __Breakthrough__game_model__
#define __Breakthrough__game_model__

//extern int count_incremental;


/* includes */

#include "lib.h"
#include "board.h"
#include "AI_negamax.h"

/* defines */


#define PL_HUMAN 0
#define PL_COMPUTER 1

#define PL_BLACK 0
#define PL_WHITE 1

/* variables */

extern int computer_col,nodecount;



extern int turn_of;
extern int move_number;

extern int pl_type[2];

/* functions */
void init_game_model(int pl_b_type,int pl_w_type);

int play(UI32 mv);
void undo(UI32 mv,int capture);
void play_real(UI32 mv);
void get_possible_moves(int i,int list[]);
int check_win();
int all_possible_moves(UI32 list[]);

UI64 hash_board();
int hash_is_correct(UI64 hash);


#endif /* defined(__Breakthrough__game_model__) */
