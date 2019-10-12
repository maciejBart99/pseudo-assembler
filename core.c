#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "structures.h"
#include "functions.h"
#include "math.h"

struct TagsList jumpTags = {0,NULL};
struct TagsList memoryTags = {0,NULL};
struct Memory memory={NULL,0};
struct Register registers[]={{0,0},{1,0},{2,0},{3,0},{4,0},{5,0},{6,0},{7,0},{8,0},{9,0},{10,0},{11,0},{12,0},{13,0},{14,0},{15,0}};
short stateRegisterValue=0;

enum OpeartionType{ADD,SUBSTRACT,MULTIPLY,DIVIDE,COMPARE};

enum TransferType{MtoR,RtoM,RtoR,AtoR};

enum DataSource {MEMORY,REGISTER};

void addJumpTag(char* tag,int target) {
    struct Tag tagS;

    tagS.target=target;
    tagS.tag=tag;

    if(jumpTags.length==0) {
        *(jumpTags.tags)=tagS;
        jumpTags.length=1;
    } else {
        jumpTags.tags[jumpTags.length]=tagS;
        jumpTags.length++;
    }

}

void addMemoryTag(char* tag,int target) {
    struct Tag tagS;

    tagS.target=target;
    tagS.tag=tag;

    if(memoryTags.length==0) {
        
        *memoryTags.tags=tagS;
        memoryTags.length=1;
    } else {
        *(memoryTags.tags+memoryTags.length)=tagS;
        memoryTags.length=1;
    }

}

void allocateMemory(char* tag,long value,short size,bool makeTag) {

    size_t address;
    struct MemoryCell cell;

    if(memoryTags.length==0) {
        address=0;
    } else {
        struct MemoryCell mem=*(memory.cells+memory.length-1);
        address=mem.address+mem.size;
    }

    cell.size=size;
    cell.value=value;
    cell.address=address;
    if(makeTag) addMemoryTag(tag,address);
}

long getMemoryAddress(char * tag,int line) {
    size_t address;
    
    for(size_t i=0;i<memoryTags.length;i++) {
        struct Tag t=*(memoryTags.tags+i);
        if(strcmp(t.tag,tag)) {
            return t.target;
        }
    }

    char*buffer;

    sprintf(buffer,"Nieodnaleziono komórki pamięci o podanym adresie w %d lini!",line);
    logError(buffer);
    exit(1);
}

long getMemoryValue(size_t addressA,int line) {   
    for(size_t i=0;i<memory.length;i++) {
        struct MemoryCell m=*(memory.cells+i);
        if(m.address==addressA) {
            return m.value;
        }
    }

    char*buffer;

    sprintf(buffer,"Nieodnaleziono komórki pamięci o podanym adresie w %d lini!",line);
    logError(buffer);
    exit(1);
}

size_t getJumpTarget(char * tag,int line) {   
    for(size_t i=0;i<memory.length;i++) {
        struct Tag t=*(jumpTags.tags+i);
        if(strcmp(tag,t.tag)) {
            return t.target;
        }
    }

    char*buffer;

    sprintf(buffer,"Nieodnaleziono wskazanej etykiety w %d lini!",line);
    logError(buffer);
    exit(1);
}

void setMemoryValue(size_t addressA,long value,int line) {   
    for(size_t i=0;i<memory.length;i++) {
        struct MemoryCell m=*(memory.cells+i);
        if(m.address==addressA) {
            (*(memory.cells+i)).value=value;
            return;
        }
    }

    char*buffer;

    sprintf(buffer,"Nieodnaleziono komórki pamięci o podanym adresie w %d lini!",line);
    logError(buffer);
    exit(1);
}

void mathOperation(char* to,char* from, enum DataSource secondSource,enum OpeartionType type,int line) {
    int valueA,valueB,addressA,addressB;

    if(isNumeric(to)) {
        short tmp=atoi(to);

        if(tmp<0||tmp>13) {
            char*buffer;

            sprintf(buffer,"Odwołanie do rejestru musi się odbywać poprzez liczbę od 0 do 13 %d lini!",line);
            logError(buffer);
            exit(1);
        } else {
            addressA=tmp;
        }
    } else {
        char*buffer;

        sprintf(buffer,"Odwołanie do rejestru musi się odbywać poprzez liczbę od 0 do 13 %d lini!",line);
        logError(buffer);
        exit(1);
    }

    if(secondSource==MEMORY) {
        valueB=getMemoryValue(getMemoryAddress(from,line),line);
    } else {
        if(isNumeric(from)) {
        short tmp=atoi(from);
            if(tmp<0||tmp>13) {
                char*buffer;

                sprintf(buffer,"Odwołanie do rejestru musi się odbywać poprzez liczbę od 0 do 13 %d lini!",line);
                logError(buffer);
                exit(1);
            } else {
                addressB=tmp;
            }
        } else {
            char*buffer;

            sprintf(buffer,"Odwołanie do rejestru musi się odbywać poprzez liczbę od 0 do 13 %d lini!",line);
            logError(buffer);
            exit(1);
        }

        valueB=registers[addressB].value;
    }

    switch (type)
    {
        case ADD:   registers[addressA].value=valueB+valueA;
            break;
        case SUBSTRACT:   registers[addressA].value=valueA-valueB;
            break;
        case MULTIPLY:   registers[addressA].value=valueB*valueA;
            break;
        case DIVIDE:   registers[addressA].value=valueA/valueB;
            break;
        case COMPARE:   registers[addressA].value=valueA-valueB;
            break;
    }

    if(registers[addressA].value>0) {
            stateRegisterValue=1;
    } else if(registers[addressA].value<0) {
        stateRegisterValue=2;
    } else {
        stateRegisterValue=0;
    }

    if(type==COMPARE) {
        registers[addressA].value=valueA;
    } 
}

void transferData(char* to,char* from, enum TransferType type,int line) {
    int valueA,valueB,addressA,addressB;

    if(isNumeric(to)) {
        short tmp=atoi(to);

        if(tmp<0||tmp>13) {
            char*buffer;

            sprintf(buffer,"Odwołanie do rejestru musi się odbywać poprzez liczbę od 0 do 13 %d lini!",line);
            logError(buffer);
            exit(1);
        } else {
            addressA=tmp;
        }
    } else {
        char*buffer;

        sprintf(buffer,"Odwołanie do rejestru musi się odbywać poprzez liczbę od 0 do 13 %d lini!",line);
        logError(buffer);
        exit(1);
    }

    switch (type)
    {
        case MtoR: registers[addressA].value=getMemoryValue(getMemoryAddress(from,line),line);
            break;
        case RtoM: setMemoryValue(getMemoryAddress(from,line),registers[addressA].value,line);
            break;
        case RtoR: 
                    if(isNumeric(from)) {
                        short tmp=atoi(from);

                        if(tmp<0||tmp>13) {
                            char*buffer;

                            sprintf(buffer,"Odwołanie do rejestru musi się odbywać poprzez liczbę od 0 do 13 %d lini!",line);
                            logError(buffer);
                            exit(1);
                        } else {
                            addressB=tmp;
                        }
                    } else {
                        char*buffer;

                        sprintf(buffer,"Odwołanie do rejestru musi się odbywać poprzez liczbę od 0 do 13 %d lini!",line);
                        logError(buffer);
                        exit(1);
                    }

                    registers[addressA]=registers[addressB];
            break;
        case AtoR: registers[addressA].value=getMemoryAddress(from,line);
            break;
    }
}

struct Order* parseOrder(char *line,int index) {
    struct CharArray args;
    int elementsCount;
    size_t tmpSize;

    args=str_split(line,' ');

    if(args.length>3) {
        char*buffer;

        sprintf(buffer,"Błąd składni w %d lini!",index);
        logError(buffer);
        exit(1);
    }
    if(args.length==1) {
        struct Order* order=malloc(sizeof(struct Order));
        strcpy(order->tag,*args.array);
        strcpy(order->command,"");
        addJumpTag(*args.array,index);
        return order;
    }
    

    if(checkIfKeyWord(*args.array)) {
        if(args.length==2) {
            struct Order* order=malloc(sizeof(struct Order));
            order->tag="";
            order->command=*args.array;
            order->args=*(args.array+1);
            return order;
        } else {
            char*buffer;

            sprintf(buffer,"Błąd składni w %d lini!",index);
            logError(buffer);
            exit(1);
        }

    } else if(args.length==3) {
        struct Order* order=malloc(sizeof(struct Order));
        order->tag=*args.array;
        order->command=*(args.array+1);
        order->args=*(args.array+2);
        return order;
    } else {
        char*buffer;

        sprintf(buffer,"Błąd składni w %d lini!",index);
        logError(buffer);
        exit(1);
    }

}

struct OrderList parseScript(char *fileName) {
    static struct Order ** resultList;
    struct OrderList list;
    struct Order buffer;
    FILE *fptr;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    size_t resultLength=0;

    //alocate memory for address linkers
    jumpTags.tags=malloc(16*100);
    memoryTags.tags=malloc(16*100);
    //open script
    fptr = fopen(fileName, "r");
    if (fptr == NULL)
    {
        logError("Nie można otworzyć wskazanego pliku");
        exit(1);
    }

    //read script line by line
    while ((read = getline(&line, &len, fptr)) != -1) {
        if(resultLength==0) {
            if(read>2) {
                resultLength=1;
                resultList=malloc(4);
                resultList[0]=parseOrder(line,resultLength);
            }
        } else {
            if(read>2) {
                resultLength++;
                resultList=realloc(resultList,resultLength*4);
                resultList[resultLength-1]=parseOrder(line,resultLength);
            }
        }
    }
    
    fclose(fptr);    

    list.length=resultLength;
    list.orders=resultList;

    return list;
}

size_t executeOrder(struct Order* order,int line) {
    struct CharArray arguments=str_split(order->args,',');
   
    if(strcmp(order->command,"")) {
        return line;
    }

    //memory allocation
    if(strcmp(order->command,"DC")||strcmp(order->command,"DS")) {
        struct CharArray mul=str_split(*(arguments.array),'*');
        size_t up;
        struct CharArray ar;
        char * dType;
        short size;
        long valueInt;

        //check if array
        if(mul.length==1) {
            ar=str_split(*mul.array,'(');
            up=1;
        } else {
            ar=str_split(*(mul.array+1),'(');
            up=_atoi64(*(mul.array));
        }

        if(up<1) {
            char*buffer;

            sprintf(buffer,"Nie można utowrzyć tablicy o podanym rozmiarze w %d lini!",line);
            logError(buffer);
            exit(1);
        }

        //check data type
        if(strcmp(*(ar.array),"INTEGER")||strcmp(*(ar.array),"INT")||strcmp(*(ar.array),"INT32")) {
            size=4;
        } else if(strcmp(*(ar.array),"SHORT")||strcmp(*(ar.array),"INT16")) {
            size=2;
        } else if(strcmp(*(ar.array),"BYTE")||strcmp(*(ar.array),"CHAR")||strcmp(*(ar.array),"INT8")) {
            size=1;
        } else {
            char*buffer;

            sprintf(buffer,"Nieobsługiwany typ danych w %d lini!",line);
            logError(buffer);
            exit(1);
        }

        //retrive value
        if(ar.length==2) {
            char*value=*(ar.array+1);
            
            value[strlen(value)-1] = 0;
            valueInt=_atoi64(value);
            if(valueInt>pow(2,(size*8)-1)||valueInt<-pow(2,(size*8)-1)) {
                char*buffer;

                sprintf(buffer,"Liczba z poza zakresu w %d lini!",line);
                logError(buffer);
                exit(1);
            }

        } else if(ar.length==1) {
            valueInt=0;
        } else {
            char*buffer;

            sprintf(buffer,"Błąd składni w %d lini!",line);
            logError(buffer);
            exit(1);
        }

        //alocate single/array cell
        for(size_t i=0;i<up;i++) {
            allocateMemory(order->tag,valueInt,size,i==0);
        }
    //math instructions
    } else if(strcmp(order->command,"A")) {
        mathOperation(*(arguments.array),*(arguments.array+1),MEMORY,ADD,line);
    } else if(strcmp(order->command,"AR")) {
        mathOperation(*(arguments.array),*(arguments.array+1),REGISTER,ADD,line);
    } else if(strcmp(order->command,"S")) {
        mathOperation(*(arguments.array),*(arguments.array+1),MEMORY,SUBSTRACT,line);
    } else if(strcmp(order->command,"SR")) {
        mathOperation(*(arguments.array),*(arguments.array+1),REGISTER,SUBSTRACT,line);
    } else if(strcmp(order->command,"M")) {
        mathOperation(*(arguments.array),*(arguments.array+1),MEMORY,MULTIPLY,line);
    } else if(strcmp(order->command,"MR")) {
        mathOperation(*(arguments.array),*(arguments.array+1),REGISTER,MULTIPLY,line);
    } else if(strcmp(order->command,"D")) {
        mathOperation(*(arguments.array),*(arguments.array+1),MEMORY,DIVIDE,line);
    } else if(strcmp(order->command,"DR")) {
        mathOperation(*(arguments.array),*(arguments.array+1),REGISTER,MULTIPLY,line);
    } else if(strcmp(order->command,"C")) {
        mathOperation(*(arguments.array),*(arguments.array+1),MEMORY,COMPARE,line);
    } else if(strcmp(order->command,"CR")) {
        mathOperation(*(arguments.array),*(arguments.array+1),REGISTER,COMPARE,line);
    } 
    //data tranfer
    else if(strcmp(order->command,"L")) {
        transferData(*(arguments.array),*(arguments.array+1),MtoR,line);
    } else if(strcmp(order->command,"LR")) {
        transferData(*(arguments.array),*(arguments.array+1),RtoR,line);
    } else if(strcmp(order->command,"ST")) {
        transferData(*(arguments.array),*(arguments.array+1),RtoM,line);
    } else if(strcmp(order->command,"LA")) {
        transferData(*(arguments.array),*(arguments.array+1),AtoR,line);
    }
    //jumps
    else if(strcmp(order->command,"J")) {
        return getJumpTarget(*(arguments.array),line)-1;
    } else if(strcmp(order->command,"JN")) {
        if(stateRegisterValue==2) return getJumpTarget(*(arguments.array),line)-1;
    } else if(strcmp(order->command,"JP")) {
        if(stateRegisterValue==1) return getJumpTarget(*(arguments.array),line)-1;
    } else if(strcmp(order->command,"JZ")) {
        if(stateRegisterValue==0) return getJumpTarget(*(arguments.array),line)-1;
    }

    return line;
}

void executeScript(struct OrderList orders){
    for(size_t i=0;i<orders.length;i++) {
        struct Order* order=*(orders.orders+i);
        i=executeOrder(order,i);
    }
}

void clearMemory(struct OrderList listToClear) {

}