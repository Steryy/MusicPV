
#include "command.h"
#include<unistd.h>

#include <curses.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "commands/file.h"
#include "commands/sort.c"
#include "input.h"
#include "prompt.h"
#include "dstruct.h"
#include "helpers.h"
int command_quit(char *args) {
	run = false;
	return 1;
}

int command_create_playlist(char *args){
if (args ==NULL)
	return false;
struct  playlist * ply =(struct playlist* )realloc(playlists,sizeof(struct playlist)*(pl_size +1) );
ply[pl_size].name = args;
ply[pl_size].filnum = 0;
playlists = ply;
pl_size++;
move_d = true;
return true;
}
int command_remove(char * args){
mvprintw( 0, 0,"remove all files (y/n)");
curs_set(0);
refresh();
int c = getch();
if (c == 'y'){
printf ("no files");
}

return true;
}
int command_prefered_rate(char *args){

if (args ==NULL)
	return false;
if (!chosen_playlist)
return false;

int h = file_hi;
if (chosen_playlist->filnum > 0)
	h = chosen_playlist->files[file_hi];
int e =  atoi(args) -100;
if (h > fi_size || h < 0)
	return false;

if (e > -51 && e < 51 ){


	files[h].pref_rate = (char)e;

	return true;
}

return false;
}

int write_to_files(char* arg) {

	 char*HOME = getenv("HOME");
	chdir(HOME);
	FILE* f = fopen(FILEPATH, "w");
	if (f) {
		for (int i = 0; i < fi_size; i++) {
			int g = files[i].dur;
			if (!g)
				g = 0;
			int h = 0;
if (files[i].pref_rate > -51 && files[i].pref_rate < 51)
	h = (int )files[i].pref_rate;
else if ( files[i].pref_rate =='\0')
	h = (int )files[i].pref_rate;


			fprintf(f, "%s\t%d\t%d\t%d\n", files[i].name,
				files[i].folderind,g,h);
		}
	} else
		return 0;
	fclose(f);
	FILE* d = fopen(DIRPATH, "w");
	if (d) {
		for (int i = 0; i < fo_size; i++) {
			fprintf(d, "%s\n", folders[i]);
		}
	} else
		return 0;

	fclose(d);

	FILE* p = fopen(PLAYLISTPATH, "w");
	if (p) {
		for (int i = 1; i < pl_size; i++) {
			fprintf(p, "%s\t", playlists[i].name);
			for (int e = 0; e < playlists[i].filnum; e++) {
				fprintf(f, "%d", playlists[i].files[e]);
				if (e < playlists[i].filnum - 1)

					fprintf(f, ",");
			}

			fprintf(p, "\n");
		}
	} else
		return 0;
	fclose(p);

	return 1;
}

int add_files_from(char* arg) {
	if (arg == NULL) return 0;
	int max = get_folders(arg, 2, &folders, &fo_size);
	if (max == 1) {
		get_files(folders, fo_size, &files, &fi_size, fi_size);
		filnumber = chosen_playlist->filnum;
		if (filnumber <= 0) filnumber = fi_size;
		move_file = true;
		return 1;
	}

	return 0;
}



struct command commands[] = {{"write", write_to_files},
			     {"add", add_files_from},
			     {"sort", m_sort},
			     {"quit", command_quit},
			     {"pl-create",command_create_playlist},
			     {"remove_all",command_remove},
			     {"pref_rate",command_prefered_rate}


};








int check_command(char *cha) {
	if (strlen(cha) <2) return -1;
	for (int i = 0; i < 7; i++)
		if (strstr(commands[i].name, cha) != NULL) return i;
	return -1;
}
int get_last_delim(char *s) {
	for (int i = 0; i < 30; i++) {
		if (s[i] == ' ') {
			s[i] = '\0';
			return i;
		}
	}
	return -1;
}
void run_command(char *cha) {
	int g = get_last_delim(cha);
	int e = check_command(cha);
	int s = 0;
	if (e != -1) {
		s = commands[e].func((cha) + g + 1);
	}

	werase(input);
	if (s == 1) {
		wattron(input, COLOR_PAIR(9));
		mvwprintw(input, 0, 0, "%s succeses", commands[e].name);

		wattroff(input, COLOR_PAIR(9));
	} else {
		wattron(input, COLOR_PAIR(8));
		char *te = NULL;
		if (e > -1)
			te = commands[e].name;
		else
			te = "Command dont exist";
		mvwprintw(input, 0, 0, "%s error", te);
		wattroff(input, COLOR_PAIR(8));
	}
}

void tabe(char *cha, int *i) {
	int e = check_command(cha);
	if (e != -1) {
		int st = strlen(commands[e].name);
		for (int i = 0; i < st; i++) cha[i] = commands[e].name[i];

		cha[st] = ' ';
		cha[st + 1] = '\0';
		*i = st + 1;
	}
}

void open_command_mode() {
	curs_set(1);
	mvprintw( 0, 0, ":");
	struct prompt cmdline;
	prompt_init(&cmdline);
	bool re = true;
	mouse_position = 1;
	refresh();
	while (re) {
		int i = getch();
		switch (i) {
			case 10:

				run_command(cmdline.text);
				re = false;
				break;
			case 27:
				re = false;

				break;
			case KEY_LEFT:
				prompt_cursor_left(&cmdline);
				break;
			case KEY_RIGHT:
				prompt_cursor_right(&cmdline);
				break;
			case '\t':
				tabe(cmdline.text, &cmdline.len);
				cmdline.mouse_pos = cmdline.len;
				break;
			case KEY_BACKSPACE:
				if (prompt_remove_char(&cmdline) == -1)
					re = false;
				break;
			default:
				if (i < 127 && i > 31)
					prompt_add_char(&cmdline, (char)i);
				break;
		}
		if (re) {

	move(0,0);
	clrtoeol();
			mvprintw( 0, 0, ":%s", cmdline.text);
			mouse_position = cmdline.mouse_pos + 1;
			refresh();
		}
	}

	move(0,0);
	clrtoeol();
	curs_set(0);
	refresh();
}
