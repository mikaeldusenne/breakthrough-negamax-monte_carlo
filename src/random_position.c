/*
 *  random_position.c
 *  Breakthrough
 *
 *  Created by Mikaël Dusenne on 26/07/15.
 *  Copyright 2015 __MyCompanyName__. All rights reserved.
 *
 */

#include "random_position.h"
#include "AI_negamax.h"

int sbf=0;

int generate_random_position(int plies,int should_be_fair){
	sbf=should_be_fair;
	int v;
	//int i=0;
	//char str[40];
	do {
		v=play_random(plies);
		//i++;
		//sprintf(str, "%i , v = %i",i,v);
		//consolog(str);
		//render(0, 1);
	} while (should_be_fair && (v<-2000 || v>2000));
	return v;
}

int play_random(int prof){
	
	if (prof==0) {
		return sbf?negamax(5, -INF, INF, turn_of):0;
	}
	UI32 l[50];
	all_possible_moves(l);
	int i=-1;
	for (; ~l[i+1]; i++) {}
	UI32 mv=l[rand()%i];
	int cp = play(mv);
	int v=play_random(prof-1);
	if (v<-2000 || v>2000) {
		undo(mv, cp);
	}
	return v;
	
}
