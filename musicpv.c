// Build with: gcc -o simple.so simple.c `pkg-config --cflags mpv` -shared -fPIC
#include <locale.h>
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include "init.h"
#include "helpers.h"
#include "commands/file.h"

#include "input.h"


int main(){

	 char*HOME = getenv("HOME");
fo_size = create_folders(DIRPATH, &folders);
fi_size =  create_files(FILEPATH,&files,folders);

pl_size = create_playlists( PLAYLISTPATH,&playlists);

setlocale(LC_ALL,"C-UTF-8");
setlocale(LC_CTYPE, "");
	setlocale(LC_COLLATE, "");

    initscr();
curs_set(0);

start_color();

 use_default_colors();
assume_default_colors(-1,-1);
// border
init_pair(1, 1, -1);
// play focused
init_pair(2, 0, 3);
//play unfocused
init_pair(3, 3, -1);
//playind inactive cur_sel
init_pair(4, 11, -1);
// focused window focused
init_pair(5, 0, 12);
//unfocused window focused
init_pair(6, 14, -1);
// unfocused window unfocused
init_pair(7,12,-1);
//cmdline error
init_pair(8,1,-1);
//cmdline success
init_pair(9,11,-1);
init_pair(10,5,-1);

init_pair(11,13,-1);
char ** chose = folders;
noecho();
mainwin = newwin(LINES-5,COLS*2/3,1,0);
  info = newwin(4,COLS,LINES-4,0);
  play = newwin(LINES-5,COLS/3,1,COLS*2/3);
input = newwin(1,COLS,0,0);
  create_border(mainwin,'m');
create_border(play,'p');
create_border(info,'i');
int hight = 0;
keypad(mainwin,true);

mvwprintw(mainwin,0,1,  "File");

mvwprintw(play,0,1,  "Playlist");
refresh();
wrefresh(info);


mpv_init();
keypad(stdscr, true);
	while(run){

 if ( move_d ){
move_d = false;

playlist_move( &chosen_playlist, playlists,play,p_hi,pl_size, loaded,playlis == true  );
filnumber = chosen_playlist->filnum;
	if (filnumber <= 0){
		filnumber = fi_size;}
print_files(chosen_playlist,files,fi_size,file_hi,chosen_file, playlis == true  );

}
if ( move_file){
print_files(chosen_playlist,files,fi_size,file_hi,chosen_file,playlis == true  );
move_d = false;
move_file = false;
}
wrefresh(mainwin);
_refresh(play);
int c = getch();
get_input(c);
}

//*/
endwin();
  free(files);
free(playlists);
free(folders);
return 0;
}


