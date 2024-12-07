#ifndef TRAVERSE_H
#define TRAVERSE_H

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

// traverse
void traverse(int, int);

// convert mode
void mode_to_str(unsigned short, char *);

// convert number number to letters
const char* numberToMonth(int);

#endif
