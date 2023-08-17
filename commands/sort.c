
#include <stdlib.h>
#include <string.h>

#include "../input.h"
struct sort {
	struct file f;
	int index;
};
char *ret(const char *s, const char *d) {
	char *re = (char *)malloc((strlen(s) + strlen(d)) + 2);
	strcpy(re, s);
	strcat(re, d);
	return re;
}
int filcom(const void *s1, const void *s2) {
	struct sort *e1 = (struct sort *)s1;
	struct sort *e2 = (struct sort *)s2;
	return strcmp(ret(e1->f.fol, e1->f.name), ret(e2->f.fol, e2->f.name));
}
int get_eeee(struct sort *fi, int e) {
	for (int i = 0; i < fi_size; i++)
		if (fi[i].index == e) return i;
	return -1;
}
int intcomp(const void *a, const void *b) {
	int int_a = *((int *)a);
	int int_b = *((int *)b);
	if (int_a == int_b)
		return 0;
	else if (int_a < int_b)
		return 1;
	else
		return -1;
}
int m_sort(char *arg) {
	struct sort *sor = malloc(sizeof(struct sort) * fi_size);
	for (int i = 0; i < fi_size; i++) {
		sor[i].f = files[i];
		sor[i].index = i;
	}

	qsort(sor, fi_size, sizeof(struct sort), filcom);
	for (int i = 1; i < pl_size; i++) {
		for (int e = 0; e < playlists[i].filnum; e++) {
			playlists[i].files[e] =
			    get_eeee(sor, playlists[i].files[e]);
		}
		qsort(playlists[i].files, playlists[i].filnum, sizeof(int),
		      intcomp);
	}
	for (int g = 0; g < fi_size; g++) {
		files[g] = sor[g].f;
	}
	free(sor);
	move_file = true;
	return 1;
}
