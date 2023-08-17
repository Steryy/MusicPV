#ifndef INPUT_H
#define INPUT_H

#include <ncurses.h>

#include "dstruct.h"
/*
 * struct file;
 *  struct playlist;
 *
 * struct file {
 *
 *  char* name;
 *  int folderind;
 *  char * fol;
 * short dur;
 * };
 * //TODO change playlist files to pointer
 * struct playlist{
 *  char *name;
 * int files[100];
 * int filnum;
 * struct file **fil;
 * };
 */


//mpv speed of played file
extern double speed ;

//mpv volume of played file
extern double volume ;

// mpv pitch_correction
extern bool pitch ;

// pointer to currently playing file
extern struct file *chosen_file ;

//playlists to read from file
extern struct playlist *playlists;
//files to read from file
extern struct file* files;

//folders/directories to read from file
extern char ** folders;
//playlists size
extern int pl_size;
//files size
extern int fi_size;
//folders size
extern int fo_size;

//ncurses window
extern WINDOW * info; //contain information about playing file speed volume etc.
extern  WINDOW * play;//contain playlists
extern WINDOW * mainwin;//contain files
extern WINDOW * input; //contain command mode and //TODO search mode/

extern struct playlist *chosen_playlist;
extern struct playlist *loaded;
//stops loop in void main  and void *mpv_observe
extern bool run ;

//refresh mainwin
extern bool move_file ;
// refresh mainwin or play
extern bool move_d ;
// if refresh mainwin and play
extern bool playlis ;

// highlighted playlist index
extern int p_hi;
// number of chosen_playlist.files if 0 then use *files
extern int filnumber ;
// highlighted file index
extern int file_hi ;

extern int play_sel;

extern int mouse_position;
//TODO delete index of played file
extern int player ;
void _refresh( );

#endif
