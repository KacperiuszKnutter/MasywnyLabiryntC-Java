#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//deklaracje funkcji z labprocess.c
#ifndef BFSQUEUE_H
#define BFSQUEUE_H


bool is_Valid(char* characters, char orgch);

void mark_Branching_Points(FILE* default_file, int rows, int cols);

#endif //BSFQUEUE_H