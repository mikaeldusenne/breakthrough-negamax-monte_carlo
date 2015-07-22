//
//  board.c
//  Breakthrough
//
//  Created by Mikaël Dusenne on 23/07/2015.
//
//

#include "board.h"

UI64 mask,MASK_LEFT,MASK_RIGHT,w,b,ul,u,ur;

char ALL_RECORDS[10000];

int quit=0,endgame=0;

float pl_elapsed_time[2];


int last_from=-1;
int last_to=-1;
int count_b=16,count_w=16,count_rank_b[8]={8,8,0,0,0,0,0,0},count_file_b[8]={2,2,2,2,2,2,2,2};
char count_rank_w[8]={0,0,0,0,0,0,8,8},count_file_w[8]={2,2,2,2,2,2,2,2},count_diag_b[2]={8,8},count_diag_w[2]={8,8};
//*
char diag_n[64]={
	
	0,1,0,1,0,1,0,1,
	1,0,1,0,1,0,1,0,
	0,1,0,1,0,1,0,1,
	1,0,1,0,1,0,1,0,
	0,1,0,1,0,1,0,1,
	1,0,1,0,1,0,1,0,
	0,1,0,1,0,1,0,1,
	1,0,1,0,1,0,1,0
	
};
//*/
///////////////////////////////////////////*   BITBOARD   */

void init_board(){
	int i=0;
	
	ALL_RECORDS[0]='\0';
	
	reset_board();
	
	mask=0xffffffffffffffff;
	
		
	MASK_LEFT=mask;
	MASK_RIGHT=mask;
	
	for (i=0; i<64; i++) {
		
		if (i%8==0) {
			MASK_LEFT ^= 1ULL<<i;
		}else if (i%8==7) {
			MASK_RIGHT ^= 1ULL<<i;
		}
		
	}

	
}

void reset_board(){
	pl_elapsed_time[0]=0;
	pl_elapsed_time[1]=0;
	
	int i=0;
	for (i=0; i<8; i++) {
		
		if (i<2) {
			count_diag_b[i]=8;
			count_diag_w[i]=8;
		}
		
		count_rank_b[i]=i<2?8:0;
		count_file_b[i]=2;
		count_file_w[i]=2;
		count_rank_w[i]=i>5?8:0;
	}
	
	count_b=16;
	count_w=16;
	endgame=0;
	
	w=0;
	b=0;
	
	UI64 b1=1;
	
	//*
	for (i=0; i<16; i++) {
		b|=b1<<i;
		w|=b1<<(63-i);
	}

}



UI64 rank(int n){
	return 0xFFULL<<(8*n);
}

UI64 file(int n){
	return 0x0101010101010101<<n;
}