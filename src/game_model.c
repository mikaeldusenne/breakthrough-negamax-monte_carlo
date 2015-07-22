//
//  game_model.c
//  Breakthrough
//
//  Created by Mikaël Dusenne on 23/07/2015.
//
//

#include "game_model.h"



int computer_col,nodecount;



int turn_of;
int move_number;

int pl_type[2];




void init_game_model(int pl_b_type,int pl_w_type){
	
	turn_of=PL_BLACK;
	move_number=0;
	
	pl_type[0]=pl_b_type;
	pl_type[1]=pl_w_type;
}


///////////////////////////////////////////*   GAME LOGIC  */


int all_possible_moves(UI32 list[]){
	
	UI32 n=0,m;
	
	//int besti[3],best=0;
	
	if (turn_of==PL_BLACK) {
		
		UI64 bits = (b<<8) & mask & ~(b|w);
		for (; bits!=0; bits=bit_last_rest(bits)) {
			m=bit_last(bits);
			list[n++]=(m-8) | (m<<7);
		}
		
		
		for (bits = (b<<9) & MASK_LEFT & ~b  ; bits!=0; bits=bit_last_rest(bits)) {
			m=bit_last(bits);
			list[n++]=(m-9) | (m<<7);
		}
		
		
		for (bits = (b<<7) & MASK_RIGHT & ~b  ;   bits!=0; bits=bit_last_rest(bits)) {
			m=bit_last(bits);
			list[n++]=(m-7) | (m<<7);
		}
		
	}else {
		
		UI64 bits = (w>>8) & mask & ~(b|w);
		for (; bits!=0; bits=bit_rest(bits)) {
			m=bit_first(bits);
			list[n++]=(m+8) | (m<<7);
		}
		
		
		for (bits = (w>>9) & MASK_RIGHT & ~w   ;     bits!=0; bits=bit_rest(bits)) {
			m=bit_first(bits);
			list[n++]=(m+9) | (m<<7);
		}
		
		
		for (bits = (w>>7) & MASK_LEFT & ~w    ;    bits!=0; bits=bit_rest(bits)) {
			m=bit_first(bits);
			list[n++]=(m+7) | (m<<7);
		}
		
	}
	
	
	
	
	list[n]=-1;
	return n;
}

void get_possible_moves(int i,int list[]){ // for one particular coordinate
	
	list[0]=-1;
	list[1]=-1;
	list[2]=-1;
	
	UI64 b1=1;
	
	if ((w>>i)&1) {
		
		if ((~(b|w))&(b1<<(i-8))) {
			list[1]=i-8;
		}
		if ( (~w & MASK_RIGHT) & (b1<<(i-9))) {
			list[0]=i-9;
		}
		if ( (~w & MASK_LEFT) & (b1<<(i-7)) ) {
			list[2]=i-7;
		}
		
		
	}
	if ((b>>i)&1) {
		if ((~(b|w)) & (b1<<(i+8))  ) {
			list[1]=i+8;
		}
		//mvprintw(7, 40, "left = %i   ",(~((b>>(i+9))&1))&MASK_RIGHT);
		if ( (~b & MASK_LEFT) & (b1<<(i+9))  ){
			list[0]=i+9;
		}
		if ( (~b & MASK_RIGHT) & (b1<<(i+7)) ) {
			list[2]=i+7;
		}
	}
	
}

int play(UI32 mv){
	nodecount++;
	UI32 to=(mv>>7) , from = mv&127;
	
	int capture=0;
	
	if (turn_of==PL_BLACK) {
		b^=(1ULL<<from)|(1ULL<<to);
		
		
		board_hash ^= zobrist_hash_rnd[from] ^ zobrist_hash_rnd[to];
		
		if (w&(1ULL<<to)) {
			board_hash ^= zobrist_hash_rnd[64+to];
			capture=1;
			w^=(1ULL<<to);
			count_w--;
		}
		
	}else {
		w^=(1ULL<<from)|(1ULL<<to);
		
		board_hash ^= zobrist_hash_rnd[64+from] ^ zobrist_hash_rnd[64+to];
		
		if (b&(1ULL<<to)) {
			board_hash ^= zobrist_hash_rnd[to];
			capture=1;
			b^=(1ULL<<to);
			count_b--;
		}
		
	}
	
	//board_hash^=zobrist_hash_rnd[zobrist_player_index];
	turn_of^=1;
	
	return capture;
}

void undo(UI32 mv,int capture){
	
	UI32 from=mv&127,to=mv>>7;
	
	//board_hash^=zobrist_hash_rnd[zobrist_player_index];
	turn_of ^= 1;
	
	if (turn_of==PL_BLACK) {
		b^=( (1ULL<<from) | (1ULL<<to) );
		
		board_hash ^= zobrist_hash_rnd[from] ^ zobrist_hash_rnd[to];
		
		if (capture) {
			w |= (1ULL<<to);
			board_hash ^= zobrist_hash_rnd[64+to];
			count_w++;
		}
	}else {
		w^=( (1ULL<<from) | (1ULL<<to) );
		
		board_hash ^= zobrist_hash_rnd[64+from] ^ zobrist_hash_rnd[64+to];
		
		if (capture) {
			b |= (1ULL<<to);
			board_hash ^= zobrist_hash_rnd[to];
			count_b++;
		}
	}
	
	
}

void play_real(UI32 mv){
	play(mv);
	
	UI32 from=mv&127;
	UI32 to=(mv>>7);
	last_to=to;
	last_from=from;
	/*
	 char str[40];
	 sprintf(str,"is quiet? %s",is_quiet()?"yes":"no");
	 consolog(str);
	 //*/
	move_number++;
}

int check_win(){
	
	if (w==0 || b&(0xFFULL<<56)) {
		return PL_BLACK;
	}
	if (b==0 || w&0xFF) {
		return PL_WHITE;
	}
	return -1;
	
}

UI64 hash_board(){
	UI64 h=0;
	int i;
	for (i=0; i<64; i++) {
		if (b & (1ULL<<i)) {
			h ^= zobrist_hash_rnd[i];
		}else if (w & (1ULL<<i)) {
			h ^= zobrist_hash_rnd[64+i];
		}
	}
	/*
	if (turn_of==PL_BLACK) {
		h ^= zobrist_hash_rnd[zobrist_player_index];
	}
	//*/
	return h;
}

int hash_is_correct(UI64 hash){
	
	return (hash_board()==hash);
	
}


///////////////////////////////////////////////////////////////////////////////////////////////////


