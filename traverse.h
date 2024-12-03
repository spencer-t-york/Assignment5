#ifndef TRAVERSE_H
#define TRAVERSE_H

struct files { // linked list to point to dirent struct for sorting
    struct dirent *direntp;
    struct files *next;
};

struct files *sort(struct files *);
void mode_to_str(mode_t, char *)
const char* numberToMonth(int);
void printFiles(struct files *);
void printFilesL(struct files *, const char *);
void ls(char dirname[], int, int);
int traverse(int, char *argv[]);

#endif
