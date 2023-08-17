
#include <curses.h>
#include <mpv/client.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include "command.h"
#include "commands/file.h"
#include "search.c"
#include "input.h"
#include <pthread.h>
#include <threads.h>
#include <unistd.h>
#include "init.h"
#include "mpvValues.h"
#include "mpris.h"
#include "dstruct.h"

int to_sec = 1000000;
static const char vol_type[] = "volume";

void set_file(int i ){
	if (loaded) {
		if (loaded->filnum > 0)
			chosen_file = &files[loaded->files[player]];
		else
			chosen_file = &files[player];

if (chosen_file->dur <1){
double e = 0;
mpv_get_property(player_ctx, "duration", MPV_FORMAT_DOUBLE, &e);
chosen_file->dur = (short)e;
	}

		player_duration = chosen_file->dur;

		player_filename = chosen_file->name;

		mpris_player_property_change("mpris:length");
		if (i){
		print_files(chosen_playlist, files, fi_size, file_hi,
			    chosen_file, playlis == true);
		wrefresh(mainwin);
		}
			if (chosen_file->pref_rate > -51 && chosen_file->pref_rate < 51 ){
player_rate = (1 + (double)((int)chosen_file->pref_rate)/100);
	mpv_set_property(player_ctx, "speed", MPV_FORMAT_DOUBLE, &player_rate);


			}

	}
}
double MAX_RATE = 2;
double MIN_RATE= 0.1;
int MAX_VOLUME=150;
int MIN_VOLUME=0;

int col = 0;
int lines = 0;
void update_mpv() {
	if (!run) return;
	werase(info);
	create_border(info,'i');
	double vol = 0;
	double speed = 0;
	double time = 0;
	int pause = -1;
	int play;

	char pas = '/';

	short dure = 0;
	mpv_get_property(player_ctx, "pause", MPV_FORMAT_FLAG, &pause);
	mpv_get_property(player_ctx, vol_type, MPV_FORMAT_DOUBLE, &vol);

	mpv_get_property(player_ctx, "playlist-pos", MPV_FORMAT_INT64, &play);
	if (chosen_file){

	if (chosen_file->dur)
		dure = chosen_file->dur;
		if (chosen_file->name) {

	if (pause == 0 ) pas = '>';
		if (play != player) {
			player = play;
		set_file(1);
		}


	if (vol !=  player_volume ) {
		if (vol > 110)
			vol = 110;
		else if (vol < 0)
			vol = 0;
		 player_volume = vol;

		mpv_set_property(player_ctx, vol_type, MPV_FORMAT_DOUBLE, &vol);

mpris_player_property_change("Volume");
	}

		wattron(info, COLOR_PAIR(10));
		mvwaddnstr(info, 1, 1, chosen_file->name,COLS-23);
		wattroff(info, COLOR_PAIR(10));
	}
	}
	mpv_get_property(player_ctx, "time-pos", MPV_FORMAT_DOUBLE, &time);
	mpv_get_property(player_ctx, "speed", MPV_FORMAT_DOUBLE, &speed);
	wattron(info, COLOR_PAIR(11));
	mvwprintw(info, 2, 1, "%c %s/%s", pas, to_human_redable((int)time),
		  to_human_redable((int)dure));
	mvwprintw(info, 1, COLS - 23, " vol %03d%%  speed %03d%%", (int)vol,
		  (int)(speed * 100));

		if (player_loop == 1 && player_loop_file == 0 )
			mvwprintw(info, 2, COLS - 3, "R");
		else if (player_loop == 1)
			mvwprintw(info, 2, COLS - 3, "r");
		else
			mvwprintw(info, 2, COLS - 3, " ");
	if (player_pitch == 1)

		mvwprintw(info, 2, COLS - 2, "P");

	else
		mvwprintw(info, 2, COLS - 2, " ");

	wattroff(info, COLOR_PAIR(11));
	_refresh(info);

}

void chack_change(){


if (col != COLS || lines != LINES){
col = COLS;
lines = LINES;

resizeterm(LINES, COLS);
wresize(mainwin, LINES-5, COLS*2/3);
wresize(play, LINES-5, COLS/3);
mvwin(play,1,COLS*2/3);
wresize(info, 4, COLS);
mvwin(info,LINES-4,0);

wresize(input, 1, COLS);
move(0,mouse_position);


refresh();

playlist_move( &chosen_playlist, playlists,play,p_hi,pl_size, loaded,playlis == true  );

print_files(chosen_playlist,files,fi_size,file_hi,chosen_file, playlis == true  );
wrefresh(mainwin);
wrefresh(play);
update_mpv();
}



}
void load_playlist() {

const char *cmd_load[] = {"loadlist", "/tmp/musicpv.play.tmp", NULL};
	load_file(*chosen_playlist,files,fi_size);
	mpv_command(player_ctx, cmd_load);
	loaded = chosen_playlist;
	player = file_hi;
remove("/tmp/musicpv.play.tmp");
	move_d = true;

int pau = 0;
			mpv_set_property(player_ctx, "pause", MPV_FORMAT_FLAG, &pau);
	mpv_set_property(player_ctx, "playlist-pos", MPV_FORMAT_INT64, &player);
	set_file( 0);

	update_mpv();
}

void *mpv_observe_changes(void *args) {
	int i = 0;
	while (run) {

chack_change();
		if (!run) break;
if ( i > 5){
		update_mpv();
i = 0;
}
		if (!run) break;
		usleep((int)(0.1 * to_sec));
		if (!run) break;
		i++;
	}
	mpv_terminate_destroy(player_ctx);

	return 0;
}
void set_pitch_correct() {
	if (player_pitch == 1)

		mpv_set_property_string(player_ctx, "audio-pitch-correction", "yes");
	else

		mpv_set_property_string(player_ctx, "audio-pitch-correction", "no");


}

void mpv_init() {
	col  = COLS;
	lines = LINES;
	player_ctx = mpv_create();
	mpv_set_property_string(player_ctx, "really-quiet", "yes");
	set_pitch_correct();
if (player_loop == 1)
	mpv_set_property_string(player_ctx, "loop-playlist", "inf");
else

	mpv_set_property_string(player_ctx, "loop-playlist", "0");
	mpv_set_property_string(player_ctx, "terminal", "no");
	mpv_set_property_string(player_ctx, "vo", "null");

player_volume = 20;
	mpv_set_property(player_ctx, vol_type, MPV_FORMAT_DOUBLE, &player_volume);
	mpv_initialize(player_ctx);


	pthread_t hello_world_thread;
	int result = pthread_create(&hello_world_thread, NULL,
				    mpv_observe_changes, NULL);


	pthread_t mpris;
	int resu = pthread_create(&mpris, NULL,
				    mpris_init, NULL);

	player_volume = -1;
}
void change_speed(int i) {
	switch (i) {
		case '[':
			player_rate -= 0.1;
			break;
		case ']':
			player_rate += 0.1;
			break;
		case '{':
			player_rate -= 0.01;
			break;
		case '}':
			player_rate += 0.01;
			break;
		default:
			return;
			break;
	}
	if (player_rate >MAX_RATE)
		player_rate = MAX_RATE;
	else if (player_rate <MIN_RATE)
		player_rate = MIN_RATE;

	mpv_set_property(player_ctx, "speed", MPV_FORMAT_DOUBLE, &player_rate);
	update_mpv();
}
void remove_playlist() {
	if (p_hi == 0) {
		return;
	}
	for (int i = p_hi; i < pl_size - 1; i++) {
		playlists[i] = playlists[i + 1];
	}
	pl_size--;
	p_hi = 0;
	werase(play);
	create_border(play,'p');
	move_d = true;
}
void remove_file() {
	if (p_hi == 0) {
		for (int i = file_hi; i < fi_size - 1; i++) {
			files[i] = files[i + 1];
		}
		fi_size--;
	}
	if (p_hi == 0)
		for (int i = 1; i < pl_size; i++) {
			int inde = -1;
			for (int c = 0; c < playlists[i].filnum; c++) {
				if (file_hi == playlists[i].files[c]) {
					inde = c;
					break;
				}
			}

			if (inde > -1) {
				for (int c = inde; c < playlists[i].filnum - 1;
				     c++) {
					playlists[i].files[c] =
					    playlists[i].files[c + 1] - 1;
				}

				playlists[i].filnum--;
			} else {
				for (int c = 0; c < playlists[i].filnum; c++) {
					if (playlists[i].files[c] > file_hi)
						playlists[i].files[c]--;
				}
			}
		}
	else {
		for (int i = file_hi; i < playlists[p_hi].filnum - 1; i++) {
			playlists[p_hi].files[i] = playlists[p_hi].files[i + 1];
		}
		playlists[p_hi].filnum--;
		chosen_playlist = &playlists[p_hi];
	}
	if (file_hi > fi_size-1)
		file_hi = fi_size-1;
	move_file = true;
}
void add_file_to_playlist() {
	bool t = true;
	if (p_hi != 0) return;
	if (play_sel < 1) return;
	for (int i = 0; i < playlists[play_sel].filnum; i++) {
		if (file_hi == playlists[play_sel].files[i]) t = false;
	}
	if (t) {
		playlists[play_sel].files[playlists[play_sel].filnum] = file_hi;
		playlists[play_sel].filnum++;
	}
	file_hi++;
	if (file_hi > fi_size - 1)
		file_hi = 0 ;
move_file = true;
}
void loop_change() {
	if (player_loop_file ==0 ) {
		if (player_loop ==1) {
			mpv_set_property_string(player_ctx, "loop-playlist", "0");
			player_loop = 0;
		} else {
			mpv_set_property_string(player_ctx, "loop-playlist", "inf");
			player_loop =1;
		}

	} else {
		if (player_loop == 1) {
			mpv_set_property_string(player_ctx, "loop-file", "0");
			player_loop = 0;
		} else {
			mpv_set_property_string(player_ctx, "loop-file", "inf");
			player_loop = 1;
		}
	}
}
void get_input(int c) {
	int pau = -1;
	int filnumbe = fi_size;
	if (loaded)
		if (loaded->filnum > 0) filnumbe = loaded->filnum;
	switch (c) {
		case 'g':
			move_d = true;
			if (playlis)
				p_hi = 0;
			else
				file_hi = 0;

			break;
		case 'G':
			if (playlis)
				p_hi = pl_size - 1;
			else
				file_hi = filnumber - 1;

			move_d = true;
			break;

		case 'j':
			move_d = true;
			if (playlis) {
				p_hi++;
				if (p_hi > pl_size - 1) p_hi = 0;
				file_hi = 0;
			} else {
				file_hi++;
				if (file_hi > filnumber - 1) file_hi = 0;
			}
			break;
		case 'k':
			move_d = true;
			if (playlis) {
				p_hi--;
				if (p_hi < 0) p_hi = pl_size - 1;

				file_hi = 0;
			} else {
				file_hi--;
				if (file_hi < 0) file_hi = filnumber - 1;
			}
			break;
		case '\t':
			move_d = true;
			playlis = !playlis;
			break;
		case 'q':
			run = false;
			break;
		case 10:
			load_playlist();
			break;
		case ':':
			open_command_mode();

			break;
		case '/':
			open_search_mode();
			break;
		case 'R':
			if (player_loop == 1) loop_change();

			if (player_loop_file == 1)
				player_loop_file =0 ;
			else
			player_loop_file =1;
		case 'r':
			loop_change();
			update_mpv();
			break;
		case 'P':
			if (player_pitch == 1)
				player_pitch =0;
			else
				player_pitch = 1;
			set_pitch_correct();
			break;
		case 32:
			if (playlis) {
				move_d = true;
				play_sel = p_hi;
			}
			add_file_to_playlist();
			break;
		case 'c':

			mpv_get_property(player_ctx, "pause", MPV_FORMAT_FLAG, &pau);
			if (pau == 0)
				pau = 1;
			else
				pau = 0;

			mpv_set_property(player_ctx, "pause", MPV_FORMAT_FLAG, &pau);

			update_mpv();

mpris_player_property_change("PlaybackStatus");
			break;
		case 'D':
			if (!playlis)
				remove_file();
			else
				remove_playlist();

			break;
		case 'p':
		case 'n':
			if (!loaded) return;
			if (c == 'p')
				player--;
			else
				player++;
			if (player > filnumbe - 1)
				player = 0;
			else if (player < 0)
				player = filnumbe - 1;
			set_file(1);
			mpv_set_property(player_ctx, "playlist-pos",
					 MPV_FORMAT_INT64, &player);

			update_mpv();
			break;
		case '-':
			player_volume -= 10;
		case '+':
		case '=':
			player_volume+=5;
			if ( player_volume < MIN_VOLUME)
				player_volume = MIN_VOLUME;
			else if (player_volume > MAX_VOLUME)
				player_volume = MAX_VOLUME;
			mpv_set_property(player_ctx, vol_type, MPV_FORMAT_DOUBLE,
					 &player_volume);

mpris_player_property_change("Volume");
			update_mpv();
			break;
		default:
			change_speed(c);
			break;
	}
}

