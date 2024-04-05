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


void bfsSearch(queue_t myQueue)
{
    int pos;
    char dir;
    while (dequeue(myQueue, &pos, &dir)) {
        printf("Dequeued: Position = %d, Direction = %c\n", pos, dir);
    }
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
    printf("Rows: %d\n", rows);
    printf("Pozycja Konc: %d\n", posK);
    printf("Pozycja Pocz: %d\n", posP);

    //Na początku dodajemy polozenie startowe jako pierwszego node'a do kolejki
    struct Queue myQueueStruct;
    queue_t myQueue = &myQueueStruct;
    initialize_queue(myQueue);
    enqueue(myQueue, posP, 'P');
    bfsSearch(myQueue);




    fclose(default_file);
    return 0;
}