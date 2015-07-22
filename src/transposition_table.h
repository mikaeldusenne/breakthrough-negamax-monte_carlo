//
//  transposition_table.h
//  Breakthrough
//
//  Created by Mikaël Dusenne on 24/07/2015.
//
//

#ifndef __Breakthrough__transposition_table__
#define __Breakthrough__transposition_table__

/* includes */
#include "lib.h"
#include "board.h"

/* defines */

#define NODE_PV 1
#define NODE_FAIL_HIGH 2
#define NODE_FAIL_LOW 3


/* variables */
extern UI64 board_hash;

extern UI64 zobrist_hash_rnd[64*2+1];
extern int zobrist_player_index;

extern int n_transpo_lookup,n_transpo_cutoffs,n_transpo_errorN2;

struct Transpo{
	//UI64 lock;
	//int used;
	UI32 lock;
	
	UI32 best_mv;
	int score;
	char node_type;
	
	unsigned char depth;
	int alpha;
	int beta;
} ;

typedef struct Transpo Transpo;


/* functions */
void init_zobrist();

void reinit_transpo_table();
Transpo* lookup(UI64 hash_key,char *is_ok);
void store(UI64 hash_key,UI32 best_move,int score,int node_type,int depth,int alpha, int beta);

#endif /* defined(__Breakthrough__transposition_table__) */
