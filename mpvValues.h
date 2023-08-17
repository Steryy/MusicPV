#ifndef MPV_VALUES_H
#define MPV_VALUES_H
#include <mpv/client.h>
extern double player_rate;
extern double player_volume;
extern int player_pitch;
extern char* player_filename;
extern short player_duration;
extern int player_loop;
extern int player_loop_file ;
extern mpv_handle *player_ctx;
#endif
