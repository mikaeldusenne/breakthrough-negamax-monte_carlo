/*
 *  lib.c
 *  GSP_A
 *
 *  Created by Mikaël Dusenne on 25/06/15.
 *  Copyright 2015 __MyCompanyName__. All rights reserved.
 *
 */

#include "lib.h"

#define CLOCK_ARRAY_LENGTH 100

typedef struct my_clock{
	
	clock_t start_time;
	clock_t paused_time;
	clock_t skip_time;
	char in_use;
	
} my_clock;

my_clock all_clocks[CLOCK_ARRAY_LENGTH];

void init_lib(){
	srand(time(NULL));
	
	init_clocks();
}

void init_clocks(){
	
	int i=0;
	for (; i<CLOCK_ARRAY_LENGTH; i++) {
		reset_clock(i);
	}
	
}

void reset_clock(int i){
	my_clock c;
	c.in_use=0;
	c.start_time=0;
	c.paused_time=0;
	c.skip_time=0;
	all_clocks[i]=c;
}

int start_clock(){ // finds a non-used clock, sets it's value and returns corresponding index in array

	int i=0,k=0;
	for (; all_clocks[i].in_use; i = (i+1)%CLOCK_ARRAY_LENGTH) {
		k++;
		assert(k<CLOCK_ARRAY_LENGTH);
	}
	
	reset_clock(i);
	my_clock *c = &all_clocks[i];
	
	c->in_use=1;
	c->start_time=clock();
	
	return i;
}

float get_clock_value(int index){ // returns elapsed time in millisec
	
	my_clock *c = &all_clocks[index];
	assert(c->in_use);
	
	int was_paused=c->paused_time==0?0:1;
	
	if (was_paused) {
		resume_clock(index);
	}
	
	float ans = (((clock() - c->start_time) - c->skip_time) / 1000.0F );
	
	if (was_paused) {
		pause_clock(index);
	}
	
	return ans;
	
}

void pause_clock(int index){
	my_clock *c = &all_clocks[index];
	assert(c->in_use && !c->paused_time);
	c->paused_time=clock();
	
}

void resume_clock(int index){
	my_clock *c = &all_clocks[index];
	assert(c->in_use && c->paused_time);
	c->skip_time+=(clock() - c->paused_time);
	c->paused_time=0;
}


float stop_clock(int index){
	assert(all_clocks[index].in_use);
	float ans = get_clock_value(index) ;
	
	reset_clock(index);
	
	return ans;
	
}


const char *byte_to_binary(UI64 x)
{
    static char string_bit[65];
    string_bit[0] = '\0';
	
    UI64 z;
	for (z = (1ULL<<(x>0xFFFFFFFFFFFFFFFFULL?63:31)); z > 0; z >>= 1)
    {
        strcat(string_bit, ((x & z) == z) ? "1" : "0");
    }
	
    return string_bit;
}


int bit_first(uint64 b) { assert(b != 0); return __builtin_ctzll(b); }
int bit_last(uint64 bits) {
	assert(bits != 0);
	return bits>0xFFFFFFFF?(63-__builtin_clz(bits>>32)) : 31-__builtin_clz(bits);
}
uint64 bit_rest(uint64 b) { assert(b != 0); return (b & (b - 1)); }
uint64 bit_last_rest(uint64 bb){assert(bb!=0);return (bb ^ (1ULL<<(bit_last(bb))));}





int count(UI64 bits){
	
	return __builtin_popcountll(bits);
	/*
	 if (bits>0xFFFFFFFF) {
		return __builtin_popcount(bits)+__builtin_popcount(bits>>32);
	 }else {
		return __builtin_popcount(bits);
	 }
	 //*/
	
	/*
	 int count;
	 for (count=0; bits!=0; bits=bit_rest(bits>>bit_first(bits))) {
		count++;
	 }
	 return count;
	 //*/
}


void bit_dump(UI64 bits){
	//printf("\n");
	//recur_bit_dump(bits);
	//printf("\n");
	
	UI64 z=0;
	
	for (z=(1ULL<<(bits>0xFFFFFFFFFFFFFFFFULL?63:31)); z ; z>>=1) {
		printf((bits&z)?"1":"0");
	}
	
}






