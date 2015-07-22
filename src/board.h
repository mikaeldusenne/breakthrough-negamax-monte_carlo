//
//  board.h
//  Breakthrough
//
//  Created by Mikaël Dusenne on 23/07/2015.
//
//

#ifndef __Breakthrough__board__
#define __Breakthrough__board__

/* includes */
#include "lib.h"

/* defines */


/* variables */
extern UI64 mask,MASK_LEFT,MASK_RIGHT,w,b,ul,u,ur;

extern UI64 oew,oeb;
extern int last_from;
extern int last_to;

extern int quit,endgame;

extern int count_b,count_w,count_rank_b[8],count_file_b[8];
extern char count_rank_w[8],count_file_w[8],count_diag_b[2],count_diag_w[2];
extern char diag_n[64];

extern char ALL_RECORDS[10000];
extern float pl_elapsed_time[2];

/* functions */
void init_board();
void reset_board();

UI64 rank(int n);
UI64 file(int n);



#endif /* defined(__Breakthrough__board__) */
