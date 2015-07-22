/*
 *  eval.h
 *  Breakthrough
 *
 *  Created by Mikaël Dusenne on 28/07/15.
 *  Copyright 2015 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef __Breakthrough__eval__
#define __Breakthrough__eval__

#include "board.h"
#include "lib.h"


extern int eval_lateral_factor,eval_central_factor,eval_first_pawn,eval_diag_factor;
extern int eval_factor_first_piece_defended;

int eval(int color);
int piece_is_defended(int n,int color);

#endif
