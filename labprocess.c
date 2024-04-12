#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <labprocess.h>

//Sprawdza czy badane miejsce w labiryncie nadaje sie do wstawienia wierzcholka
bool is_Valid(char* characters, char orgch) {
    int counter = 0;
    //Nie rozwazamy scian ani miejsc w ktorych juz sa wierzcholki
    if (orgch != ' ') {
        return false;
    }

    for (int i = 0; i < 4; i++) {
        if (characters[i] == ' ') {
            counter++;
        }
    }
    //Pomijamy korytarze bo nie chcemy dodatkowych O
    if ((characters[0] == ' ' && characters[1] != ' ' && characters[2] == ' ' && characters[3] != ' ') ||
        (characters[0] != ' ' && characters[1] == ' ' && characters[2] != ' ' && characters[3] == ' ')) {
        return false;
    } else {
        if ((orgch == ' ' && counter >= 2) || orgch == 'P' || orgch == 'K') {
            return true;
        }
    }

    return false;
}

//Znajduje otoczenie dla kazdego miejsca w labiryncie i przechowuje je w wektorze characters
void mark_Branching_Points(FILE* default_file, int rows, int cols) {
    int capacity = rows * cols;
    long position = 0;
    int row_num[] = {-1, 0, 1, 0};
    int col_num[] = {0, 1, 0, -1};
    char ch;
    char originalchar;
    char characters[5];
    int ileO = 0;

    while (position < capacity) {
        //Mozna pominac pierwszy i ostatni wiersz bo to i tak same sciany
        if(position<cols || position >cols*(rows-1)){
            position++;
            continue;
        }
        fseek(default_file, position, SEEK_SET);
        fread(&originalchar, sizeof(char), 1, default_file);

            for (int i = 0; i < 4; i++) {
                int col = (int) (position % cols) + col_num[i];
                int row = (int) (position / cols) + row_num[i];

                if (row >= 0 && row < rows && col >= 0 && col < cols) {
                    fseek(default_file, row * (cols) + col, SEEK_SET);
                    fread(&ch, sizeof(char), 1, default_file);
                    if (ch == 'X') {
                        characters[i] = 'X';
                    }
                    //Wszystko co nie jest X traktujemy jako potencjonalna droge
                    else {
                    characters[i] = ' ';
                    }
                }
                //Jak wyjdzie poza plansze to X
                else{
                    characters[i] = 'X';
                }
            }

            characters[4] = '\0';
            fseek(default_file, position, SEEK_SET);
            if (is_Valid(characters, originalchar)) {
              
                fwrite("B", sizeof(char), 1, default_file);
                ileO++;
                
            }
        position++;
    }
    printf("Tyle jest wierzcholkow --> %d\n", ileO);
}