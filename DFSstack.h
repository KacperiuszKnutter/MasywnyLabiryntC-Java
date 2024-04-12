#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


//implementacja stosu ruchow
typedef struct krok{
    char kierunek;// G P D L
    int pozycja;
    struct krok *next;
} *krok;

//head ktory liczy ile mamy krokow
typedef struct{
    krok head;
    int n;
}*obecnekroki; 

obecnekroki zainiciuj();

obecnekroki appendElement(obecnekroki droga, int position, char direction);

krok popElement(obecnekroki droga, int *position, char *direction);

krok peekPath(obecnekroki droga, int *position, char *direction);

void zwolnijkroki(obecnekroki droga);


void wypiszDroge(obecnekroki droga);