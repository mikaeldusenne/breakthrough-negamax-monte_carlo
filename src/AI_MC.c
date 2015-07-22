//
//  AI_MC.c
//  Breakthrough
//
//  Created by Mikaël Dusenne on 23/07/2015.
//
//
#if 0

#include "AI_MC.h"


UI32 gen_mc(int color){
	nodecount=0;
	all_nmoves=0;
	UI32 l[50]={-1};
	all_possible_moves(l);
	
	UI64 bb=b,ww=w;
	int tturnof=turn_of;
	int all_values[50];
	int i=0;
	
	int lbm[50];
	int lbmc=0;
	int max_val=-INF;
	float time_begin=clock();// , diff=time_begin;
	
	for (i=0; l[i]!=-1; i++) {
		play(l[i]);
		int j=0;
		int v=0;
		//nodecount=0;
		UI64 bbb=b , www=w;
		
		
		for (j=0; j<N_RANDOM; j++) {
			nmoves=0;
			v+=gen_random_game(color);
			turn_of=tturnof;
			b=bbb;w=www;
		}
		b=bb;w=ww;
		
		if (v>max_val) {
			max_val=v;
			lbmc=0;
			lbm[lbmc++]=l[i];
		}
		if (v==max_val) {
			lbm[lbmc++]=l[i];
		}
		
		all_values[i]=v;
	}
	
	print_all_values(l,all_values);
	
	//*
	char strt[40];
	//sprintf(strt,"---- move %i ----",move_number);
	//consolog(strt);
	sprintf(strt,"best_val == %i / %i",max_val,N_RANDOM);
	consolog(strt);
	sprintf(strt,"%f sec , %i games",(((float)clock()-(float)time_begin)/1000000.0F),nodecount);
	consolog(strt);
	sprintf(strt,"%f moves per game",((float)all_nmoves)/((float)nodecount));
	consolog(strt);
	//printf("  |  %iM NPS",(int)((float)(nodecount)/(1000*diff)));
	//printf("  |  prof == %i",prof_max);
	//sprintf(strt,"Time == %f",(((float)clock()-(float)time_begin)/1000000.0F));
	//consolog(strt);
	//printf("  |  Nodes == %i",nodecount);
	//*/
	
	
	return lbm[rand()%lbmc];
}


int gen_random_game(int col){
	nodecount++;
	
	int winner=-1;
	
	UI32 l[50];
	int lmax=0;
	
	do {
		nmoves++;
		all_possible_moves(l);
		for (lmax=0; ~l[lmax]; lmax++) {}
		
		play(l[rand()%lmax]);
		winner = check_win();
	} while (winner==-1);
	
	//render(0,1);
	//pause_wait_key();
	
	//fprintf(file_log, "\n-> %i",nodecount);
	//render_to_file(file_log);
	
	all_nmoves+=nmoves;
	
	return (winner==col?1:0);
	
}



#endif 
