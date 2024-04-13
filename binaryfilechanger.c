#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <binaryfilechanger.h>

void collectBinaryDdataAndWriteToFile(FILE *binFile, binFileHeader_t *header, mazeBuilder_t *maze, FILE *outPutFile) {
    //wczytywanie naglowka
    fread(&header->FileId, sizeof(uint32_t), 1, binFile);
    fread(&header->Escape, sizeof(uint8_t), 1, binFile);
    fread(&header->Columns, sizeof(uint16_t), 1, binFile);
    fread(&header->Lines, sizeof(uint16_t), 1, binFile);
    fread(&header->EntryX, sizeof(uint16_t), 1, binFile);
    fread(&header->EntryY, sizeof(uint16_t), 1, binFile);
    fread(&header->ExitX, sizeof(uint16_t), 1, binFile);
    fread(&header->ExitY, sizeof(uint16_t), 1, binFile);
    fread(&header->Reserved[0], sizeof(uint32_t), 1, binFile);
    fread(&header->Reserved[1], sizeof(uint32_t), 1, binFile);
    fread(&header->Reserved[2], sizeof(uint32_t), 1, binFile);
    fread(&header->Counter, sizeof(uint32_t), 1, binFile);
    fread(&header->SolutionOffset, sizeof(uint32_t), 1, binFile);
    fread(&header->Separator, sizeof(uint8_t), 1, binFile);
    fread(&header->Wall, sizeof(uint8_t), 1, binFile);
    fread(&header->Path, sizeof(uint8_t), 1, binFile);

    //wyliczanie pozycji P i K w pliku
    int lineCounter = 0;
    int start = (header->Columns) * (header->EntryY-1) + (header->EntryX-1);
    int end = (header->Columns) * (header->ExitY-1) + (header->ExitX-1);

    for (int i = 0; i < header->Counter; i++) {//Petla zewnetrzna po znakach jakie nas interesuja
        fread(&maze->Separator, sizeof(uint8_t), 1, binFile);
        fread(&maze->Value, sizeof(uint8_t), 1, binFile);
        fread(&maze->Count, sizeof(uint8_t), 1, binFile);

        for (int j = 0; j <= maze->Count; j++) {//Petla wewnetrzna ile razy wpisujemy dany znak
          

            if (lineCounter % (header->Columns) == 0 && lineCounter != 0) {
                fputc('\n', outPutFile);
            }
            if (lineCounter == start)
                fputc('P', outPutFile);
            else if (lineCounter == end)
                fputc('K', outPutFile);
            else
                fputc((char)maze->Value, outPutFile);

            lineCounter++; 
        }
    }
}

