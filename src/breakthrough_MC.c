#include "lib.h"

#include <ncurses.h>

/*  VAR  */

/* Game */

#define timeout_val -1

#define PL_HUMAN 0
#define PL_COMPUTER 1

#define PL_BLACK 0
#define PL_WHITE 1

int turn_of=PL_BLACK;
int pl_type[2]={0};
int move_number=0;

UI64 mask,MASK_LEFT,MASK_RIGHT,w,b,ul,u,ur;

/*	UI  */
#define HIGHLIGHT_CURSOR A_STANDOUT
#define SELECTED_CURSOR (A_BOLD | A_UNDERLINE)

UI64 tab_style[64];

int ui_cursor=0,ui_selected=-1,ui_from=-1,ui_to=-1,quit=0;
int ui_possible_selection[3],uipsn=0;

char line[10][41],default_line[41];
int consoline=0;

// inline int bit_first(uint64 b) { assert(b != 0); return __builtin_ctzll(b); }
// inline int bit_last(uint64 b) { assert(b != 0); return __builtin_clz(b); }
// inline uint64 bit_rest(uint64 b) { assert(b != 0); return (b & (b - 1)); }


int last_from=-1;
int last_to=-1;

/*    AI     */

#define INF 99999999

int file_n[64] = { 
	0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 ,
	0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 ,
	0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 ,
	0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 ,
	0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 ,
	0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 ,
	0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 ,
	0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 };

int rank_n[64] = {
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 
	2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 
	3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 
	4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 
	5 , 5 , 5 , 5 , 5 , 5 , 5 , 5 , 
	6 , 6 , 6 , 6 , 6 , 6 , 6 , 6 , 
	7 , 7 , 7 , 7 , 7 , 7 , 7 , 7 ,
	//	8 , 8 , 8 , 8 , 8 , 8 , 8 , 8 
};


/* FUNCTIONS */
void init();
void render(UI64 bits,int shall_draw_board);

void play(UI32 mv);
void play_real(UI32 mv);
void get_possible_moves(int i,int list[]);
int check_win();

/* bitboards */

UI64 rank(int n);
UI64 file(int n);
// int count(UI64 bits);

/* AI */

UI32 gen_move(int color);
int negamax(int prof,int alpha,int beta,int col);
int eval0(int col);
int eval1(int col);


/*  UI  */
void pause_wait_key();
int get_user_input();
int press_enter();
void press_esc();
void press_arrow(int dir);
void consolog(char * str);
WINDOW *console;

//FILE *my_file_with_errors;


int main (int argc, const char * argv[]) {
	//timeout(-1);
	
	
	init();
	
	
	//UI64 moves=(((b<<8)&mask)&(~w) | ((b<<9)&MASK_LEFT) | ((b<<7)&MASK_RIGHT)) & (~b);
	
	//printf("\n%s",byte_to_binary(moves));
	
	render(0,1);
	
	timeout(timeout_val);
	UI32 cm;
	char strt[40];
	
	while (!quit) {
		//fprintf(my_file_with_errors, "\nmove %i",move_number);
		
		switch (pl_type[turn_of]) {
			case PL_HUMAN:
				cm=get_user_input();
				
				sprintf(strt,"Human plays %i->%i",cm&127,cm>>7);
				consolog(strt);
				break;
			case PL_COMPUTER:
				cm = gen_move(turn_of);
				//pause_wait_key();
				sprintf(strt,"Computer plays %i->%i",cm&127,cm>>7);
				consolog(strt);
				//*
				if ((cm&127)<0 || (cm&127)>64 || (cm>>7)<0 || (cm>>7)>64) {
					sprintf(strt,"Error with mv %i",cm);
					consolog(strt);
					//fprintf(my_file_with_errors,"Error with move %s",byte_to_binary((UI64)cm));
				}//*/
				
				break;
			default:
				break;
		}
		if (cm!=-1) {
			play(cm);
		}
		move_number++;
		char str[20];
		sprintf(str,"---- move %i ----",move_number);
		consolog(str);
		
		render(0, 1);
		
		if (~check_win()) {
			mvprintw(2, 40, "Winner: %s    ",(turn_of^3==PL_BLACK?"Black":"White"));
			timeout(-1);
			//while(getch()!='q'){};
			getch();
			quit=1;
		}
		
	}
	delwin(console);
	endwin();
	
	//render(MASK_LEFT, 0);
	//render(MASK_RIGHT, 0);
	//fprintf(my_file_with_errors, "\n------------------------\n");
	//fclose(my_file_with_errors);
	
    return 0;
}

void pause_wait_key(){
	timeout(-1);
	int c=getch();
	if (c=='q') {
		quit=1;
	}
	timeout(timeout_val);
}


void init(){
	
	
	//fclose(ferr);
	//ferr=fopen("log_errors.txt", "a+");
	
	srand(time(NULL));
	
	//pl_type[1]=PL_COMPUTER;
	pl_type[1]=PL_HUMAN;
	//pl_type[1]=PL_HUMAN;
	pl_type[0]=PL_COMPUTER;
	
	/* curses */
	int i=0;
	/*
	for (i=0; i<64; i++) {
		tab_style[i]=A_NORMAL;
	}
	//*/

	
	
	initscr();
	
	//my_file_with_errors=fopen("my_perso_log_errors.txt", "r+");
	//fprintf(my_file_with_errors, "TestF:\n");
	
	
	cbreak();
	noecho();
	refresh();
	//timeout(100);
	keypad(stdscr, TRUE); 
	
	start_color();
	use_default_colors();
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	init_pair(1, -1, -1);
	
	init_color(COLOR_RED, 400, 400, 400);
	init_color(COLOR_BLUE, 100, 100, 100);
	init_pair(3, COLOR_RED, COLOR_RED);
	init_pair(4, COLOR_WHITE, COLOR_BLUE);
	
	//attron(COLOR_PAIR(1));
	printw("Welcome to Breakthrough !");
	
	console=newwin(12, 44, 8, 35);
	for (i=0; i<40; i++) {
		default_line[i]=' ';
	}
	default_line[40]='\0';
	refresh();
	//box(window, 0, 0);
	
	

	
	for (i=0; i<=8; i++) {
		mvhline(i*2+1, 1, 0, 32);
	}
	//mvvline(2, 1, 0, 15);
	//mvvline(2, 8*4+1, 0, 15);
	//*
	for (i=0; i<=8; i++) {
		mvvline( 2, i*4+1, 0, 15);
	}
	
	for (i=0; i<=8; i++) {
		int j=0;
		for (; j<=8; j++) {
			mvprintw(1+j*2, 1+i*4, "%c",'+');
		}
	}
	//*/
	
	//wrefresh(window);
	/* Bitboard */
	
	mask=0xffffffffffffffff;
	
	w=0;
	b=0;
	
	UI64 b1=1;
	
	for (i=0; i<16; i++) {
		
		b|=b1<<i;
		w|=b1<<(63-i);
		
	}
	
	MASK_LEFT=mask;
	MASK_RIGHT=mask;
	
	for (i=0; i<64; i++) {
		
		if (i%8==0) {
			MASK_LEFT ^= b1<<i;
		}else if (i%8==7) {
			MASK_RIGHT ^= b1<<i;
		}
		
	}
	
	ui_possible_selection[0]=-1;
	ui_possible_selection[1]=-1;
	ui_possible_selection[2]=-1;
	
	//printf("%s\n",byte_to_binary(b));
	
	for (i=0; i<10; i++) {
		strcpy(line[i]," ");
	}
	move(0, 0);
	box(console, 0, 0);
	wrefresh(console);
	
	curs_set(0);
	
	
}



////////////////////////////////////////////////////// disp

void render(UI64 bits,int shall_draw_board){
	
	//printf("\n\n");
	int i=0,x=0,y=0,j=0;
	
	char *c;
	for (; i<64; i++) {
		
		//*
		for (j=0; j<3; j++) {
			if (i==ui_possible_selection[j]) {
				attron(COLOR_PAIR(3));
			}
		}
		//*/
		
		if(i==last_from || i==last_to){
			attron(COLOR_PAIR(4));
		}else if (i==ui_cursor && pl_type[turn_of]==PL_HUMAN) {
			attron(HIGHLIGHT_CURSOR);
			attron(COLOR_PAIR(1));
		}else if (i==ui_selected) {
			attron(SELECTED_CURSOR);
			attron(COLOR_PAIR(1));
		}
		
		
		
		if ((bits>>i)&1) {
			c="x";
		}else if (shall_draw_board && (w>>i)&1) {
			c="W";
		}else if (shall_draw_board && (b>>i)&1) {
			c="B";
		}else {
			c=" ";
		}
		
		x=((i%8));
		y=((i-x)/8);
		
		//x+=1;
		x*=4;
		x+=3;
		y*=2;
		y+=2;
		
		move( y, x);
		printw(c);
		
		move( y, x-1);
		printw(" ");
		move( y, x+1);
		printw(" ");
		
		attroff(COLOR_PAIR(3));
		attroff(COLOR_PAIR(4));
		attroff(HIGHLIGHT_CURSOR);
		attroff(SELECTED_CURSOR);
		
	}
	//move(0, 0);
	
	//mvprintw(2, 40, "MOVE %i   ",move_number);
	//mvprintw(4, 40, "%s's turn    ",(turn_of==PL_BLACK?"Black":"White"));
	
	
	i=consoline-1;
	int k=0;
	for (k=0; k<10; k++) {
		if (i==-1) {i=9;}
		
		mvwprintw(console, 10-k, 1, line[i]);
		
		i--;
	}
	
	refresh();
	wrefresh(console);
}

void consolog(char* str){
	strcpy(line[consoline],default_line);
	strncpy(line[consoline],str,strlen(str));
	consoline=(consoline+1)%10;
}

////////////////////////////////////////////////////// user input



int get_user_input(){

	int dir=0,ans=-1;
	
	while (ans==-1 && !quit) {
		dir=getch();
		
		switch (dir) {
			case KEY_UP:
				press_arrow(-8);
				
				break;
			case KEY_RIGHT:
				press_arrow(1);
				break;
			case KEY_DOWN:
				press_arrow(8);
				break;
			case KEY_LEFT:
				press_arrow(-1);
				break;
			case KEY_ENTER:
			case ' ':
				ans = press_enter();
				break;
			case 27:
				press_esc();
				break;
			case 'q':
				quit=1;
				break;
				
			default:
				break;
		}
		render(0, 1);
		
	}
	
	return ans;
}

void press_esc(){
	if (ui_selected!=-1) {
		ui_cursor=ui_selected;
		ui_selected=-1;
		ui_possible_selection[0]=-1;
		ui_possible_selection[1]=-1;
		ui_possible_selection[2]=-1;
	}
}

int press_enter(){
	
	if (ui_selected==-1) {
		
		//ui_cursor++;
		//*
		UI64 bits=(turn_of==PL_BLACK?b:w);
		if((bits>>ui_cursor)&1){
			ui_selected=ui_cursor;
			get_possible_moves(ui_cursor, ui_possible_selection);
			if (!(ui_possible_selection[0]==-1 && ui_possible_selection[1]==-1 && ui_possible_selection[2]==-1)) {
				press_arrow(1);
			}else {
				ui_selected=-1;
			}
			
		}
		
		//*/
	}else {
		int ans=ui_selected|(ui_cursor<<7);
		ui_selected=ui_cursor;
		press_esc();
		
		return ans;
	}
	
	return -1;
}

void press_arrow(int dir){
	
	if (ui_selected==-1) {
		switch (dir) {
			case 1:
				if (ui_cursor%8==7) {
					ui_cursor-=7;
				}else {
					ui_cursor++;
				}
				break;
			case -1:
				if (ui_cursor%8==0) {
					ui_cursor+=7;
				}else {
					ui_cursor--;
				}
				break;
			case 8:
			case -8:
				ui_cursor=(64+ui_cursor+dir)%64;
				break;
			default:
				break;
		}
	}else {
		
		uipsn=(3+uipsn+(dir<0?1:-1)*(turn_of==PL_BLACK?1:-1))%3;
		while (ui_possible_selection[uipsn]==-1) {uipsn=(3+uipsn+(dir<0?1:-1))%3;}
		ui_cursor=ui_possible_selection[uipsn];
		
		//mvprintw(4, 40,"--> possible: %i ; %i ; %i",ui_possible_selection[0],ui_possible_selection[1],ui_possible_selection[2]);
		//mvprintw(7, 40, "current = %i   ",uipsn);
	}
	
	
}

///////////////////////////////////////////*   GAME LOGIC  */



void all_possible_moves(UI32 list[]){
	
	UI32 n=0,m;
	
	if (turn_of==PL_BLACK) {
		
		UI64 bits = (b<<8) & mask & ~(b|w);
		for (; bits!=0; bits=bit_rest(bits)) {
			m=bit_first(bits);
			list[n++]=(m-8) | (m<<7);
		}
		
		
		for (bits = (b<<9) & MASK_LEFT & ~b  ; bits!=0; bits=bit_rest(bits)) {
			m=bit_first(bits);
			list[n++]=(m-9) | (m<<7);
		}
		
		
		for (bits = (b<<7) & MASK_RIGHT & ~b  ;   bits!=0; bits=bit_rest(bits)) {
			m=bit_first(bits);
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
	
}

void get_possible_moves(int i,int list[]){ // for one particular position
	
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

void play(UI32 mv){
	
	UI32 to=(mv>>7);
	
	if (turn_of==PL_BLACK) {
		b^=(1ULL<<(mv&127))|(1ULL<<to);
		w&=~(1ULL<<to);
	}else {
		w^=(1ULL<<(mv&127))|(1ULL<<to);
		b&=~(1ULL<<to);
	}
	
	turn_of^=1;
}

void play_real(UI32 mv){
	play(mv);
	
	UI32 from=mv&127;
	UI32 to=(mv>>7);
	last_to=to;
	last_from=from;
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
///////////////////////////////////////////*   BITBOARD   */

UI64 rank(int n){
	
	return 0xFFULL<<(8*n);
	
}

UI64 file(int n){
	
	return 0x0101010101010101<<n;
	
}




////////////////////////////////////////////////////// AI


int computer_col,nodecount;

UI32 gen_move(int color){
	
	UI32 lbm[100];
	int lbmc=0;
	
	computer_col=color;
	nodecount=0;
	int prof_max=3;
	
	float time_begin=clock();
	float diff=0;
	
	UI32 l[50]={-1};
	all_possible_moves(l);
	
	//printf("\nclock() == %ld",clock());
	int max_val;
	
	if (l[1]==-1) {return l[0];} // only one possible move
	
	//int capture=0;
	UI64 bb=b,ww=w;
	//UI32 best_move=~0;
	//(color==PL_BLACK?1000:20)
	while (prof_max<4 &&   (diff)<(color==PL_BLACK?.6:.6)){
		prof_max++;
		
		max_val=-INF;
		int alpha=-INF;
		int beta=INF;
		int v=-INF;
		int i=0;
		
		
		for (i=0; l[i]!=-1; i++) {
			play(l[i]);
			v=-negamax(prof_max, -beta, -alpha, 1-color);
			turn_of^=1;
			b=bb;w=ww;
			if (v>max_val) {
				max_val=v;
				lbmc=0;
				lbm[lbmc++]=l[i];
				
				//best_move=l[i];
			}
			if (v==max_val) {
				lbm[lbmc++]=l[i];
			}
			if (v>alpha) {
				alpha=v;
			}
		}
		diff=(((float)clock()-(float)time_begin)/1000000.0F);
	}
	
	
	
	while ((((float)clock()-(float)time_begin)/1000000.0F)<2) {}
	/*
	printf("\n---- move %i ----",move_number);
	
	printf("\nbest_val == %i",max_val);
	printf("  |  %iM NPS",(int)((float)(nodecount)/(1000*diff)));
	printf("  |  prof == %i",prof_max);
	printf("  |  Time == %f",diff);
	printf("  |  Nodes == %i",nodecount);
	//*/
	return lbm[rand()%lbmc];
	
	
}

int negamax(int prof,int alpha,int beta,int col){
	nodecount++;
	
	int win=check_win();
	if (~win) {
		return (win==col?1:-1)*(1000000+prof);
	}
	
	if (prof<=0) {
		return (computer_col==PL_BLACK?eval1(col):eval0(col));
	}
	
	int max_val=-INF;
	int v=-INF;
	UI32 l[50]={-1};
	all_possible_moves(l);
	int i=0;//,capture=0;
	UI64 bb=b,ww=w;
	
	for (; l[i]!=-1; i++) {
		
		//if ((col==PL_BLACK?w:b)&(1ULL<<(l[i]>>7))) {capture=1;}
		
		play(l[i]);
		v=-negamax(prof-1,-beta,-alpha,1-col);
		max_val=max(max_val,v);
		if (v>alpha) {
			alpha=v;
			if (alpha>beta) {
				w=ww;b=bb;
				turn_of^=1;
				return v;
			}
		}
		w=ww;b=bb;
		turn_of^=1;
		//capture=0;
	}
	
	
	return max_val;
}

int eval0(int col){
	int score = 0;
	
	UI64 bw=w,bb=b;
	int wc=count(bw),bc=count(bb);
	
	
	score+=10*(col==PL_BLACK?bc-wc:wc-bc);
	
	score -= bit_first(w)+bit_last(b);
	
	int bit_last_b=bit_last(b),bit_first_w=bit_first(b);
	
	UI64 tmp_b = 0;
	
	if (file_n[bit_first_w]>0) {
		tmp_b = file(file_n[bit_first_w]-1);
	}
	if (file_n[bit_first_w]<7) {
		tmp_b |= file(file_n[bit_first_w]+1);
	}
	
	if ( ((1ULL<<(rank_n[bit_first_w])*8)-1) & tmp_b & w  == 0) {
		score+=1000;
	}
	
	tmp_b = 0;
	if (file_n[bit_last_b]>0) {
		tmp_b = file(file_n[bit_last_b]-1);
	}
	if (file_n[bit_last_b]<7) {
		tmp_b |= file(file_n[bit_last_b]+1);
	}
	if (   (1ULL<<(rank_n[bit_last_b]-1)*8) & tmp_b & b == 0 ) {
		score -= 1000;
	}
	
	
	
	
	return score;
}

int eval1(int col){
	
	int score = 0;
	
	UI64 bw=w,bb=b;
	int wc=count(bw),bc=count(bb);
	
	score+=10*(col==PL_BLACK?bc-wc:wc-bc);
	
	/*
	 int i=0,sc_line=0;
	 if (col==PL_BLACK) {
	 for (; i<8; i++) {
	 sc_line += i*count(b&rank(i));
	 }
	 }
	 /*
	 else {
	 for (; i<8; i++) {
	 sc_line -= i*count(w&rank(i));
	 }
	 }
	 score+=sc_line;
	 //*/
	
	int bit_last_b=bit_last(b),bit_first_w=bit_first(b);
	
	score += bit_first_w+bit_last_b;
	
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
	
	//score += count(b&((1ULL<<bit_first(w))-1)); // dont let him go through !
	
	//score+=count( (w<<18) & (b<<9) & b & MASK_RIGHT ) + count( (w<<14) & (b<<7) & b & MASK_LEFT ); // counter attacking positions
	
	return score;
}


/* int count(UI64 bits){ */
/* 	int count; */
/* 	for (count=0; bits!=0; bits=bit_rest(bits>>bit_first(bits))) { */
/* 		count++; */
/* 	} */
/* 	return count; */
/* } */



