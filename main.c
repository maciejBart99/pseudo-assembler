#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "structures.h"
#include "functions.h"

int main(int argc, char *argv[])
{
    char fileName[256];
    char lastTag = ' ';
    bool hasSetFile = false;
    bool hasSetOutput = false;
    struct OrderList orders;
    long start;
    char outputs[200];
    struct CharArray oArray;


    //get input cmd arguments
    for (size_t i = 1; i < argc; i++)
    {
        if (*argv[i] == '-')
            lastTag = *(argv[i] + 1);
        else
        {
            switch (lastTag)
            {
            case 'i':
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

    //check if user has selected file
    if (!hasSetFile)
    {
        logError("Nie podano pliku");
        exit(1);
    }

    //check if user has set the desired output
    if (!hasSetOutput)
    {
        printf("Podaj zmienne ktore maja zostac wypisane (rozdzielajac przecinkiem, t:ZMIENNA wypisze cala tablice):\n");
        fgets(outputs,200,stdin);
    }

    //compile program
    printf("Trwa kompilowanie pliku \253r\242d\210owego...\n");

    start = getMicrotime();

    orders = parseScript(fileName);

    //start program
    printf("Start programu...\n");

    executeScript(orders);

    //print the output
    oArray=str_split(trim(outputs),',');

    for(int i=0;i<oArray.length;i++) {
        size_t address;
        struct CharArray ar=str_split(*(oArray.array+i),':');

        if(ar.length==1){
            address=getMemoryAddress(*(ar.array),-1);
            printf("%s = %d\n",*(oArray.array+i),getMemoryValue(address,-1));
        }else {
            address=getMemoryAddress(*(ar.array+1),-1);
            printf("%s = [",*(ar.array+1));
            for(int j=getMemoryIndex(address);j<getMemoryRows(address);j++) {
                printf("%d",getMemoryValueByIndex(j));

                if(j+1!=getMemoryRows(address))
                    printf(",");
            }
            printf("]\n");
        }
    }

    printf("Wykonywanie programu trwa\210o %d ms i zako\344czy\210o si\251 sukcesem!\n", (long)(getMicrotime() - start) / 10);

    return 0;
}