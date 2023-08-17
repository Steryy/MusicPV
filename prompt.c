
#include <stdio.h>

#include <stdlib.h>

#include <string.h>
#include "prompt.h"

void prompt_init(struct  prompt *pro){
pro->size = 30;
pro->text = (char *)malloc(sizeof(char) * 30);
pro->text[0] = '\0';
pro->len = 0 ;
pro->mouse_pos = 0 ;
}

void prompt_cursor_left(struct prompt * pro){
pro->mouse_pos -=1;
if (pro->mouse_pos <0)
	pro->mouse_pos = 0;


}

void prompt_cursor_right(struct prompt * pro){
pro->mouse_pos ++;
if (pro->mouse_pos >pro->len )
	pro->mouse_pos = pro->len ;



}

int prompt_remove_char(struct prompt * pro){


if (pro->mouse_pos >0 && pro->len >0 ){
memmove(pro->text + pro->mouse_pos-1, pro->text + pro->mouse_pos,pro->len - 1);
pro->text[pro->len-1] = '\0';
pro->len --;
pro->mouse_pos--;
return 1;
}

return -1;
}
void prompt_add_char(struct prompt * pro , char c){
if (pro->len +10 > pro->size){
pro->size *=2;
pro->text = (char*)realloc(pro->text,sizeof(char)* pro->size );
}
if (pro->mouse_pos < pro->len){
memmove(pro->text + pro->mouse_pos, pro->text + pro->mouse_pos-1,pro->len );
}
pro->text[pro->mouse_pos] = c;
pro->len ++;

	pro->mouse_pos++;
if (pro->len == pro->mouse_pos)
	pro->text[pro->len] = '\0';
}
