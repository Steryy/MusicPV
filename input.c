#include "input.h"
#include <ncurses.h>
#include "dstruct.h"
bool run = true;
bool move_d = true;
bool move_file = false;

int mouse_position = 1;
int p_hi = 0;
int file_hi = 0;
int player = -1;
bool pitch = false;
struct file * chosen_file = NULL;
double speed = 1;
double volume = 100;
bool playlis = true;
int filnumber = 0;
int pl_size ;
int fi_size;
int fo_size;
struct playlist *playlists;
struct file * files;
char ** folders;
WINDOW * info;
WINDOW * play;
WINDOW * input;
WINDOW * mainwin;
struct playlist *chosen_playlist;
struct playlist * loaded;
int play_sel = -1;
void _refresh(WINDOW * win){
wmove(input,0,mouse_position);
wrefresh(win);

if (win != input){

wrefresh(input);
}
}

