#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int logError(char*message) {
    char text[100];
    FILE *fptr;

    sprintf(text,"B\210\245d: %s \n",message);
    printf(text);

    fptr = fopen("errors.log","a");
    if(fptr == NULL)
    {
        printf("Nie można otworzyć logu, błąd krytyczny!");   
        exit(1);             
    }
    fprintf(fptr,"%s",text);
    fclose(fptr);

    return 0;
}