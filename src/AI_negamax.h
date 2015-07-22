//
//  AI_negamax.h
//  Breakthrough
//
//  Created by Mikaël Dusenne on 23/07/2015.
//
//



#ifndef __Breakthrough__AI_negamax__
#define __Breakthrough__AI_negamax__


/* includes */
#include "lib.h"
//#include "board.h"
#include "game_model.h"
#include "GUI.h"
#include "transposition_table.h"
#include "random_position.h"
#include "eval.h"

/* defines */
#define EVAL_VERBOSE 0

#define WINNING_VALUE 9999999
#define INF 99999999
#define ALLOWED_BLACK_TIME 1
#define ALLOWED_WHITE_TIME ALLOWED_BLACK_TIME

#define MIN_SEARCH_PROF 2
#define MAX_SEARCH_PROF 99

/* variables */

extern int USE_TRANSPOSITION_TABLE;

extern UI64 quiescence_pw_rank;
extern UI64 quiescence_pb_rank;

extern UI64 diag1,diag2;

extern int qs_count,qs_max;


/*   TT   */

/*
 0,  1,  2,  3,  4,  5,  6,  7
 8,  9, 10, 11, 12, 13, 14, 15
 16, 17, 18, 19, 20, 21, 22, 23
 24, 25, 26, 27, 28, 29, 30, 31
 32, 33, 34, 35, 36, 37, 38, 39
 40, 41, 42, 43, 44, 45, 46, 47
 48, 49, 50, 51, 52, 53, 54, 55
 56, 57, 58, 59, 60, 61, 62, 63
 //*/
/*
 file_n = {
 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 ,
 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 ,
 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 ,
 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 ,
 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 ,
 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 ,
 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 ,
 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 };
 
 rank_n[64] = {
 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 ,
 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 ,
 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 ,
 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 ,
 5 , 5 , 5 , 5 , 5 , 5 , 5 , 5 ,
 6 , 6 , 6 , 6 , 6 , 6 , 6 , 6 ,
 7 , 7 , 7 , 7 , 7 , 7 , 7 , 7 ,
 //	8 , 8 , 8 , 8 , 8 , 8 , 8 , 8
 };
 //*/

extern int file_n[64];

extern int rank_n[64];


/* functions */

void init_ai_negamax();


UI32 gen_alphabeta(int color);
int negamax(int prof, int alpha, int beta,int color);

int is_critical(int color);

// bitboards


int count(UI64 bits);



// QS
int all_not_quiet_moves(UI32 list[]);
int QS(int alpha, int beta);

void order_moves(UI32 l[],int size,int col,UI32 tt_best_move);


void dump_PV();








#endif /* defined(__Breakthrough__AI_negamax__) */
