#include "file.h"
#include <sys/types.h>
#include <sys/stat.h>
#include<unistd.h>
#include <errno.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../dstruct.h"
#include "../helpers.h"
const char exts[3][4] = {"mp3", "wav","mp4"};
int get_filename_ext(char filename[]) {
	char* dot = strrchr(filename, '.');
	int t = 0;
	if (!dot) return 0;
	dot = dot + 1;
	for (int i = 0; i < 3; i++) {
		if (strncmp(exts[i], dot, 3) == 0) t = 1;
	}
	return t;
}

char* concat(const char* s1, const char* s2) {
	char* result = (char*)malloc(strlen(s1) + strlen(s2) + 2);
	strcpy(result, s1);
	strcat(result, "/");
	strcat(result, s2);
	return result;
}

void get_files(char** folders, int fol, struct file** files, int* fi_size,
							 int b) {
	for (int i = 0; i < fol; i++) {
		DIR* d;
		struct dirent* dir;
		d = opendir(folders[i]);
		while ((dir = readdir(d)) != NULL) {
			if (get_filename_ext(dir->d_name)) {
				int show = 1;
				for (int j = 0; j < b; j++)
					if (strcmp(files[0][j].name,
								dir->d_name) == 0 &&
						files[0][j].fol == folders[i])
						show = 0;
				if (show == 1) {
					struct file* fils =
						(struct file*)realloc(
							*files,
							(*fi_size + 1) *
							sizeof(struct file));
					fils[*fi_size].name = dir->d_name;
					fils[*fi_size].dur = 0;

					fils[*fi_size].pref_rate = 0;
					fils[*fi_size].folderind = i;
					fils[*fi_size].fol = folders[i];

					*files = fils;
					*fi_size += 1;
				}
			}
		}
	}
}

int get_folders(char* folder, int l, char*** folders, int* fol_size) {
	if (l <= 0) {
		return -2;
	}
	DIR* d;
	struct dirent* dir;
	d = opendir(folder);
	if (d) {
		while ((dir = readdir(d)) != NULL)

		{
			if (strcmp(dir->d_name, "..") == 0 ||
				strcmp(dir->d_name, ".") == 0)
				continue;
			if (dir->d_type == DT_DIR) {
				char* a = concat(folder, dir->d_name);
				get_folders(a, l - 1, folders, fol_size);
			} else {
				if (get_filename_ext(dir->d_name)) {
					char* fol = concat(folder, "");
					int b = -1;
					for (int i = 0; i < *fol_size; i++) {
						if (strcmp(folders[0][i],
							fol) == 0)
						b = i;
					}
					if (b == -1) {
						char** nex = (char**)realloc(
							*folders,
							(*fol_size + 1) *
							sizeof(char*));
						(nex)[*fol_size] = fol;
						b = *fol_size;
						*folders = nex;
						*fol_size += 1;
					}
				}
			}
		}

		closedir(d);
		return 1;
	}
	return -1;
}
void create_if_dont_exist(char* file){
	 char*HOME = getenv("HOME");
	chdir(HOME);
	chdir(CONFIGDIR);


	if (access(file, F_OK) != 0){
		FILE *fptr;

		// Create a file
		fptr = fopen(file, "w");

		// Close the file
		fclose(fptr);
	}
}
int arry_fy(char* line, char*** po, int num) {
	char* nuu[num];
	int b = 0;

	char* token = strtok(line, "\t");

	*po = (char**)malloc(num * sizeof(char*));
	while (token) {
		(*po)[b] = strdup(token);
		b++;
		token = strtok(0, "\t");
	}
	free(line);
	return b - 1;
}


int readfile(char file[], char*** po) {
	FILE* fp = fopen(file, "r");
	if (fp == NULL) {
		fclose(fp);
		FILE* f = fopen(file, "w");
		fclose(f);

		return 0;
	}
	int count = 0;
	char ch = getc(fp);
	while ((ch = fgetc(fp)) != EOF) {
		if (ch == '\n') count++;
	}
	fclose(fp);
	FILE* fi = fopen(file, "r");
	char* line = NULL;
	size_t len = 0;
	int linenum = 0;
	char* b[count];

	while (getline(&line, &len, fi) != -1) {
		int num = 0;

		b[linenum] = strdup(line);

		linenum++;
	}

	fclose(fi);
	*po = (char**)malloc(count * sizeof(char*));
	for (int i = 0; i < count; i++) {
		(*po)[i] = b[i];
	}

	return count;
}
int create_folders(char* file, char*** point) {
	create_if_dont_exist(file);
	char** b;
	int size = 0;
	size = readfile(file, &b);

	*point = (char**)malloc(size * sizeof(char*));
	for (int i = 0; i < size; i++) {
		char** temp;
		arry_fy(b[i], &temp, 1);
		int le = strlen(strdup(temp[0]));
		temp[0][le - 1] = '\0';
		(*point)[i] = temp[0];
	}

	free(b);
	return size;
}

void load_file(struct playlist p , struct file * files, int fi_size) {
	FILE* n = fopen("/tmp/musicpv.play.tmp", "w");
	if (!n) {
		fclose(n);
		return;
	}
	int len = p.filnum;
	if (p.filnum <= 0) len = fi_size;
	char be[300];
	for (int i = 0; i < len; i++) {
		struct file tmp;
		if (p.filnum > 0)
			tmp = files[p.files[i]];

		else
			tmp = files[i];
		strcpy(be, tmp.fol);
		strcat(be, tmp.name);
		fprintf(n, "%s\n", be);
	}

	fclose(n);
}

int create_playlists(char* file, struct playlist** point) {
	char** b;
	int size = 0;
create_if_dont_exist(file);
	size = readfile(file, &b) + 1;

	*point = (struct playlist*)malloc((size) * sizeof(struct playlist));
	(*point)[0].name = (char*)malloc(sizeof(char) * 7);
	(*point)[0].name = "default";
	(*point)[0].filnum = -1;
	for (int i = 1; i < size; i++) {
		char** temp;
		arry_fy(b[i - 1], &temp, 2);
		(*point)[i].name = temp[0];
		int ee = 0;
		(*point)[i].files[ee] = -1;
		char* token = strtok(temp[1], ",");

		while (token) {
			(*point)[i].files[ee] = atoi(token);

			token = strtok(NULL, ",");
			ee++;
		}
		(*point)[i].filnum = ee;
	}

	free(b);
	return size;
}
void create_config_dir(char* home){
	DIR* dir = opendir(CONFIGDIR);
if (dir) {
    closedir(dir);
} else if (ENOENT == errno) {
		mkdir(CONFIGDIR,0700);
    /* Directory does not exist. */
} else {
    /* opendir() failed for some other reason. */
}

chdir(home);
}
int create_files(char* file, struct file** point, char **folders) {
	char** b;
	int size = 0;
create_if_dont_exist(file);
	size = readfile(file, &b);

	*point = (struct file*)malloc(size * sizeof(struct file));
	for (int i = 0; i < size; i++) {
		char** temp;
		int e =  arry_fy(b[i], &temp, 4);
		//	 strncpy((*point)[i].name,temp[1],300);
		(*point)[i].name = temp[0];
		(*point)[i].folderind = atoi(temp[1]);
		(*point)[i].fol = folders[(*point)[i].folderind];
		(*point)[i].dur = 0;

		(*point)[i].pref_rate = ((char)0);
		if (e >1)
			(*point)[i].dur = atoi(temp[2]);
		if (e > 2){
			(*point)[i].pref_rate = ((char)atoi(temp[3]));
		}

	}

	free(b);
	return size;
}
