#include "mpvValues.h"
#include <stddef.h>
#include <mpv/client.h>
double player_volume = 0;
int player_pitch = 0;
double player_rate = 1;
char * player_filename = NULL;
short player_duration = 0;
int player_loop= 1;
int player_loop_file = 0;
mpv_handle * player_ctx;
