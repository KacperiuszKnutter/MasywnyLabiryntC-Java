#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct binFileHeader {
    uint32_t FileId;
    uint8_t Escape;
    uint16_t Columns;
    uint16_t Lines;
    uint16_t EntryX;
    uint16_t EntryY;
    uint16_t ExitX;
    uint16_t ExitY;
    uint32_t Reserved[3];
    uint32_t Counter;
    uint32_t SolutionOffset;
    uint8_t Separator;
    uint8_t Wall;
    uint8_t Path;
} binFileHeader_t;

typedef struct mazeBuilder {
    uint8_t Separator;
    uint8_t Value;
    uint8_t Count;
} mazeBuilder_t;

void collectBinaryDdataAndWriteToFile(FILE *binFile, binFileHeader_t *header, mazeBuilder_t *maze, FILE *outPutFile);