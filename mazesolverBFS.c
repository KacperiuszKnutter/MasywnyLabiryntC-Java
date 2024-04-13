#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <labprocess.h>
#include <BFSqueue.h>
#include <DFSstack.h>
#include <binaryfilechanger.h>


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

    //printf("[%c]\n", originalchar);

    
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

    //printf("[%c]\n", originalchar);
    //printf("[%d]\n", myQueue->ile_node);
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




int main(int argc, char **argv) {
    int rows = 0, columns = 0, maxColumns = 0, maxRows = 0, iter = 0, posP = 0, posK = 0;
    char ch;
    if (argc < 2) {
        fprintf(stderr, "Aby uruchomic program podaj plik w formacie txt lub bin!");
        return -1;
    }
    FILE* default_file;
    char fileName[] = "test.txt";
    // jesli plik ma rozszerzenie bin
    if (strstr(argv[1], ".bin") != NULL) {
        binFileHeader_t *header = malloc(sizeof(binFileHeader_t));
        if (header == NULL) {
            perror("Blad alokacji pamieci dla naglowka pliku binarnego!");
            return -19;
        }

        mazeBuilder_t *mazeinfo = malloc(sizeof(mazeBuilder_t));
        if (mazeinfo == NULL) {
            perror("Blad alokacji pamieci dla specyfikacji labiryntu z pliku binarnego!");
            free(header);
            return -18;
        }
        //otwieramy go i czytamy
        FILE *inputFile = fopen(argv[1], "rb");
        if (!inputFile) {
            perror("Blad otwierania pliku binarnego");
            free(header);
            free(mazeinfo);
            return -1;
        }

        //printf("Znaleziono plik binarny, otwieramy: %s\n", argv[1]);
        //tworzymy plik tekstowy do ktorego wpisujemy labirynt ktory bedzie rozwiazywany
        default_file = fopen(fileName, "w");
        if (!default_file) {
            perror("Blad otwierania tworzonego pliku tekstowego");
            fclose(inputFile);
            free(header);
            free(mazeinfo);
            return -2;
        }
        //logika odpowiedzialna za odczytanie labiryntu z postaci binarnej
        collectBinaryDdataAndWriteToFile(inputFile, header, mazeinfo, default_file);
        //zamykamy gotowy plik jeden i drugi
        fclose(inputFile);
        fclose(default_file);
        //zwalniamy pamiec struktur
         default_file = fopen(fileName, "r+");
            if (!default_file) {
            perror("Blad otwierania pliku w przypaku binarnym");
            return -11;
            }
        columns =header->Columns+2;
        rows = header->Lines;
        char c1,c2;
        posP = columns * (header->EntryY-1) + (header->EntryX-1);
        posK = columns * (header->ExitY-1) + (header->ExitX-1);
        fseek(default_file,(long)posP,SEEK_SET);
        fread(&c1,sizeof(char),1,default_file);
        fseek(default_file,(long)posK,SEEK_SET);
        fread(&c2,sizeof(char),1,default_file);
        //("posP: %d\n",posP);
        //printf("posK: %d\n",posK);
        //printf("Znak posP: %c\n",c1);
        //printf("Znak posK: %c\n",c2);
        free(header);
        free(mazeinfo);
    }else{
    //zaczynamy prace na pliku tekstowym ktory mozemy rozwiazac
    default_file = fopen(fileName, "r+");
    if (!default_file) {
        perror("Blad otwierania pliku w przypadku tekstowym");
        return -12;
    }

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
    }
    rewind(default_file);
    //Tu plik z labiryntem ulega modyfikacji
    mark_Branching_Points(default_file, rows, columns);

    //printf("Columns: %d\n", columns);
    mazeSize.columns = columns;
    //printf("Rows: %d\n", rows);
    mazeSize.rows = rows;
    //printf("Pozycja Konc: %d\n", posK);
    //printf("Pozycja Pocz: %d\n", posP);
    // //Na początku dodajemy polozenie startowe jako pierwszego node'a do kolejki
    struct Queue myQueueStruct;
    queue_t myQueue = &myQueueStruct;
    initialize_queue(myQueue);
    enqueue(myQueue, posP, 'P');
   
   //Szukamy dopoki nie znajdziemy K
    int result = 1;
    while(result != 0)
    {
        
        result = bfsSearch(default_file, myQueue);
    }
    //zwalniamy kolejke
    free_queue(myQueue);

    //tworzymy stos krokow (wierzcholkow ktore sprawdzilismy)
    obecnekroki droga = zainiciuj();
    appendElement(droga, posK-1, 'L');

    //usuwamy sciezki od konca ktore nie prowadza do poczatka tak by zostawic jedyna najkrotsza
    result = 1;
    while(result != 0)
    {
        result = dfsSolve(default_file, droga, result);
    }
    //Gdy nie korzystamy ze stosu zwalniamy pamiec
    zwolnijkroki(droga);
    //tworzymy plik ze sciezka z przejsciem labiryntu
    FILE* output_file;
    char fileName1[] = "result.txt";
    output_file = fopen(fileName1, "w");
    transformAndWrite(rows,columns,default_file, output_file);
    fclose(default_file);
    fclose(output_file);
    return 0;
}