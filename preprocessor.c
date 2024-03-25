#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SIZE 1024

void markBranchingPoints(FILE* in, int rows, int cols, int number, char* characters) {
    int dx[] = {0, 1, 0, -1};
    int dy[] = {-1, 0, 1, 0};
    int row = (int) number / cols;
    int col = (int) number % cols;
    long offset = 0;

    for (int i = 0; i < 4; i++) {
        int newRow = row + dy[i];
        int newCol = col + dx[i];

        if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
            offset = newRow * cols + newCol;
            fseek(in, offset, SEEK_SET);
            characters[i] = fgetc(in);
            if(characters[i] == 'O')
                characters[i] = ' ';
        } else {
            characters[i] = 'X';
        }
    }
    characters[4] = '\0';
}

int main() {
    FILE *file;
    char fileName[] = "innytest2.txt";

    file = fopen(fileName, "r+");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    int rows = 0, columns = 0, maxColumns = 0;
    char ch;

    while ((ch = fgetc(file)) != EOF) {
        columns++;
        if (ch == '\n') {
            rows++;
            if (columns > maxColumns) {
                maxColumns = columns;
            }
            columns = 0;
        }
    }
    columns = maxColumns;
    printf("%d/%d\n", columns, rows);

    int number = 0;
    char characters[5];
    char x;

    rewind(file);
    while (fscanf(file, "%c", &x) == 1) {
        int counter = 0;
        if (x == '\n') {
            continue;
        }
        if (x != 'X') {
            markBranchingPoints(file, rows, columns, number, characters);

            for (int i = 0; i < 4; i++) {
                if (characters[i] == ' ') {
                    counter++;
                }
            }
            if((characters[0] == ' ' && characters[2] == ' ' && characters[1] != ' ' && characters[3] != ' ') ){
                    number++;
                    continue;
            }
            else if((characters[1] == ' ' && characters[3] == ' ' && characters[0] != ' ' && characters[2] != ' ')){
                    number++;
                    continue;
            }
            else{
                if ((x == ' ' && counter >= 2) || x == 'P' || x == 'K') {
                    fseek(file, number, SEEK_SET);
                    fputc('O', file);
                    fseek(file, -1, SEEK_CUR);
                    }
                }
        }
        number++;
    }
    printf("%d",number);
    fclose(file);
    return 0;
}