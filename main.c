#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "structures.h"
#include "functions.h"

extern struct TagsList inputsList;

int main(int argc, char *argv[])
{
    char fileName[256];
    char lastTag = ' ';
    bool hasSetFile = false;
    bool hasSetOutput = false;
    bool hasSetInput=false;
    struct OrderList orders;
    long start;
    char outputs[200];
    struct CharArray oArray;

    if(argc==2)
    {
        strcpy(fileName, argv[1]);
        hasSetFile = true;
    } else {
    //get input cmd arguments
        for (size_t i = 1; i < argc; i++)
        {
            if (*argv[i] == '-')
                lastTag = *(argv[i] + 1);
            else
            {
                switch (lastTag)
                {
                case 'f':
                    strcpy(fileName, argv[i]);
                    hasSetFile = true;
                    break;
                case 'o':
                    strcpy(outputs, argv[i]);
                    hasSetOutput= true;
                    break;
                default:
                    break;
                }
            }
        }
    }


    //check if user has selected file
    if (!hasSetFile)
    {
        logError("Nie podano pliku!");
        exit(1);
    }

    //check if user has set the desired output
    if (!hasSetOutput)
    {
        printf("Podaj zmienne ktore maj\245 zosta\206 wypisane (rozdzielaj\245c przecinkiem, t:ZMIENNA wypisze ca\210a tablic\251, puste-pomi\344): ");
        fgets(outputs,200,stdin);
    }

    //compile program
    printf("Trwa kompilowanie pliku \253r\242d\210owego...\n");

    orders = parseScript(fileName);


    //read arguments from user
    for (size_t i = 0; i < inputsList.length; i++)
    {
        if((*(inputsList.tags+i)).array_len==1) {
            char buffer[200];
            printf("Podaj warto\230\206 pola %s (lub zostaw puste aby pomin\245\206): ",(*(inputsList.tags+i)).tag);
            fgets(buffer,200,stdin);
            (*(inputsList.tags+i)).target=atoi(buffer);
        } else {
            char buffer[200];
            printf("Podaj warto\230ci tablicy %s (rozdzielaj\245c przecinkami lub zostaw puste aby pomin\245\206): ",(*(inputsList.tags+i)).tag);
            fgets(buffer,200,stdin);
            if(strlen(buffer)==1)
                 (*(inputsList.tags+i)).target=-1;
            (*(inputsList.tags+i)).add=strdup(buffer);
        }
    }
    start = getMicrotime();

    //start program
    printf("Start programu...\n");

    
    executeScript(orders);

    //print the output
    oArray=str_split(trim(outputs),',',100);

    for(int i=0;i<oArray.length;i++) {
        size_t address;
        struct CharArray ar=str_split(*(oArray.array+i),':',3);

        if(ar.length==1){
            address=getMemoryAddress(trim(*(ar.array)),-1);
            printf("%s = %d\n",trim(*(oArray.array+i)),getMemoryValue(address,-1));
        }else {
            address=getMemoryAddress(trim(*(ar.array+1)),-1);
            printf("%s = [",trim(*(ar.array+1)));
            for(int j=getMemoryIndex(address);j<getMemoryRows(address);j++) {
                printf("%d",getMemoryValueByIndex(j));

                if(j+1!=getMemoryRows(address))
                    printf(",");
            }
            printf("]\n");
        }
    }

    printf("Wykonywanie programu trwa\210o %d ms i zako\344czy\210o si\251 sukcesem!\n",(long)((getMicrotime() - start) / 10));

    return 0;
}