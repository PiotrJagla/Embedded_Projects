#ifndef MY_STRINGS_H
#define MY_STRINGS_H

#include <string.h>
#include <stdlib.h>


char* substring(char* text, int start, int end);
char** split(char* text, int len, char delimiter);

#endif
