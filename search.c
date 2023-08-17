
#include <curses.h>
#include <ncurses.h>
#include "prompt.h"
#include "helpers.h"
#include <string.h>
#include "input.h"
char * upper_string(char *g) {
   int c = 0;
char *s = strdup(g);
   while (s[c] != '\0') {
      if (s[c] >= 'a' && s[c] <= 'z') {
         s[c] = s[c] - 32;
      }
      c++;
   }
   return s;
}
struct prompt search;
int   search_select_item(){

	if (files[file_hi].name !=NULL)
if (strstr(files[file_hi].name,search.text) != NULL)
	return file_hi ;
int size = fi_size;
if (chosen_playlist->filnum >0){
size = chosen_playlist->filnum;
}
for (int i = 0 ; i < size ; i++){
struct file f ;
if (chosen_playlist->filnum >0)
f = files[chosen_playlist->files[i]];
else
	f = files[i];
	if (files[i].name !=NULL)
		if (strstr(upper_string(files[i].name),upper_string(search.text)) != NULL)
			return i;
}

return  -1;
}

void open_search_mode(){
curs_set(1);

mvprintw(0, 0,"/");
mouse_position = 1;
refresh();
prompt_init(&search);
bool re = true;
while(re){
int i = getch ();
switch(i){
	case 10:  re = false ; break;
	case 27: re = false ; break;
	case KEY_LEFT:prompt_cursor_left(&search);break;

	case KEY_RIGHT: prompt_cursor_right(&search);break;
	case KEY_BACKSPACE:if  (prompt_remove_char(&search) ==-1)re = false;break;
	default: if (i <127 && i >31) prompt_add_char(&search,(char)i) ;

int  tmp = search_select_item();
		 if (tmp >-1){
			 file_hi = tmp;

print_files(chosen_playlist,files,fi_size,file_hi,chosen_file,playlis == true  );
refresh();
		 }

			 break;
}
if (re){
	mvwprintw(input,0,0,  "/%s",search.text);
mouse_position = search.mouse_pos +1;

move(0,mouse_position);
refresh();
}
}

	move(0,0);
	clrtoeol();
curs_set(0);
refresh();

}
