CC=clang
# CFLAGS=-I.
CFLAGS= -O3 `pkg-config --cflags --libs ncurses`

mc:
	cd src && $(CC) -o ../breakthrough breakthrough_MC.c AI_MC.c lib.c ${CFLAGS}

negamax:
	cd src && $(CC) -o ../breakthrough \
		AI_negamax.c board.c eval.c game_model.c GUI.c lib.c main.c random_position.c \
		transposition_table.c ${CFLAGS}
