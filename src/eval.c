#include "eval.h"
#include "AI_negamax.h"

#define EVAL_ENABLE_LATERAL 1
#define EVAL_ENABLE_CENTER 0
#define EVAL_ENABLE_DIAGONAL 1

/* macros */
#define piece_is_attacked(X,Y) piece_is_defended( X , 1-Y )


int center_eval[8]={-7,-5,-3,-1,1,3,5,7};
//int forward_eval[8]={,WINNING_VALUE,WINNING_VALUE};

int eval_lateral_factor=100,eval_first_pawn=80,eval_central_factor=0,eval_diag_factor=200;
int eval_factor_first_piece_defended=480;

int eval(int col){
	
	//UI64 bw=w,bb=b;
	//int wc=count(w),bc=count(b);    // material count
	
	int score=0;
	
	//int bit_last_b=bit_last(b),bit_first_w=bit_first(b);   // leading piece
	
#if EVAL_VERBOSE
	char str[40];
	sprintf(str,"-------- eval (%s)",col==PL_BLACK?"Black":"White");consolog(str);
#endif
	
	//score+=1000*(count(b)-count(w));
	score+=1000*(count_b-count_w);
	
		
#if EVAL_VERBOSE
		sprintf(str,"count: %i",score);consolog(str);
#endif
		
		//*
		//if (computer_col==PL_WHITE) {
		
		int fw=bit_first(w);     // most advanced piece
		int fb=bit_last(b);
		int fw_rank=rank_n[fw];   // it's rank
		int fb_rank = rank_n[fb];
		int fw_value=11-fw_rank;  // score value...
		int fb_value=4+fb_rank;
		
		//if (computer_col==PL_WHITE) {
		score +=eval_first_pawn*(SQ(fb_value)-SQ(fw_value));
		//}
	
	/*
	if (col==PL_BLACK) {
		if (fb_rank==6) {
			return WINNING_VALUE;
		}
		
		if ((fw_rank==1) && !piece_is_attacked(fw, PL_WHITE)) {
			return -WINNING_VALUE;
		}
		
	}else{
		if (fw_rank==1) {
			return WINNING_VALUE;
		}
		
		if ((fb_rank==6) && !piece_is_attacked(fb, PL_BLACK)) {
			return -WINNING_VALUE;
		}
	}
	//*/
	
	
//if (computer_col==PL_WHITE) { ////////////////////////////////////////////////////////////    ALERT
		//* /////////////////////////////////////////////////////////////
		//if (computer_col==PL_WHITE) {
		if (piece_is_defended(fb, PL_BLACK)) {
			score+=eval_factor_first_piece_defended;
		}
		if (piece_is_defended(fw, PL_WHITE)) {
			score-=eval_factor_first_piece_defended;
		}
		//}
		//*/
		
		//}
		//*/
		
#if EVAL_VERBOSE
		
		sprintf(str,"best b = %i ; best w = %i",fb,fw);consolog(str);
		sprintf(str,"--> %i",SQ(fw)+SQ(fb));consolog(str);
		sprintf(str,"TOTAL == %i",(col==PL_BLACK?score:-score));consolog(str);
		
#endif
		
		int i=0,ci;
		
		
		//if (computer_col==PL_WHITE) {
#if EVAL_ENABLE_LATERAL
		//*
		
		//if (computer_col==PL_WHITE) {
		int latb=0,latw=0;
		
		
		//*
		for (; i<8; i++) {
			ci=center_eval[i];
			
			latb+= count(b&file(i))*ci;
			latw+= count(w&file(i))*ci;
			
		}
		//*/
		if (latb<0) {
			latb=-latb;
		}
		if (latw<0) {
			latw=-latw;
		}
		score+=eval_lateral_factor*(latw-latb);
		
		
		//}
		//*/
#endif
		
#if EVAL_ENABLE_CENTER
		if (computer_col==PL_WHITE) {
			for (i=0; i<4; i++) {
				ci=center_eval[i];
				score+= eval_central_factor*(count(b&rank(i))*ci - count(w&rank(7-i))*ci);
			}
		}
#endif
		
#if EVAL_ENABLE_DIAGONAL
		//*
		//if int oeb,oew;(computer_col==PL_WHITE) {
		int oeb = count(b&diag1)-count(b&diag2);
		int oew = count(w&diag1)-count(w&diag2);
		
		
		if (oeb<0) {
			oeb=-oeb;
		}
		if (oew<0) {
			oew=-oew;
		}
		score+=eval_diag_factor*(oew-oeb);
		
		//}
		
		//*/
#endif
		//}
		
		
		//*/
		
		/* side by side heuristic */
		/*
		 score += count( ((b&(b<<8)) | ((b&(b<<7))&MASK_RIGHT) | ((b&(b<<9))&MASK_RIGHT) | ((b&(b<<1))&MASK_RIGHT) )&mask);
		 score -= count( ((w&(w<<8)) | ((w&(w<<7))&MASK_RIGHT) | ((w&(w<<9))&MASK_RIGHT) | ((w&(w<<1))&MASK_RIGHT) )&mask);
		 //*/
		//score += 100*count( ((b&(b<<8)) )&mask);
		//score -= 100*count( ((w&(w<<8)))&mask);
		
		
		
		/*     DODGING SKILLS
		 if (-(1ULL<<(rank_n[bit_last_b]+1)*8) & ( file(bit_last_b+9) & MASK_LEFT | file(bit_last_b+7) & MASK_RIGHT ) & w  == 0) {
		 score+=1000;
		 }
		 
		 UI64 tmp_b = 0;
		 if (file_n[bit_first_w]>0) {
		 tmp_b |= file(file_n[bit_first_w]-1);
		 }
		 if (file_n[bit_first_w]<7) {
		 tmp_b |= file(file_n[bit_first_w]+1);
		 }
		 if (   (1ULL<<(rank_n[bit_first_w]-1)*8) & tmp_b & b == 0 ) {
		 score -= 1000;
		 }
		 //*/
		
		
		//score += count(b&((1ULL<<bit_first(w))-1)); // dont let him go through !
		
		//score+=count( (w<<18) & (b<<9) & b & MASK_RIGHT ) + count( (w<<14) & (b<<7) & b & MASK_LEFT ); // counter attacking positions
		
	//} ////// ENDIF THAT BYPASSES ALL THE EVAL
	
	
	return (col==PL_BLACK?score:-score);
}


int piece_is_defended(int n,int color){
	int k=0;
	if (color==PL_BLACK) {
		if (b&(1ULL<<(n-9))&MASK_RIGHT) {
			k++;
		}
		if (b&(1ULL<<(n-7))&MASK_LEFT) {
			k++;
		}
		
	}else{
		if (w&(1ULL<<(n+9))&MASK_LEFT) {
			k++;
		}
		if (b&(1ULL<<(n+7))&MASK_RIGHT) {
			k++;
		}
	}
	
	return k;
}



