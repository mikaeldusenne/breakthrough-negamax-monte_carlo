//
//  AI_negamax.c
//  Breakthrough
//
//  Created by Mikaël Dusenne on 23/07/2015.
//
//




#include "AI_negamax.h"

UI64 quiescence_pw_rank=0xFFULL<<8;
UI64 quiescence_pb_rank=0xFFULL<<48;

UI64 diag1=0,diag2=0;
UI32 PV[20];

int USE_TRANSPOSITION_TABLE=1;

int file_n[64];

int rank_n[64];
int qs_count,qs_max;

int should_check_time=0;
int time_over=0;
int current_running_clock;
int is_time_over(int clock_index,float max_time);

void init_ai_negamax(){
	
	PV[0]=~0;
	
	qs_max=-1;
	
	//UI64 my_mask = 0x55;
	
	int i=0;
	for (; i<64; i++) {
		file_n[i]=i%8;
		rank_n[i]=(i-file_n[i])/8;
		
	}
	
	//bit_dump(my_mask | (my_mask<<8));
	//printf("\n%s\n%s\n%s\n----\n",byte_to_binary(0x55ULL),byte_to_binary(0x55ULL | (0x55ULL<<8)),byte_to_binary(0x55ULL | (0x55ULL<<16)));
	
	for (i=0; i<8; i++) {
		if (i%2==0) {
			diag1 |= (0x55ULL<<(i*8));
		}else{
			diag1 |= (0x55ULL<<((i*8)+1));
		}
		//diag1 ^= (bit_mask<<(i%2?0:1))<<(i*8);
	}
	diag2=~diag1;
	/*
	for (i=0; i<64; i++) {
		if (((diag1>>i)&1)==1) {
			printf("1");
		}else{printf("0");}
		if (i%8==7) {
			printf("\n");
		}
	}
	printf("\n");
	for (i=0; i<64; i++) {
		if (((diag2>>i)&1)==1) {
			printf("1");
		}else{printf("0");}
		if (i%8==7) {
			printf("\n");
		}
	}
	//*/
	
	//printf("\n%llu\n%s\n%s\n",diag1,byte_to_binary(diag1),byte_to_binary(diag2));
	pause_wait_key();
	
	
	init_zobrist();
	reinit_transpo_table();
}
/*
void order_ID(int *all_values, int *l){
	
	int i=0,j=0;
	for (; ~l[i]; i++) {
		for (j=i+1; ~l[j]; j++) {
			if (all_values[j]>all_values[i]) {
				
				
				
			}
		}
	}
	
}
//*/

int current_max_prof;

UI32 gen_alphabeta(int color){
	should_check_time=1;
	time_over=0;
	
	for (int k=0; k<20; k++) {
		PV[k]=~0;
	}
	
	//UI32 lbm[50];
	//int lbmc=0;
	
	UI32 best_move=0,previous_best_move=0;
	
	int all_values[50];
	
	computer_col=color;
	nodecount=0;
	int prof_max=(MIN_SEARCH_PROF)-1;
	
	int cl = start_clock();
	current_running_clock=cl;
	
	UI32 l[50]={-1};
	int nmv=all_possible_moves(l);
	
	order_moves(l,nmv,color,-1);
	
	int max_val;
	//*
	//reinit_transpo_table();
	n_transpo_cutoffs=0;
	n_transpo_lookup=0;
	n_transpo_errorN2=0;
	//*/
	
	while (prof_max<MAX_SEARCH_PROF && !time_over){
		
		prof_max++;
		for (int k=0; k<20; k++) {
			PV[k]=~0;
		}
		
		current_max_prof=prof_max+1;
		
		qs_max=0;
		qs_count=0;
		
		max_val=-INF;
		int alpha=-INF;
		int beta=INF;
		int v=-INF;
		int i=0;
		best_move=0;
		//lbmc=0;
		
		for (i=0; i<nmv; i++) {
			int capture = play(l[i]);
			v=-negamax(prof_max, -beta, -alpha, 1-color);
			undo(l[i],capture);
			
			all_values[i]=v;
			
			if (v>max_val) {
				max_val=v;
				best_move=l[i];
			}
			
			
			
			//*
			if (v>alpha) {
				alpha=v;
			}///*/
			//*
			if (v>=WINNING_VALUE) {
				prof_max=MAX_SEARCH_PROF;
				break;
			}//*/
			
		}
		time_over=is_time_over(cl,(color==PL_BLACK?ALLOWED_BLACK_TIME:ALLOWED_WHITE_TIME));
		if (time_over) {
			assert(previous_best_move);
			best_move=previous_best_move;
			break;
		}else{
			previous_best_move=best_move;
		}
		
		
		//diff=(((float)clock()-(float)time_begin)/1000000.0F);
		
	}
	PV[0]=best_move;
	PV[current_max_prof]=~0;
	
	//print_all_values(l, all_values);
	
	//while ((((float)clock()-(float)time_begin)/1000000.0F)<2) {}
	/*
	 char str[40];
	 sprintf(str,"first move is %i->%i",l[0]&127,l[0]>>7);
	 consolog(str);
	 //*/
	//*
	//printf("\n---- move %i ----",move_number);
	float time_taken = stop_clock(cl);
	char str[40];
	sprintf(str,"best_val == %i",max_val);consolog(str);
	sprintf(str,"%ik NPS | prof = %i",(int)((float)(nodecount)/(time_taken)),prof_max);consolog(str);
	// printf("  |  prof == %i",prof_max);
	sprintf(str,"Nodes == %i , Time == %f",nodecount,time_taken/1000.0F);consolog(str);
	
	sprintf(str, "%i transpositions",n_transpo_lookup);consolog(str);
	sprintf(str, "%i possible cutoffs",n_transpo_cutoffs);consolog(str);
	sprintf(str, "%i collisions",n_transpo_errorN2);consolog(str);
	dump_PV();
	//sprintf(str,"QS_count = %i  |  QS_max = %i",qs_count,qs_max);consolog(str);
	// printf("  |  Nodes == %i",nodecount);
	//*/
	//return lbm[0];
	
	
	should_check_time=0;
	
	
	return best_move;
}


int negamax(int prof,int alpha,int beta,int col){
	
	if (!(nodecount&4095)) {
		time_over =is_time_over(current_running_clock,(col==PL_BLACK?ALLOWED_BLACK_TIME:ALLOWED_WHITE_TIME));
	}
	
	if (time_over) {
		return 0;
	}
	
	//assert(hash_is_correct(board_hash));
	/*
	if ((computer_col==PL_WHITE) && (!prof) && is_critical(col)) {   // extension if almost loosing ???
		prof++;
	}
	//*/
	char tt_lookup_is_ok=0;
	Transpo* tlook;
	//*
	//if (USE_TRANSPOSITION_TABLE) {
		tlook = lookup(board_hash,&tt_lookup_is_ok);
		
		
		if (tt_lookup_is_ok) {
			n_transpo_lookup++;
			//tt_lookup_is_ok=1;
			if (tlook->depth>=prof) {
				n_transpo_cutoffs++;
				
				//*
				if (tlook->node_type==NODE_PV) {
					return tlook->score;
				}else if (tlook->node_type==NODE_FAIL_LOW && tlook->alpha<=alpha){
					return tlook->score;
				}else if (tlook->node_type==NODE_FAIL_HIGH && tlook->beta>=beta){
					return tlook->score;
				}
				//*/
				
			}
		}
	//}
	
	//*/
	int win=check_win();
	if (win!=-1) {
		return -(WINNING_VALUE+prof);
		//*
		//return ((win==col?1:-1)*(99999+prof));
		//*/
	}
	//assert((w|b)!=0);
	
	if (prof==0) {
		return QS(alpha, beta)-prof;
	}
	
	int max_val=-INF;
	int v;
	UI32 l[50],best_move;
	int nmv=all_possible_moves(l);
	
	UI32 save_pv[20];
	memcpy(save_pv, &PV[current_max_prof-prof-1], (20-current_max_prof+prof)*sizeof(UI32));
	
	order_moves(l,nmv,col,tt_lookup_is_ok?tlook->best_mv:0);
	
	int i=0,node_type=NODE_FAIL_LOW;
	
	for (i=0; i<nmv; i++) {
		
		int capture = play(l[i]);
		v=-negamax(prof-1,-beta,-alpha,1-col);
		undo(l[i],capture);
		
		if (v>max_val) {max_val=v;best_move=l[i];}
		
		if (v>alpha) {
			node_type=NODE_PV;
			alpha=v;
			
			if (v>=beta) {
				memcpy( &PV[current_max_prof-prof-1],save_pv, (20-current_max_prof+prof)*sizeof(UI32));
				
				store(board_hash, l[i], v, NODE_FAIL_HIGH, prof,alpha,beta);
				return v;
			}
		}
		
	}
	
	store(board_hash, best_move, max_val, node_type, prof,alpha,beta);
	
	if (node_type==NODE_PV) {
		PV[current_max_prof-prof]=best_move;
	}
	
	return max_val;
}


void order_moves(UI32 l[],int size,int col,UI32 tt_best_move){
	
	
	
	int i,k=0;
	UI32 tmp;
	
	//pause_wait_key();
	//quit_view();
	
	//printf("\nimax=%i",imax);
#if 0
	if (tt_best_move) {
		for (i=0; l[i]!=tt_best_move; i++) {
			/*
			printf("\nl[%i] = ",i);
			bit_dump(l[i]);
			printf("[");
			bit_dump(tt_best_move);
			printf("]");
			//*/
			if (i>=size) {
				last_from=tt_best_move&127;
				last_to=tt_best_move>>7;
				consolog(turn_of==PL_BLACK?"Black's turn":"White's turn");
				render(0, 1);
				
				pause_wait_key();
				quit_view();
				
				printf("\nerror , array overflow\ntt_best_move == ");
				bit_dump(tt_best_move);
				printf("\n");
				exit(0);
			}
		}
		if (i) {
			tmp=l[i];
			l[i]=l[0];
			l[0]=tmp;
			k++;
		}
	}
#endif
	if (col==PL_BLACK) {
		int best_rank=rank_n[bit_last(b)];
		/*
		 for (; ~l[k]; k++) {}
		 k--;
		 UI32 tmp;
		 for (i=0; i<(int)(k/2); i++) { // invert array --> most advanced first
			tmp=l[k-i];
			l[k-i]=l[i];
			l[i]=tmp;
		 }
		 k=0;
		 //*/
		for (i=k+1; i<size; i++) {      // captures first
			if ((w>>(l[i]>>7))&1) {
				tmp=l[i];
				l[i]=l[k];
				l[k]=tmp;
				k++;
			}
		}
		//*
		for (i=k+1; i<size; i++) {
			if (rank_n[l[i]&127]==best_rank) {
				tmp=l[i];
				l[i]=l[k];
				l[k++]=tmp;
			}
		}
		//*/
		
	}else {
		/*
		 for (i=0; ~l[i]; i++) {
			for (j=i+1; ~l[j]; j++) {
		 if (rank_n[l[i]&127]<rank_n[l[j]&127]) {
		 tmp=l[i];
		 l[i]=l[j];
		 l[j]=tmp;
		 }
			}
		 }
		 //*/
		int best_rank=rank_n[bit_first(w)];
		
		for (i=k+1; i<size; i++) {
			if ((b>>(l[i]>>7))&1) {
				tmp=l[i];
				l[i]=l[k];
				l[k]=tmp;
				k++;
			}
		}
		for (i=k; i<size; i++) {
			if (rank_n[l[i]&127]==best_rank) {
				tmp=l[i];
				l[i]=l[k];
				l[k++]=tmp;
			}
		}
		
	}
	
	
	
}


//*
int QS(int alpha, int beta){
	qs_count++;
	if (~check_win()) {
		return -WINNING_VALUE;
	}
	
	int i=0,v,max_val;
	int stand_pat=eval(turn_of);
	
	max_val=stand_pat;
	if (stand_pat>alpha) {
		alpha=stand_pat;
		if (stand_pat>=beta) {
			return stand_pat;
		}
	}
	
	
	//UI64 bb=b,ww=w;
	
	UI32 l[50];
	all_not_quiet_moves(l);
	
	
	for (; ~l[i]; i++) {
		int cap = play(l[i]);
		v=-QS(-beta,-alpha);
		undo(l[i], cap);
		
		//*
		if (v>max_val) {
			max_val=v;
			if (v>alpha) {
				alpha=v;
				if (v>=beta) {
					return max_val;
				}
			}
		}
		//*/
		
		
	}
	return max_val;
	
}

//*/










int all_not_quiet_moves(UI32 list[]){
	nodecount++;
	
	int n=0;
	UI32 m;
	
	//int besti[3],best=0;
	
	if (turn_of==PL_BLACK) {
		
		UI64 bits;
		/*
		 for (bits = (((b&quiescence_pb_rank)<<8) & ~(b|w)); bits!=0; bits=bit_last_rest(bits)) {
			m=bit_last(bits);
			list[n++]=(m-8) | (m<<7);
		 }
		 //*/
		for (bits = (b<<9) & MASK_LEFT & (~b) & w  ; bits!=0; bits=bit_last_rest(bits)) {
			m=bit_last(bits);
			list[n++]=(m-9) | (m<<7);
		}
		
		
		for (bits = (b<<7) & MASK_RIGHT & (~b) & w  ;   bits!=0; bits=bit_last_rest(bits)) {
			m=bit_last(bits);
			list[n++]=(m-7) | (m<<7);
		}
		
	}else {
		
		UI64 bits;
		/*
		 for (bits = (((w&quiescence_pw_rank)>>8) & ~(b|w)); bits!=0; bits=bit_rest(bits)) {
			m=bit_first(bits);
			list[n++]= (m+8) | (m<<7);
		 }
		 //*/
		for (bits = (w>>9) & MASK_RIGHT & (~w) & b   ;     bits!=0; bits=bit_rest(bits)) {
			m=bit_first(bits);
			list[n++]=(m+9) | (m<<7);
		}
		
		
		for (bits = (w>>7) & MASK_LEFT & (~w) & b    ;    bits!=0; bits=bit_rest(bits)) {
			m=bit_first(bits);
			list[n++]=(m+7) | (m<<7);
		}
		
	}
		
	list[n]=-1;
	return n;
	
}



int is_critical(int color){
	return (color==PL_BLACK? ( (w&0x00FF) ) : ( b&0x000000000000FFULL ) );
}




int is_time_over(int clock_index,float max_time){
	return should_check_time?(get_clock_value(clock_index)/1000.0F)>=max_time:0;
}


char alpha[8]="ABCDEFGH";

void dump_PV(){
	consolog("----  PV ----");
	char str[40],s[10];
	
	s[0]='\0';
	str[0]='\0';
	
	
	
	int i=0;
	for (; ~PV[i]; i++) {
		assert(i<20);
		/*
		if (i%4!=0) {
			strcat(s, " - ");
		}
		//*/
		
		int from =PV[i]&127;
		int to=PV[i]>>7;
		/*
		int fx=from%8;
		int fy=((from-fx)/8);
		
		int tx=to%8;
		int ty=((to-tx)/8);
		//*/
		//fx+=alpha;
		//fy+=alpha;
		
		sprintf(str,"[%i-%i]",from,to);
		//strcat(str, s);
		
		
		consolog(str);
		
		
	}
	/*
	if (i%4) {
		consolog(str);
	}//*/
	
	consolog("----  /PV ----");
}





