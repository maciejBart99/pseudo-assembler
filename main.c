#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include "structures.h"
#include "functions.h"


int main(int argc,char *argv[]) {
    char fileName[256];
    char lastTag=' ';
    bool hasSetFile=false;
    struct OrderList orders;
    long start=getMicrotime();


    for(size_t i=1;i<argc;i++) {
       if(*argv[i]=='-')lastTag=*(argv[i]+sizeof(char));
       else {
           switch (lastTag)
           {
           case 'i':strcpy(fileName,argv[i]);
                    hasSetFile=true;
               break;
           default:
               break;
           }
       }
    }

    if(!hasSetFile) {
        logError("Nie podano pliku");
        exit(1);
    }

    printf("Trwa kompilowanie pliku \253r\242d\210owego...\n");

    orders=parseScript(fileName);

    printf("Start programu...\n");

    executeScript(orders);

    printf("Wykonywanie programu trwa\210o %d ms i zako\344czy\210o si\251\n",(long)(getMicrotime()-start)/10);

    return 0;
}