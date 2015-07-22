//
//  view.c
//  Breakthrough
//
//  Created by Mikaël Dusenne on 23/07/2015.
//
//

#include "GUI.h"


// game tree
UI64 previous_board[1000];
int previous_coord[1000] , prev_count=0 , prev_mn[1000];

WINDOW *console;

char line[25][41],default_line[41];
int consoline=0;



int render_mode;


int ui_cursor,ui_selected,ui_from,ui_to;
int ui_possible_selection[3],uipsn;



void init_view(int gmmd){
	
	uipsn=0;
	ui_cursor=0;
	ui_selected=-1;
	ui_from=-1;
	ui_to=-1;
	quit=0;
	
	
	render_mode = gmmd;
	
	ui_possible_selection[0]=-1;
	ui_possible_selection[1]=-1;
	ui_possible_selection[2]=-1;

	
	/* curses */
	int i=0;
	/*
	 for (i=0; i<64; i++) {
		tab_style[i]=A_NORMAL;
	 }
	 //*/
	
	for (i=0; i<40; i++) {
		default_line[i]=' ';
	}
	for (i=0; i<10; i++) {
		strcpy(line[i]," ");
	}
	
	default_line[40]='\0';
	if (render_mode==GAMEMODE_NCURSES) {
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
		init_color(COLOR_GREEN, 200, 200, 200);
		init_pair(3, COLOR_RED, COLOR_RED);
		init_pair(4, COLOR_WHITE, COLOR_BLUE);
		
		//attron(COLOR_PAIR(1));
		printw("Welcome to Breakthrough !");
		
		console=newwin(24, 44, 0, 35);
		
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
		
		move(0, 0);
		box(console, 0, 0);
		wrefresh(console);
		
		curs_set(0);
		timeout(timeout_val);
	}

	
}

void print_tournament_scores(int scores[],int max){
	mvprintw(20, 1, "TOURNAMENT SCORES (%i/%i):    ",scores[0]+scores[1],max);
	mvprintw(22, 20,"%i - %i    ",scores[0],scores[1]);
}

void refresh_me(){
	refresh();
}

////////////////////////////////////////////////////// disp

void render(UI64 bits,int shall_draw_board){
	
	if (render_mode==GAMEMODE_NCURSES) {
		
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
		for (k=0; k<22; k++) {
			if (i==-1) {i=23;}
			
			mvwprintw(console, 22-k, 1, line[i]);
			
			i--;
		}
		
		refresh();
		wrefresh(console);
	}
	
	else {
		
		
		int i=0,j;
		printf("\n        +---+---+---+---+---+---+---+---+\n        ");
		char c,cc;
		for (; i<64; i++) {
			
			cc=' ';
			if(i==last_from || i==last_to){
				cc='.';
			}else if (i==ui_cursor && pl_type[turn_of]==PL_HUMAN) {
				cc='%';
			}else if (i==ui_selected) {
				cc=':';
			}
			
			for (j=0; j<3; j++) {
				if (i==ui_possible_selection[j]) {
					cc='_';
				}
			}
			
			
			if ((w>>i)&1) {
				c='W';
			}else if ((b>>i)&1) {
				c='B';
			}else {
				c=' ';
			}
			printf("|%c%c%c",cc,c,cc);
			
			
			if (i%8==7) {
				printf("|\n        +---+---+---+---+---+---+---+---+\n        ");
			}
		}
		
		
		
		
		
	}
	
	
	
	
}

void consolog(char* str){
	if (render_mode==GAMEMODE_NCURSES) {
		strcpy(line[consoline],default_line);
		strncpy(line[consoline],str,strlen(str));
		consoline=(consoline+1)%24;
	}else {
		printf("\n%s",str);
	}
	
}

////////////////////////////////////////////////////// disp

void render_to_file(FILE *f){
	
	fprintf(f,"\n\n");
	int i=0,j=0;
	fprintf(f,"                          +---+---+---+---+---+---+---+---+\n                          ");
	char c,cc;
	for (; i<64; i++) {
		
		cc=' ';
		if(i==last_from || i==last_to){
			cc='.';
		}else if (i==ui_cursor && pl_type[turn_of]==PL_HUMAN) {
			cc='%';
		}else if (i==ui_selected) {
			cc=':';
		}
		
		for (j=0; j<3; j++) {
			if (i==ui_possible_selection[j]) {
				cc='_';
			}
		}
		
		
		if ((w>>i)&1) {
			c='W';
		}else if ((b>>i)&1) {
			c='B';
		}else {
			c=' ';
		}
		
		fprintf(f,"|%c%c%c",cc,c,cc);
		
		if (i%8==7) {
			fprintf(f,"|\n                          +---+---+---+---+---+---+---+---+\n                          ");
		}
		
	}
	
}



void print_all_values(UI32 l[],int all_values[]){
	int i=0;
	for (; ~l[i]; i++) {}
	
	int k=0;// , cc=0;
	char str[40];
	for (k=1; k<i; k+=2) {
		char sst[2][20];
		int j=-1;
		for (; j<1; j++) {
			
			int mv = l[k+j];
			int from = mv&127;
			int to = mv>>7;
			
			sprintf(sst[j+1],"[%i->%i] : %i",from,to,all_values[k+j]);
			
		}
		
		
		sprintf(str,"%s | %s",sst[0],sst[1]);
		consolog(str);
	}
	k--;
	if (k<i) {
		int mv = l[k];
		int from = mv&127;
		int to = mv>>7;
		sprintf(str,"[%i->%i] : %i",from,to,all_values[k]);
		consolog(str);
	}
	
}




void quit_view(){
	
	delwin(console);
	endwin();
	
}



void pause_wait_key(){
	timeout(-1);
	int c=getch();
	if (c=='q') {
		quit=1;
	}
	timeout(timeout_val);
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
			case 'k':
				//gen_alphabeta(turn_of);
				consolog("nope.");
				break;
			case 'z':
				if (prev_count>0) {
					prev_count--;
					move_number=prev_mn[prev_count];
					b=previous_board[prev_count*2];
					w=previous_board[prev_count*2+1];
					last_from=previous_coord[prev_count*2];
					last_to=previous_coord[prev_count*2+1];
				}
				
				return -1;
				break;
			default:
				break;
		}
		render(0, 1);
		
	}
	
	
	if (~ans) {
		prev_mn[prev_count]=move_number;
		previous_board[2*prev_count]=b;
		previous_board[2*prev_count+1]=w;
		previous_coord[2*prev_count]=last_from;
		previous_coord[2*prev_count+1]=last_to;
		prev_count++;
		
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


