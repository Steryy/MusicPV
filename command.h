
#include "dstruct.h"
struct command {
	char *name;
	int (*func)(char *arg);
};
// concentrates chars

int check_command(char *cha);

// get first index of \0 from char *
int get_last_delim(char *s);

void run_command(char *cha);
// auto complete command
void tabe(char *cha, int *i);

void open_command_mode();

