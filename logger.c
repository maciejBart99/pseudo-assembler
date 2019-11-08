#include "declarations.h"
#include <windows.h>

bool logAll=false;

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
    showcursor();
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

    printf("\n");

    splitByComa=str_split(outputs,',',100);

    for(i=0;i<splitByComa.length;i++) 
    {
        splitByDot=str_split(strdup(splitByComa.array[i]),':',2);
        tmpLabel=getLabel(hash(splitByDot.array[splitByDot.length-1]),false,0);

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

void printMemoryInit(char * cmd,int size,int address,bool def,int value)
{
    extern struct Core core;
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if(core.mode==STEPS)
    {
        SetConsoleTextAttribute(hConsole, YELLOW);
        printf("\n\n%-40s",cmd);
        SetConsoleTextAttribute(hConsole, WHITE);
        if(size==1)
        {
            if(def) printf("Zadeklarowano 4 bajty - 0x%s = %d",decToHexU2(address),value);
            else printf("Zadeklarowano 4 bajty - 0x%s",decToHexU2(address));
        }
        else printf("Zadeklarowano %d bajty/bajt\242w - 0x%s",size*4,decToHexU2(address));
    }
}

//print information about memory changes
void printJump(char * cmd,bool success)
{
    extern struct Core core;
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if(core.mode==STEPS)
    {
        SetConsoleTextAttribute(hConsole, YELLOW);
        printf("\n\n%-40s",cmd);
        if(success)
        {
            SetConsoleTextAttribute(hConsole, GREEN);
            printf("Wykonano skok");
        }
        else
        {
            SetConsoleTextAttribute(hConsole, CYAN);
            printf("Niewykonano skoku");
        }

        SetConsoleTextAttribute(hConsole, WHITE);
    }
}

//print information about memory changes
void printCompare(short state,char * cmd)
{
    extern struct Core core;
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if(core.mode==STEPS)
    {
        SetConsoleTextAttribute(hConsole, YELLOW);
        printf("\n\n%-40s", cmd);
        SetConsoleTextAttribute(hConsole, WHITE);
        printf("W rejestrze stanu ustawiono flag\251 na %u", state);
    }
}

//print information about memory changes
void printMemoryChange(int address,char * cmd)
{
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, YELLOW);
    printf("\n\n%-40s",cmd);
    SetConsoleTextAttribute(hConsole, WHITE);
    printf("Kom\242rka 0x%s = %d (0x%s)",decToHexU2(address),getMemoryValue(address),decToHexU2(getMemoryValue(address)));
}

//print information about register's changes
void printRegisterChange(short reg_num,char * cmd)
{
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, YELLOW);
    printf("\n\n%-40s",cmd);
    SetConsoleTextAttribute(hConsole, WHITE);
    printf("Rejestr %d = %d (0x%s)",reg_num,getRegisterValue(reg_num,0),decToHexU2(getRegisterValue(reg_num,0)));
}

void dumpRegisters()
{
    int i,buffer=0;
    int columns, rows;
    HANDLE hConsole;
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    SetConsoleTextAttribute(hConsole, YELLOW);
    printf("\n\nRejestry:\n");
    SetConsoleTextAttribute(hConsole, WHITE);
    for(i=0;i<LAST_REGISTER;i++)
    {
        if(columns<buffer+REG_DISPLAY_SIZE)
        {
            printf("\n");
            buffer=0;
        }
        printf("%3d = %8d(0x%s)",i,getRegisterValue(i,0),decToHexU2(getRegisterValue(i,0)));
        buffer+=REG_DISPLAY_SIZE;
    }
}