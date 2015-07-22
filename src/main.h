//
//  main.h
//  Breakthrough
//
//  Created by Mikaël Dusenne on 23/07/2015.
//
//




#ifndef Breakthrough_main_h
#define Breakthrough_main_h


/* includes */

#include "lib.h"

#include "GUI.h"
#include "AI_MC.h"
#include "AI_negamax.h"
#include "board.h"
#include "game_model.h"

/* defines */



#define AI_MC 0
#define AI_ALPHABETA 2


/*  variables */



/* FUNCTIONS */
void init();

int start_game();
int tournament(int n, int white_or_black);

int recur_tournament(int *what,int prof,int best,int lim_a,int lim_b,int val_a,int val_b);

void quit_app();













#endif
