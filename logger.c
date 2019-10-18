#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include <string.h>
#include <math.h>
#include "declarations.h"

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
    FILE *fptr;

    sprintf(text,"B\210\245d w %d lini: %s \n",line,message);
    printf(text);
    exit(1);
}

//function to print output after program is done
void printOutput(char* outputs) 
{
    struct CharArray oArray=str_split(trim(outputs),',',100);

    for(int i=0;i<oArray.length;i++) 
    {
        size_t address;
        struct CharArray ar=str_split(oArray.array[i],':',3);

        if(ar.length==1)
        {
            address=getMemoryAddress(trim(ar.array[0]),-1);
            printf("\n%s = %d\n",trim(oArray.array[i]),getMemoryValue(address,0));
        }
        else 
        {
            address=getMemoryAddress(trim(ar.array[1]),-1);
            printf("\n%s = [",trim(ar.array[1]));
            for(int j=getMemoryIndex(address);j<getMemoryRows(address);j++) 
            {
                printf("%d",getMemoryValueByIndex(j));

                if(j+1!=getMemoryRows(address)) printf(",");
            }
            printf("]\n");
        }
    }
}

//print information about register's changes
void printRegisterChange(short reg_num) 
{
    extern struct Register registers[];
    extern bool logRegisters;
    
    if(logRegisters) {
        printf("\nWarto\230\206 rejestru %d zosta\210a zmieniona:\n",reg_num);
        printf("Nowa warto\230\206(dec): %d, (u2-hex): 0x%s\n",registers[reg_num].value,decToHexU2(registers[reg_num].value));
    }
}