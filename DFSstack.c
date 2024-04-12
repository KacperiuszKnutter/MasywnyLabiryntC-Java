#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "DFSstack.h"


obecnekroki zainiciuj()
{
    obecnekroki droga = malloc(sizeof(*droga));
    if(droga == NULL)
    {
        printf("BLAD PODCCZAS ALOKACJI PAMIECI");
        return NULL;
    }
    droga->n = 0;
    droga->head = NULL;
    return droga;
}

obecnekroki appendElement(obecnekroki droga, int position, char direction)
{
    krok nowykrok = malloc(sizeof(*nowykrok));
    krok temp;

    if(nowykrok == NULL)
    {
        printf("BLAD PODCCZAS ALOKACJI PAMIECI");
        return NULL;
    }

    nowykrok->kierunek = direction;
    nowykrok->pozycja = position;

    temp = droga->head;
    droga->head = nowykrok;
    nowykrok->next= temp;
    
    droga->n++;
    return droga;
}

krok popElement(obecnekroki droga, int *position, char *direction)
{
    krok ostatnikrok = droga->head;
    if(droga->n > 0)
    {
        droga->head = droga->head->next;
        free(ostatnikrok);
        droga->n--;
        *position = ostatnikrok->pozycja;
        *direction = ostatnikrok->kierunek;
        return ostatnikrok;
    }
    else{
        printf("Nie ma juz elementow na stosie\n");
        return NULL;
    }

}


krok peekPath(obecnekroki droga, int *position, char *direction)
{
    krok ostatnikrok = droga->head;
    if(droga->n > 0)
    {
        *position = ostatnikrok->pozycja;
        *direction = ostatnikrok->kierunek;
        return ostatnikrok;
    }
    else{
        return NULL;
    }
}


void zwolnijkroki(obecnekroki droga)
{
    krok obecnykrok = droga->head;
    while (obecnykrok != NULL)
    {
        krok temp = obecnykrok->next; 
        free(obecnykrok); 
        obecnykrok = temp; 
    }
    free(droga); 
}
