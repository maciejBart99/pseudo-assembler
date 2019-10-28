#include "declarations.h"
#include <windows.h>

bool logRegisters=false;

//convert dec number to u2-hex
char * decToHexU2(long num) 
{ 
    char hexNum[]="00000000";
    unsigned long U_num;

    if(num<0) 
    {
        U_num=-num;
        U_num=~U_num;
        U_num++;
    }
    else U_num=num;

    int i = 0; 
    while (U_num > 0) 
    {
        int tmp;

        tmp = U_num % 16; 
        U_num = floor(U_num / 16); 
        i++;

        if(tmp < 10) hexNum[8-i] = tmp + 48; 
        else hexNum[8-i] = tmp + 55; 
    } 
    
    return strdup(hexNum);
}

//log errors
void logError(char*message,int line) 
{
    char text[200];
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, RED);
    if(line>0) sprintf(text,"\nB\210\245d w %d lini: %s \n",line,message);
    else sprintf(text,"\nB\210\245d w poleceniu: %s \n",message);
    printf(text);
    SetConsoleTextAttribute(hConsole, WHITE);
    exit(1);
}

//function to print output after program is done
void printOutput(char* outputs) 
{
    int i,j;
    struct Label* tmpLabel;
    struct CharArray splitByComa,splitByDot;
    
    splitByComa=str_split(outputs,',',100);

    for(i=0;i<splitByComa.length;i++) 
    {
        splitByDot=str_split(strdup(splitByComa.array[i]),':',2);
        tmpLabel=getLabel(hash(splitByDot.array[splitByDot.length-1]));

        if(tmpLabel==NULL) logError("Podane pole nie istnieje!",0);

        if(splitByDot.length==2)
        {
            printf("\n%s = [",splitByDot.array[splitByDot.length-1]);

            for(j=0;j<tmpLabel->length;j++)
            {
                printf("%d",getMemoryValue(tmpLabel->value.target+(sizeof(int)*j)));

                if(j+1!=tmpLabel->length) printf(",");
            }

            printf("]\n");
        }
        else printf("\n%s = %d\n",splitByDot.array[splitByDot.length-1],getMemoryValue(tmpLabel->value.target));
    }
}

//print information about register's changes
void printRegisterChange(short reg_num) 
{
    extern bool logRegisters;
    
    if(logRegisters) {
        printf("\n\nWarto\230\206 rejestru %d zosta\210a zmieniona:\n",reg_num);
        printf("Nowa warto\230\206(dec): %d, (u2-hex): 0x%s\n",getRegisterValue(reg_num,0),decToHexU2(getRegisterValue(reg_num,0)));
    }
}