#include "main.h"

#define PLAYER_BLACK PL_COMPUTER | AI_ALPHABETA
//#define PLAYER_WHITE PL_COMPUTER | AI_ALPHABETA
//*
#define PLAYER_WHITE PL_HUMAN
//#define PLAYER_BLACK PL_HUMAN
//*/
#define TOURNAMENT 10
#define TOURNAMENT_NB 1

#define DRAW_ALL_MOVES 1

#define TESTING 1
#define TEST_EVAL 2
#define TEST_TT 4
#define DO_TOURNAMENT 8
#define PLAY_A_SIMPLE_GAME 16

#define MASK_TESTS 16
//#define MASK_TESTS 16

int main (int argc, const char * argv[]) {
	
	init();
	render(0, 1);
	int i=0;
	i++;i--;
#if (MASK_TESTS & TESTING)
	
	quit_view();
	
	negamax(4, -INF, +INF, PL_BLACK);
	
	assert(hash_is_correct(board_hash));
	//assert(hash_is_correct(board_hash));
	//*
	/*
	UI32 l[50];
	all_possible_moves(l);
	int cap = play(l[0]);
	
	printf("\n--------\n");
	
	bit_dump(board_hash);
	printf("\n");
	bit_dump(hash_board());
	printf("\n");
	printf("\n--------\n");
	
	undo(l[0], cap);
	
	
	printf("\n--------\n");
	
	bit_dump(board_hash);
	printf("\n");
	bit_dump(hash_board());
	printf("\n");
	printf("\n--------\n");
	//*/
	pause_wait_key();
	//*/
	/*
	while (!quit) {
		reset_board();
		int v = generate_random_position(15, 1);
		//eval1(PL_BLACK);
		char str[40];
		sprintf(str, "Eval: %i    ",v);
		consolog(str);
		//eval1(PL_WHITE);
		render(0, 1);
		pause_wait_key();
	
	}
	//*/
	
	/*
	render(diag1, 0);
	pause_wait_key();
	render(diag2, 0);
	pause_wait_key();
	//*/
	
	
	quit_app();
	return 0;
#elif (MASK_TESTS & TEST_EVAL)
	
	int* what=&eval_factor_first_piece_defended;
	int minv=400,maxv=500,nsteps=3;
	int n_loops=4;
	
	int best_val=-1;
	int best_winrate=-1;
	
	double zoom_ratio=2;
	int win_size=maxv-minv;
	
	for (i=0; i<n_loops; i++) {
		
		char str[400];
		sprintf(str, "\n-------- TOURNAMENT %i : from %i to %i --------",i,minv,maxv);
		strcat(ALL_RECORDS, str);
		
		
		int j;
		for (j=0; j<=nsteps; j++) {
			int v = minv+(j*win_size/nsteps);
			*what=v;
			mvprintw(23, 1, "%i - (%i/%i)(%i/%i)  ",*what,i+1,n_loops,j+1,nsteps+1);
			int wins = tournament(TOURNAMENT, PL_WHITE);
			if (wins>best_winrate) {
				best_winrate=wins;
				best_val=v;
			}
			
			char c=' ';
			if (j) {
				c=' ';
			}
			sprintf(str, "\n%i,%.2f ",v,100.0 * wins / ((double)TOURNAMENT));
			strcat(ALL_RECORDS, str);
			
		}
		
		win_size /= zoom_ratio;
		minv=best_val-win_size/2;
		maxv=best_val+win_size/2;
		
		
	}
	
	
	
#if 0
	//quit=1;
	
	//int n=10;
	int lim[2]={180,210},val[2]={0};
	//*
	eval_lateral_factor=lim[0];
	val[0]=tournament(TOURNAMENT, PL_WHITE);
	
	eval_lateral_factor=lim[1];
	val[1]=tournament(TOURNAMENT, PL_WHITE);
	//*/
	recur_tournament(&eval_lateral_factor, 2, max(val[0],val[1]), lim[0],lim[1],val[0],val[1]);
	
	/*
	for (eval_first_pawn=1; eval_first_pawn<4; eval_first_pawn+=1) {
		int wrw = tournament(TOURNAMENT,PL_WHITE) ;
		char str[400];
		sprintf(str, "\nTournament Result: %f WR for White with lateral_factor == %i  ( %i / %i )",wrw/ ((double)TOURNAMENT),eval_first_pawn,wrw,TOURNAMENT);
		strcat(ALL_RECORDS, str);	
	}
	//*/
#endif
	//quit=1;
	
#elif (MASK_TESTS & DO_TOURNAMENT)
	
	//int i=0;
	for (i=0; i<TOURNAMENT_NB; i++) {
		mvprintw(22, 1, "(%i/%i)  ",i+1,TOURNAMENT_NB);refresh();
		//render(0, 1);
		int v = tournament(TOURNAMENT, PL_WHITE);
		
		char str[400];
		char c='\n';
		if (i) {
			c=',';
		}
		sprintf(str, "%c%.2f ",c,v / ((double)TOURNAMENT));
		strcat(ALL_RECORDS, str);
	}
	
	
#elif (MASK_TESTS & PLAY_A_SIMPLE_GAME)
	consolog("Play a simple game");
		if (pl_type[0]!=PL_HUMAN && pl_type[1]!=PL_HUMAN) {
			generate_random_position(6,1);
		}
	
	render(0, 1);
	
		int winner=start_game();
		char str[40];
		
		sprintf(str, "Winner: %s    ",(winner==PL_BLACK?"Black":"White"));
		consolog(str);
		render(0, 1);
		pause_wait_key();
#elif (MASK_TESTS & TEST_TT)
	
	float cumul[2];
	cumul[0]=0;
	cumul[1]=0;
	
	for (i=0; i<1; i++) {
		mvprintw(22, 1, "(%i/%i)  ",i,100);
		for (USE_TRANSPOSITION_TABLE=0; USE_TRANSPOSITION_TABLE<2; USE_TRANSPOSITION_TABLE++) {
			
			reset_board();
			start_game();
			
			cumul[USE_TRANSPOSITION_TABLE]+=(pl_elapsed_time[0]+pl_elapsed_time[1])/(move_number);
			
		}
	}
	
	
	char str[40];
	sprintf(str, "Without TT : %.4f",cumul[0]);
	consolog(str);
	sprintf(str, "With TT : %.4f",cumul[1]);
	consolog(str);
	
	render(0, 1);
	pause_wait_key();
	
#endif
	
	
	//render(0, 1);
	//pause_wait_key();
	
	quit_app();
	
	
	FILE *f=fopen("tournament_results.txt", "w+");
	fprintf(f, "%s",ALL_RECORDS);
	fclose(f);
	
	
	
    return 0;
}

int recur_tournament(int *what,int prof,int best,int lim_a,int lim_b,int val_a, int val_b){
	
	if (prof==0) {
		return 0;
	}
	
	*what = (int)((lim_b+lim_a)/2);
	int v = tournament(TOURNAMENT, PL_WHITE);
	//best=max(best,v);
	
	char str[400];
	sprintf(str, "\nTournament Result: %f WR for White with lateral_factor == %i  ( %i / %i )",v / ((double)TOURNAMENT),*what,v,TOURNAMENT);
	strcat(ALL_RECORDS, str);
	
	if (v>=best) {
		recur_tournament(what,prof-1,v,lim_a,(int)((lim_b+lim_a)/2),val_a,v);
		recur_tournament(what,prof-1,v,(int)((lim_b+lim_a)/2),lim_b,v,val_b);
	}
	//*
	else if (val_a>=val_b) {
		recur_tournament(what,prof-1,best,lim_a,(int)((lim_b+lim_a)/2),val_a,v);
	}else {
		recur_tournament(what,prof-1,best,(int)((lim_b+lim_a)/2),lim_b,v,val_b);
	}
	 //*/
	
	
	return 0;
}

int tournament(int n, int white_or_black){
	
	int scores[2]={0,0};
	
	do {
		reset_board();
		generate_random_position(12,1);
		print_tournament_scores(scores,TOURNAMENT);
		//refresh();
#if DRAW_ALL_MOVES
		render(0, 1);
#else
		refresh();
#endif
		scores[start_game()]++;
#if !DRAW_ALL_MOVES
		//render(0, 1);
#endif
		
	} while (scores[0]+scores[1]<TOURNAMENT);
	print_tournament_scores(scores,TOURNAMENT);
	/*char str[400];
	sprintf(str, "\n(%i / %i) == %f",scores[white_or_black],n,(white_or_black==PL_BLACK?1:-1)*(((scores[white_or_black]))/((float)n)));
	strcat(ALL_RECORDS, str);	
	//*/
	return scores[white_or_black];
}

int start_game(){
	int winner;
	UI32 cm;
	char strt[40];
	
	while (!quit && !endgame) {
		char str[40];
		sprintf(str,"---- move %i ----  %s's turn",move_number,turn_of==PL_BLACK?"Black":"White");
		consolog(str);
		
		int cl;
		
		if (turn_of==PL_WHITE) {
			USE_TRANSPOSITION_TABLE=0;
		}else{
			USE_TRANSPOSITION_TABLE=0;
		}
		
		switch (pl_type[turn_of]&1) {
			case PL_HUMAN:
				
				cm=get_user_input();
				if (~cm) {
					sprintf(strt,"Human plays %i->%i",cm&127,cm>>7);
					consolog(strt);
				}
				break;
			case PL_COMPUTER:
				cl = start_clock();
				cm = gen_alphabeta(turn_of);
				pl_elapsed_time[turn_of]+=stop_clock(cl);
				
				//pause_wait_key();
				sprintf(strt,"Computer plays %i->%i ( %.2f )",cm&127,cm>>7,pl_elapsed_time[turn_of]/1000);
				consolog(strt);
				//*
				if ((cm&127)>64 || (cm>>7)>64) {
					sprintf(strt,"Error with mv %i",cm);
					consolog(strt);
				}//*/
				
				break;
			default:
				break;
		}
		if (cm!=-1) {
			play_real(cm);
		}
		
		if (DRAW_ALL_MOVES) {
			render(0, 1);
		}
		
		
		if (~check_win()) {
			winner=check_win();
			endgame=1;
		}
		
	}
	return winner;
	
}

void quit_app(){
	
	quit_view();
	
}



void init(){
	
	init_lib();
	
	
	init_board();
	
	init_ai_negamax();
	
	
	init_game_model(PLAYER_BLACK,PLAYER_WHITE);
	//init_game_model(PL_COMPUTER | AI_ALPHABETA  , PL_HUMAN );
	
	init_view(GAMEMODE_NCURSES);
	
	
}






