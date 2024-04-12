#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <labprocess.h>
#include <BFSqueue.h>
#include <DFSstack.h>

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

//logika do znajdywania nowej pozycji (rozgladanie sie)
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

//algorytm odpowiedzdzialny za usuniecie slepych sciezek
int dfsSolve(FILE* default_file, obecnekroki droga, int backTrack)
{
    char direction;
    char originalchar;
    int originalPos;
    int wykonanoenqueue = 0;
    peekPath(droga, &originalPos, &direction);
    
    fseek(default_file, originalPos, SEEK_SET);
    fread(&originalchar, sizeof(char), 1, default_file);
    fseek(default_file, originalPos, SEEK_SET);

    printf("[%c]\n", originalchar);

    
    if(originalchar == 'P')
    {
        printf("DFS zakonczony sukcesem\n");
        return 0;
    }
    else if(originalchar == 'O')
    {
        if(backTrack == 1)
        {
            int position = newPositionGetter(originalPos, direction);
            fwrite("S", sizeof(char), 1, default_file);
            appendElement(droga, position, direction);
        }
        else{
            popElement(droga, &originalPos, &direction);
            fwrite("X", sizeof(char), 1, default_file);
            return -1;
        }
    }
    else if(originalchar == 'B' || originalchar == 'K')
    {
        char currentCharacter;
        char Directions[] = "GPDL";
        int wykonanoenqueue = 0;
        int tempPos;
        for(int i = 0; i < 4; i++)
        {
            int position = newPositionGetter(originalPos, Directions[i]);
            fseek(default_file, position, SEEK_SET);
            fread(&currentCharacter, sizeof(char), 1, default_file);
            fseek(default_file, originalPos, SEEK_SET);
            tempPos = position;
            if(currentCharacter == 'O'||currentCharacter == 'B'||currentCharacter == 'P')
            {
                peekPath(droga,&position,&direction);
                fseek(default_file, position, SEEK_SET);
                fread(&currentCharacter, sizeof(char), 1, default_file);
                fseek(default_file, originalPos, SEEK_SET);
                //dodawaj na stos tylko rozwidlenia by nie przekroczyc pamieci
                while(currentCharacter != 'B' && currentCharacter != 'K'){
                    popElement(droga,&position,&direction);
                    peekPath(droga,&position,&direction);
                    fseek(default_file, position, SEEK_SET);
                    fread(&currentCharacter, sizeof(char), 1, default_file);
                    fseek(default_file, originalPos, SEEK_SET);
                }
                appendElement(droga, tempPos, Directions[i]);
                wykonanoenqueue = 1;
                break;
            }
            //Przypadek Gdy usuniemy slepy zauek i w danym rozwidleniu nie ma juz O ale sa S
            
        }
        if(wykonanoenqueue == 0)
        {
            //Cofnij sie i wstaw x
            fwrite("X", sizeof(char), 1, default_file);
            popElement(droga, &originalPos, &direction);
            return -1;
        }
    }
    else if(originalchar == 'X' || originalchar == ' ')
    {
        popElement(droga, &originalPos, &direction);
        fwrite("X", sizeof(char), 1, default_file);
        return -1;
    }
    else if(originalchar == 'S')
    {
        popElement(droga, &originalPos, &direction);
        fwrite("X", sizeof(char), 1, default_file);
        return -1;
    }
    else if(originalchar == '\n')
    {
        popElement(droga, &originalPos, &direction);
        fwrite("\n", sizeof(char), 1, default_file);
        return -1;
    }
    return 1;

}



//Algorytm odpowiedzialny za znalezienie sciezek w labiryncie (najszybszej! bo szukamy szerokoscia)
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
    else if(originalchar != 'O' && originalchar != 'X')
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
    }
    else if(originalchar == 'O')
    {
        char currentCharacter;
        char Directions[] = "GPDL";

        char reverseDir;
        if(direction == 'G')
        {
            reverseDir = 'D';
        }
        else if(direction == 'P')
        {
            reverseDir = 'L';
        }
        else if(direction == 'D')
        {
            reverseDir = 'G';
        }
        else
        {
            reverseDir = 'P';
        }

        int position = newPositionGetter(originalPos, reverseDir);
        fseek(default_file, position, SEEK_SET);
        fwrite("X", sizeof(char), 1, default_file);
        fseek(default_file, originalPos, SEEK_SET);

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
//funkcja ktora odzyskuje sciezke po zmianach w domyslnym pliku i wypisuje ja w nowym
void transformAndWrite(int rows, int cols,FILE *inputFile, FILE *outputFile) {
    char c;
    long charNum = 0;
    long fileSize;
    int capacity = rows * cols;

    if (inputFile == NULL || outputFile == NULL) {
        printf("Blad otwierania jednego z plikow.\n");
        return;
    }

    // Read each character from the input file
    while (charNum < capacity) {
        fseek(inputFile, charNum, SEEK_SET);
        fread(&c, sizeof(char), 1, inputFile);
        char Directions[] = "GPDL";
        if (c == 'B') {
            char currentCharacter;
            int transformToSpace = 0;
            for (int i = 0; i < 4; i++) {
                int position = newPositionGetter(charNum, Directions[i]);
                if (position >= 0 && position < capacity) { 
                    fseek(inputFile, position, SEEK_SET);
                    if (fread(&currentCharacter, sizeof(char), 1, inputFile) == 1) {
                        if (currentCharacter == 'S') {
                            transformToSpace = 1;
                            break;
                        }
                    }
                    fseek(inputFile, charNum, SEEK_SET);  // Reset position
                }
            }
            c = transformToSpace ? ' ' : 'X';
        } else {
            switch (c) {
                case 'S':
                    c = ' ';
                    break;
                case 'P':
                case 'K':
                case '\n':
                    break;
                default:
                    c = 'X';
            }
        }

        // Write the transformed character to the output file
        fprintf(outputFile, "%c", c);
        charNum++;
    }
}




int main() {

    FILE* default_file;
    char fileName[] = "test.txt";
    default_file = fopen(fileName, "r+");
    if (!default_file) {
        perror("Error opening file");
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

    // //Na początku dodajemy polozenie startowe jako pierwszego node'a do kolejki
    struct Queue myQueueStruct;
    queue_t myQueue = &myQueueStruct;
    initialize_queue(myQueue);
    enqueue(myQueue, posP, 'P');
   
    int result = 1;
    while(result != 0)
    {
        
        result = bfsSearch(default_file, myQueue);
    }

    free_queue(myQueue);

    obecnekroki droga = zainiciuj();
    appendElement(droga, posK-1, 'L');

    result = 1;
    while(result != 0)
    {
        result = dfsSolve(default_file, droga, result);
    }

    
    int position;
    int calkdroga = 0;
    int currentPosition;
    int dlugoscDrogi = 0;
    char direction;
    char currentDirection;
    popElement(droga, &position, &direction);
    currentDirection;
    currentPosition;
    char reversedDir[] = "DLGP";
    char Directions[] = "GPDL";
    while(droga->n > 0 || currentPosition == (posK - 1))
    {
        
        popElement(droga, &currentPosition, &currentDirection);
        
        //Sa w tym samym wierszu
        if((int)(position / mazeSize.columns) == (int)(currentPosition/mazeSize.columns))
        {
            dlugoscDrogi = abs(position - currentPosition);
            //printf("Wiersz, Dlugosc drogi: %d\n", dlugoscDrogi);
            
            
        }
        //ta sama kolumna
        else{
            dlugoscDrogi = (int)((abs(position - currentPosition))/mazeSize.columns);
            //printf("kolumna, Dlugosc drogi: %d\n", dlugoscDrogi);
    
        }

        
        calkdroga += dlugoscDrogi;
        
        if(direction != currentDirection)
        {
            for(int i = 0; i<4; i++){
                if(direction == Directions[i])
                {
                    printf("Idz %d pola w kierunku %c\n",calkdroga,reversedDir[i]);
                    calkdroga = 0;
                }
            }
            
        }
        
        position = currentPosition;
        direction = currentDirection;
    } 




    zwolnijkroki(droga);

    FILE* output_file;
    char fileName1[] = "result.txt";
    output_file = fopen(fileName1, "w");
    transformAndWrite(rows,columns,default_file, output_file);
    fclose(default_file);
    fclose(output_file);
    return 0;
}