
void set_pitch_correct();
// set mpv properties and initialize
void mpv_init();
// change double speed from input.h
void change_speed(int i);
void *mpv_observe_changes(void *args);
void get_input(int c);
