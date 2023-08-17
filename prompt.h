#ifndef PROMPT_H
#define PROMPT_H
struct prompt{
char * text;
int mouse_pos;
int len;
int size ;

};

//add start values to prompt
void prompt_init(struct  prompt *pro);

//decrease prompt mouse_pos
void prompt_cursor_left(struct prompt * pro);


//increase prompt mouse_pos
void prompt_cursor_right(struct prompt * pro);

int prompt_remove_char(struct prompt * pro);



void prompt_add_char(struct prompt * pro , char c);

#endif








