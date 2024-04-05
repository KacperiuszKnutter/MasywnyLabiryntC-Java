#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "labprocess.h"
#include "BFSqueue.h"

/*Tu bedzie glowny program, puszczamy bfs,
kod dodam pozniej bo jest w miare ready,
tylko trzeba mozliwe zmienic w strukturze kolejki dodanie child i parent bo bedzie potrzebne do odnajdywania sciezki ostatecznej
*/


// char peekPath()
typedef struct MazeSize{
    int columns;
    int rows;
}MazeSize;
MazeSize mazeSize;


int newPositionGetter(int position, char direction)
{
    if(direction == 'G')
    {
        position -= mazeSize.columns;
    }
    else if(direction == 'P')
    {
        position += 1;
    }
    else if(direction == 'D')
    {
        position += mazeSize.columns;
    }
    else
    {
        position -= 1;
    }

    return position;
}

 void addPathsToQueue(queue_t myQueue, char originalchar, int position, char direction)
 {
    //Wszystkie mozliwe opcje skretow
    char* options[] = {
        (char[]){' ', 'X', 'X', 'X', '\0'},
        (char[]){'X', ' ', 'X', 'X', '\0'},
        (char[]){'X', 'X', ' ', 'X', '\0'},
        (char[]){'X', 'X', 'X', ' ', '\0'},
        (char[]){' ', ' ', 'X', 'X', '\0'},
        (char[]){'X', ' ', ' ', 'X', '\0'},
        (char[]){'X', 'X', ' ', ' ', '\0'},
        (char[]){' ', 'X', 'X', ' ', '\0'},
        (char[]){' ', ' ', 'X', ' ', '\0'},
        (char[]){' ', ' ', ' ', 'X', '\0'},
        (char[]){'X', ' ', ' ', ' ', '\0'},
        (char[]){' ', 'X', ' ', ' ', '\0'}
    };
    //Każdemu rodzajowi skrętu bądź rozwidlenia przysługuje literka
    char alphabet[] = "abcdefghijkl";
    int originalPosition = position;
    char Directions[] = "GPDL";

    char skipDirection;
    if (direction == 'P') {
        skipDirection = 'L';
    } else if (direction == 'G') {
        skipDirection = 'D';
    } else if (direction == 'L') {
        skipDirection = 'P';
    } else {
        skipDirection = 'G';
    }

    for(int i = 0; i < 12; i++)
    {
        if(alphabet[i] == originalchar)
        {
            for(int j = 0; j<4; j++)
            {
                if(options[i][j] == ' ' && skipDirection != Directions[j])
                {
                    position = newPositionGetter(originalPosition, Directions[j]);
                    printf("%d %d\n", position, Directions[j]);
                    enqueue(myQueue, position, Directions[j]);
                }
            }
            break;
        }
    }

 }

int bfsSearch(FILE* default_file, queue_t myQueue)
{
    char direction;
    char originalchar;
    int originalPos;
    dequeue(myQueue, &originalPos, &direction);

    fseek(default_file, originalPos, SEEK_SET);
    fread(&originalchar, sizeof(char), 1, default_file);
    fseek(default_file, originalPos, SEEK_SET);


    printf("%d\n", originalPos);
    printf("%c\n", originalchar);
    if(originalchar == 'K')
    {
        printf("Koniec");
        return 0;
    }
    if(originalchar == ' ')
    {
        printf("Ide prosto\n");
        int position = newPositionGetter(originalPos, direction);
        enqueue(myQueue, position, direction);
        fwrite("X", sizeof(char), 1, default_file);
    }
    else if(originalchar != 'X')
    {
        printf("Skrecam\n");
        addPathsToQueue(myQueue, originalchar, originalPos, direction);

    }


    return 1;    
}



int main() {

    FILE* default_file;
    char fileName[] = "test.txt";
    default_file = fopen(fileName, "r+");
    if (!default_file) {
        perror("Error opening file eeeeeeee");
        return 1;
    }

    int rows = 0, columns = 0, maxColumns = 0, maxRows = 0, iter = 0, posP = 0, posK = 0;
    char ch;
    //o ile bajtow trzeba sie przesunac zeby znalezc P i K 

    //petla while odpowiedzialna za czytanie liczby kolumn i wierszy oraz za wczytanie pozycji start oraz końcowej
    while ((ch = fgetc(default_file)) != EOF) {
        columns++;
        iter++;
        if(ch == 'K')
            posK = iter;
        if(ch == 'P')
            posP = iter;

        if (ch == '\n') {
            maxRows++;
            if (columns > maxColumns) {
                maxColumns = columns;
            }
            columns = 0;
        }
    }
    columns = maxColumns;
    rows = maxRows;

    rewind(default_file);
    //Tu plik z labiryntem ulega modyfikacji
    mark_Branching_Points(default_file, rows, columns);

    printf("Columns: %d\n", columns);
    mazeSize.columns = columns;
    printf("Rows: %d\n", rows);
    mazeSize.rows = rows;
    printf("Pozycja Konc: %d\n", posK);
    printf("Pozycja Pocz: %d\n", posP);

    //Na początku dodajemy polozenie startowe jako pierwszego node'a do kolejki
    struct Queue myQueueStruct;
    queue_t myQueue = &myQueueStruct;
    initialize_queue(myQueue);
    enqueue(myQueue, posP, 'P');

    int licznik = 0;
    int result = 1;
    while(result != 0 && licznik <3)
    {
        licznik++;
        result = bfsSearch(default_file, myQueue);
    }

    free_queue(myQueue);





    fclose(default_file);
    return 0;
}