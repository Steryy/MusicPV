#include <ncurses.h>

#include "dstruct.h"
// TODO change name to print_playlist
// prints names from struct playlist * playlists to WINDOW play
void playlist_move(struct playlist ** chosen , struct playlist * plays, WINDOW * play, int p_hi,int pl_size,struct playlist * loaded , int show  );

// changes second to human readable format
char* to_human_redable(int ien);

void create_border(WINDOW* win,char type );

// prints fol and name from struct file * files to WINDOW mainwin
void print_files( struct playlist * chosen_playlist, struct file * files,int fi_size,int file_hi, struct file * chosen_file,int show );

extern char* DIRPATH;
extern char* FILEPATH;
extern char* PLAYLISTPATH;
extern char* CONFIGDIR;

