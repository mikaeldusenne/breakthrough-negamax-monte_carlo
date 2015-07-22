//
//  transposition_table.c
//  Breakthrough
//
//  Created by Mikaël Dusenne on 24/07/2015.
//
//

#include "transposition_table.h"
#include "game_model.h"

UI64 board_hash;

UI64 zobrist_hash_rnd[64*2+1];
int zobrist_player_index=64*2;

#define TT_SIZE (0xFFFFFF+1)

Transpo table_transpo[TT_SIZE];
UI32 mask_index_TT = 0XFFFFFF;

int n_transpo_lookup=0,n_transpo_cutoffs=0,n_transpo_errorN2=0;

void init_zobrist(){
	
	/* init random values */
	
	int i=0;
	for (; i<64*2+1; i++) {
		UI64 rnd;
		
		int j=0;
		for (; j<4; j++) {
			rnd = (rnd<<16) | (rand() & 0xFFFF);
		}
		
		zobrist_hash_rnd[i]=rnd;
		
	}
	
	
	/* init board beginning hash value */
	board_hash=hash_board();
	
	
	
}

void reinit_transpo_table(){
	
	//UI64 tenth = TT_SIZE/1000,count=0;
	//int k=0;
	
	int i=0;
	for (; i<TT_SIZE; i++) {
		Transpo t;
		table_transpo[i]=t;
		table_transpo[i].depth=0;
		/*
		count++;
		if (count>tenth) {
			count=0;
			k++;
			printf("\n-  %i /1000",k);
		}
		//*/
	}
	
	
}

Transpo* lookup(UI64 hash_key,char *is_ok){
	Transpo* t = &table_transpo[hash_key&mask_index_TT];
	
	if (t->depth>0) {
		
		if (t->lock==(hash_key>>32)) {
			*is_ok=1;
			return t;
		}else{
			n_transpo_errorN2++;
		}
	
	}
	
	return NULL;
}



void store(UI64 hash_key,UI32 best_move,int score,int node_type,int depth,int alpha,int beta){
	Transpo *t=&table_transpo[hash_key&mask_index_TT];
	
	if (1 || t->depth<depth) {
		t->lock=(hash_key>>32);
		t->best_mv=best_move;
		t->score=score;
		t->node_type=node_type;
		t->depth=depth;
		t->alpha=alpha;
		t->beta=beta;
	}
	
	
	
}



