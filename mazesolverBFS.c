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


int bfsSearch(FILE* default_file, queue_t myQueue)
{
    char direction;
    char originalchar;
    int originalPos;
    int wykonanoenqueue = 0;
    dequeue(myQueue, &originalPos, &direction);

    fseek(default_file, originalPos, SEEK_SET);
    fread(&originalchar, sizeof(char), 1, default_file);
    fseek(default_file, originalPos, SEEK_SET);

    printf("[%c]\n", originalchar);
    printf("[%d]\n", myQueue->ile_node);
    if(originalchar == 'K')
    {
        printf("Koniec");
        return 0;
    }
    
    if(originalchar == ' ')
    {
        int position = newPositionGetter(originalPos, direction);
        enqueue(myQueue, position, direction);
        wykonanoenqueue = 1;
        fwrite("O", sizeof(char), 1, default_file);
    }
    else if(originalchar != 'X' && originalchar != 'O')
    {
        char currentCharacter;
        char Directions[] = "GPDL";

        for(int i = 0; i < 4; i++)
        {
            int position = newPositionGetter(originalPos, Directions[i]);
            fseek(default_file, position, SEEK_SET);
            fread(&currentCharacter, sizeof(char), 1, default_file);
            fseek(default_file, originalPos, SEEK_SET);
            if(currentCharacter != 'X' && currentCharacter != 'O' && currentCharacter != '\n')
            {
                enqueue(myQueue, position, Directions[i]);
                wykonanoenqueue = 1;
            }
        }
        fwrite("O", sizeof(char), 1, default_file);
    }
    else if(originalchar == 'O')
    {
        char currentCharacter;
        char Directions[] = "GPDL";
        char reversedDirections[] = "DLGP";


        for(int i = 0; i < 4; i++)
        {
            int position = newPositionGetter(originalPos, Directions[i]);
            fseek(default_file, position, SEEK_SET);
            fread(&currentCharacter, sizeof(char), 1, default_file);
            fseek(default_file, originalPos, SEEK_SET);
            if(currentCharacter != 'X' && currentCharacter != 'O' && currentCharacter != '\n')
            {
                enqueue(myQueue, position, Directions[i]);
                wykonanoenqueue = 1;
            }
        }
        
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
    while(result != 0)
    {
        licznik++;
        result = bfsSearch(default_file, myQueue);
    }

    free_queue(myQueue);


    fclose(default_file);
    return 0;
}