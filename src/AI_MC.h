//
//  AI_MC.h
//  Breakthrough
//
//  Created by Mikaël Dusenne on 23/07/2015.
//
//

#ifndef __Breakthrough__AI_MC__
#define __Breakthrough__AI_MC__

#if 0

/* includes */

#include "lib.h"
#include "AI_negamax.h"



/* defines */

#define N_RANDOM 100000


/* variables */

int nmoves=0,all_nmoves=0;


/* functions */



UI32 gen_mc(int color);
int gen_random_game(int color);


#endif


#endif /* defined(__Breakthrough__AI_MC__) */
