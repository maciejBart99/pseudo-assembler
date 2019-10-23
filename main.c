#include "declarations.h"

int main(int argc, char **argv)
{
    char fileName[STRING_BUFF_SIZE],outputs[STRING_BUFF_SIZE];
    char lastTag = ' ';
    size_t i;
    bool hasSetFile = false;
    long time,start;
    bool hasSetOutput=false;
    struct OrderList orders;
    struct CharArray oArray;
    extern bool logRegisters;

    if(argc==2)
    {
        strcpy(fileName, argv[1]);
        hasSetFile = true;
    } 
    else 
        for (i = 1; i < argc; i++)
        {
            if (*argv[i] == '-')
            {
                if(*(argv[i] + 1)=='r') logRegisters=true;
                else lastTag = *(argv[i] + 1);
            }
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
                        hasSetOutput=true;
                        break;
                    case 'i': parseInput(argv[i]);
                        break;
                    case ' ': 
                        strcpy(fileName, argv[i]);
                        hasSetFile = true;
                        break;
                }
                lastTag=' ';
            }
        }

    //check if user has selected file
    if (!hasSetFile) logError("Nie podano pliku!",0);

    //compile program
    printf("Trwa przetwarzanie pliku \253r\242d\210owego...\n");

    orders = parseScript(fileName);

    start = getMicrotime();

    //start program
    printf("Start programu...\n");
    
    executeScript(orders);

    time=(long)((getMicrotime() - start) / 100);

    if(hasSetOutput) printOutput(outputs);

    printf("\nWykonywanie programu trwa\210o ok. %d ms i zako\344czy\210o si\251 sukcesem!\n",time);

    return 0;
}

//parse string containing input varibles passed by user
void parseInput(char * args)
{
    struct CharArray arForVaribles,arForEquals,arForTable;
    struct Label *tmpLabel;
    extern struct Core core;
    char *buffer;
    int i;

    arForVaribles=str_split(args,';',SPLIT_LIMIT);

    for(i=0;i<arForVaribles.length;i++) 
    {
        arForEquals=str_split(arForVaribles.array[i],'=',2);

        tmpLabel=malloc(sizeof(struct Label));

        tmpLabel->key.hash=hash(arForEquals.array[0]);
        buffer=arForEquals.array[1];

        if(*buffer=='[') 
        {
            buffer++;
            buffer[strlen(buffer)-1]='\0';
            arForTable=str_split(strdup(buffer),',',SPLIT_LIMIT);
            tmpLabel->value.text=buffer;
            tmpLabel->length=arForTable.length;
        } 
        else 
        {
            tmpLabel->length=1;
            tmpLabel->value.target=atoi(buffer);
        }

        if(core.userInput.length>0) tmpLabel->previous=core.userInput.first;
        core.userInput.first=tmpLabel;
        core.userInput.length++;
    } 
}