

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "helpers.h"
#include "input.h"
#include "dstruct.h"
 char* DIRPATH= "dirs";
 char* FILEPATH= "files";
 char* PLAYLISTPATH= "playlists";
char* CONFIGDIR="./config/musicpv";

void playlist_move(struct playlist ** chosen , struct playlist * plays, WINDOW * play, int p_hi,int pl_size, struct playlist * loaded,int show ) {
	int hi = p_hi;
	int size = pl_size;

	werase(play);
	create_border(play,'p');
	int min = 0;
	if (size - hi < (LINES - 7) / 2)
		min = size - (LINES - 7);
	else if (hi > (LINES - 7) / 2)
		min = hi - (LINES - 7) / 2;
	if (min < 0) min = 0;
	int max = (LINES - 7) + min;
	if (max > size) max = size;

	for (int i = min; i < max; i++) {
	char g = ' ';
	if (i == play_sel && i !=0)
		g= '*';
		if (i == hi) {
			*chosen = &plays[i];

			if (show ==1){
				/* wattron(play, COLOR_PAIR(4)); */
				if (&plays[i] != loaded)
				wattron(play,COLOR_PAIR(5));
					else

				wattron(play,COLOR_PAIR(2));}
			else{

				if (&plays[i] != loaded)
				wattron(play, COLOR_PAIR(6));
			else

				wattron(play, COLOR_PAIR(4));}
		} else{

				if (&plays[i] != loaded)
			wattron(play, COLOR_PAIR(7));
		else

			wattron(play, COLOR_PAIR(3));
		}char be[COLS/3];
		for (int i = 0 ; i < COLS/3-2 ;i++){
be[i] = ' ';
		}
be[COLS/3-2] = '\0';
		mvwaddstr(play,i+1-min, 1,be);
		mvwprintw(play, i - min + 1, 1, " %c%s",g,plays[i].name);
	}
}

void c_toin(int *e, char *g, int h) {
	g[*e] = h / 10 + '0';
	*e += 1;
	g[*e] = h % 10 + '0';
	*e += 1;
}
char *to_human_redable(int t) {
	char *stack = malloc(sizeof(char) * 13);

	int p = 0;
	if (t < 60) {
		c_toin(&p, stack, 0);

		stack[p++] = ':';
		c_toin(&p, stack, t);

		stack[p++] = '\0';
		return stack;
	}

	int h, m, s;

	h = t / 3600;
	t = t % 3600;
	m = t / 60;
	s = t % 60;
	if (h > 0) {

		c_toin(&p, stack, h);

		stack[p++] = ':';
	}
	c_toin(&p, stack, m);
	/* put all chars to stack in reverse order ;) */

	stack[p++] = ':';

	c_toin(&p, stack, s);
	stack[p++] = '\0';
	return stack;
}

void create_border(WINDOW *win, char type) {
	int bot = (int)' ';

	wattron(win, COLOR_PAIR(1));
	if (type == 'i')
		wborder(win, bot, bot, 0, 0, 0, 0, 0, 0);

	else if (type == 'p')
		wborder(win, 0, bot, 0, bot, 0, bot, bot, bot);

	else
		wborder(win, 0, bot, 0, bot, 0, bot, bot, bot);
	wattroff(win, COLOR_PAIR(1));
}
void print_file( char * fol , char * ty, int i ){


		char be[COLS*2/3];
		for (int h = 0 ; h < COLS*2/3-1 ;h++){
be[h] = ' ';
		}
be[COLS*2/3-1] = '\0';

		mvwaddstr(mainwin,i+1, 1,be);
	int e = strlen(fol);
		for (int h = 0 ; h < e;h++){

be[h] = fol[h];
		}
		int g = strlen(ty);
if (g +e > COLS*2/3-10)
	g = COLS*2/3-10-e;
for (int h = e; h< e+g ; h++)
	be[h] = ty[h-e];

mvwaddnstr(mainwin,i +1,2,be,COLS*2/3-10 );

}
void print_files(struct playlist * chosen_playlist, struct file * files,int fi_size,int file_hi,struct file * chosen_file  , int show ) {
	if (!chosen_playlist)
		return;

	int filnum = chosen_playlist->filnum;
	werase(mainwin);
	create_border(mainwin,'m');
int max_dur = 0;
if (filnum == 0)
	return;
	struct file *temp;
	int size = fi_size;
	int hi = file_hi;
	if (filnum > 0) {
		size = filnum;
	}

	int min = 0;
	if (size - hi < (LINES - 7) / 2)
		min = size - (LINES - 7);
	else if (hi > (LINES - 7) / 2)
		min = hi - (LINES - 7) / 2;
	if (min < 0) min = 0;
	int max = (LINES - 7) + min;
	if (max > size) max = size;
	for (int i = 0; i < size; i++) {
		if (filnum <= 0) {
			temp = &files[i];
		} else {
			temp = &files[chosen_playlist->files[i]];
		}
		if (temp->dur)
			max_dur += temp->dur;

if (i < min || i > max)
	continue;

		if (i == file_hi) {
			if (!show){
				wattron(mainwin, COLOR_PAIR(5));

			if (temp == chosen_file)
				wattron(mainwin, COLOR_PAIR(2));
			}
			else {

				wattron(mainwin, COLOR_PAIR(6));


			if (temp == chosen_file)
				wattron(mainwin, COLOR_PAIR(4));
			}
		} else if (temp == chosen_file)

			wattron(mainwin, COLOR_PAIR(3));
		else
			wattron(mainwin, COLOR_PAIR(7));

print_file(temp->fol,temp->name,i - min);
		if (temp->dur <5000 && temp->dur >0){
char *dur = to_human_redable((int)temp->dur);
int e = strlen(dur);
		mvwprintw(mainwin, i - min + 1, COLS*2/3-e-2, "%s ",dur );
		}
	}
char * dur_max = to_human_redable(max_dur);
int len = strlen(dur_max);
wattroff(mainwin, COLOR_PAIR(1));
	mvwaddstr(mainwin,0,COLS*2/3-len-2,dur_max);
}
