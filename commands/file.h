#ifndef COMMANDFILE_H
#define COMMANDFILE_H
#include "../dstruct.h"

int get_filename_ext(char filename[]);
char* concat(const char* s1, const char* s2);

void get_files(char** folders, int fol, struct file** files, int* fi_size,
	       int b);

int get_folders(char* folder, int l, char*** folders, int* fol_size);

int arry_fy(char* line, char*** po, int num);

int readfile(char file[], char*** po);

void load_file(struct playlist p , struct file * files, int fi_size);
// adding playlist from file
int create_playlists(char* file, struct playlist** point);

// adding files from file
int create_files(char* file, struct file** point, char ** folders);

int create_folders(char* file, char*** point);
#endif
