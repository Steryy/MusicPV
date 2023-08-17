#ifndef DEFST_H
#define DEFST_H
struct file;
 struct playlist;

struct file {

 char* name;
 int folderind;
 char * fol;
short dur;
char pref_rate;
};
//TODO change playlist files to pointer
struct playlist{
 char *name;
int files[100];
int filnum;
struct file **fil;
};
#endif
