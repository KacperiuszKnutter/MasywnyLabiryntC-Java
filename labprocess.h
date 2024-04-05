#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//deklaracje funkcji z labprocess.c
#ifndef LABPROCESS_H
#define LABPROCESS_H


bool is_Valid(char* characters, char orgch);

void mark_Branching_Points(FILE* default_file, int rows, int cols);

#endif //LABPROCESS_H