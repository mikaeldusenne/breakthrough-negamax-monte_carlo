/*
 *  lib.h
 *  GSP_A
 *
 *  Created by Mikaël Dusenne on 25/06/15.
 *  Copyright 2015 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __lib_perso_mika
#define __lib_perso_mika



#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>


#include <ctype.h>

#include <stdlib.h>

#define SQ(X) ((X)*(X))

#define max(a,b) \
({ __typeof__ (a) _a = (a); \
__typeof__ (b) _b = (b); \
_a > _b ? _a : _b; })


#define min(a,b) \
({ __typeof__ (a) _a = (a); \
__typeof__ (b) _b = (b); \
_a < _b ? _a : _b; })



typedef uint32_t UI32;
typedef uint64_t UI64;
typedef uint16_t UI16;

typedef uint32_t uint32;
typedef uint64_t uint64;
typedef uint16_t uint16;

void init_lib();

void init_clocks();
void reset_clock(int i);
int start_clock();
void pause_clock(int index);
void resume_clock(int index);


float get_clock_value(int index);
float stop_clock(int index);

const char *byte_to_binary(UI64 x);
int bit_first(uint64 b);
int bit_last(uint64 b);
uint64 bit_rest(uint64 b);
uint64 bit_last_rest(uint64 bb);
int count(UI64 bits);

void bit_dump(UI64 bits);

#endif
