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
struct TagsList inputsList={0,NULL};

//local enums def
enum OpeartionType{ADD,SUBSTRACT,MULTIPLY,DIVIDE,COMPARE};

enum TransferType{MtoR,RtoM,RtoR,AtoR};

enum DataSource {MEMORY,REGISTER};

void addJumpTag(char* tag,size_t target) {
    struct Tag tagS;

    tagS.target=target;
    tagS.tag=tag;

    if(jumpTags.length==0) {
        *(jumpTags.tags)=tagS;
        jumpTags.length=1;
    } else {

        if(jumpTags.length%10==0) {
            jumpTags.tags=realloc(jumpTags.tags,(jumpTags.length+10)*sizeof(struct Tag));
        }

        jumpTags.tags[jumpTags.length]=tagS;
        jumpTags.length++;
    }

}

void addMemoryTag(char* tag,size_t target,size_t rows) {
    struct Tag tagS;

    tagS.target=target;
    tagS.tag=tag;
    tagS.array_len=rows;

    if(memoryTags.length==0) {
        *(memoryTags.tags)=tagS;
        memoryTags.length=1;
    } else {

        if(memoryTags.length%10==0) {
            memoryTags.tags=realloc(memoryTags.tags,(memoryTags.length+10)*sizeof(struct Tag));
        }

        memoryTags.tags[memoryTags.length]=tagS;
        memoryTags.length++;
    }

}

size_t allocateMemory(char* tag,long value,short size,bool makeTag,size_t rows) {

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

    if(memory.length==0) {
        memory.cells=malloc(sizeof(struct Memory));
        memory.cells[0]=cell;
        memory.length=1;
    } else {
        memory.cells=realloc(memory.cells,(memory.length+1)*sizeof(struct Memory));
        memory.cells[memory.length]=cell;
        memory.length++;
    }

    if(makeTag) addMemoryTag(tag,address,rows);

    return cell.address;
}

long getMemoryAddress(char * tag,int line) {
    size_t address;
    
    for(size_t i=0;i<memoryTags.length;i++) {
        struct Tag t=*(memoryTags.tags+i);

        if(strcmp(t.tag,tag)==0) {
            return t.target;
        }
    }


    char*buffer;

    sprintf(buffer,"Nieodnaleziono kom\242rki pami\251ci o podanej etykiecie w %d lini!",line+1);
    logError(buffer);
    exit(1);
}

size_t getMemoryRows(size_t addressA) {   
    for(size_t i=0;i<memory.length;i++) {
        struct Tag m=*(memoryTags.tags+i);
        if(m.target==addressA) {
            return m.array_len;
        }
    }

    logError("Nieodnaleziono podanego adresu!");
    exit(1);
}

size_t getMemoryIndex(size_t addressA) {   
    for(size_t i=0;i<memoryTags.length;i++) {
        struct MemoryCell m=*(memory.cells+i);

        if(m.address==addressA)
            return i;
    }

    logError("Nieodnaleziono podanego adresu");
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

    sprintf(buffer,"Nieodnaleziono kom\242rki pami\251ci o podanym adresie w %d lini!",line+1);
    logError(buffer);
    exit(1);
}

long getMemoryValueByIndex(size_t index) {   
   return (*(memory.cells+index)).value;
}

size_t getJumpTarget(char * tag,int line) {   

    for(size_t i=0;i<jumpTags.length;i++) {
        struct Tag t=*(jumpTags.tags+i);

        if(strcmp(tag,t.tag)==0) {
            return t.target;
        }
    }

    char*buffer;

    sprintf(buffer,"Nieodnaleziono wskazanej etykiety w %d lini!",line+1);
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

    sprintf(buffer,"Nieodnaleziono kom\242rki pami\251ci o podanym adresie w %d lini!",line+1);
    logError(buffer);
    exit(1);
}

void mathOperation(char* to,char* from, enum DataSource secondSource,enum OpeartionType type,int line) {
    int valueA,valueB,addressA,addressB;

    to=trim(to);
    from=trim(from);

    if(isNumeric(to)) {
        short tmp=atoi(to);

        if(tmp<0||tmp>13) {
            char*buffer;

            sprintf(buffer,"Odwo\210anie do rejestru musi się odbywa\206 poprzez liczb\251 od 0 do 13 %d lini!",line);
            logError(buffer);
            exit(1);
        } else {
            addressA=tmp;
            valueA=registers[addressA].value;
        }
    } else {
        char*buffer;

        sprintf(buffer,"Odwo\210anie do rejestru musi się odbywa\206 poprzez liczb\251 od 0 do 13 %d lini!",line);
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

                sprintf(buffer,"Odwo\210anie do rejestru musi się odbywa\206 poprzez liczb\251 od 0 do 13 %d lini!",line);
                logError(buffer);
                exit(1);
            } else {
                addressB=tmp;
            }
        } else {
            char*buffer;

            sprintf(buffer,"Odwo\210anie do rejestru musi się odbywa\206 poprzez liczb\251 od 0 do 13 %d lini!",line);
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
        case COMPARE:   
        registers[addressA].value=valueA-valueB;
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


    to=trim(to);
    from=trim(from);
    if(isNumeric(to)) {
        short tmp=atoi(to);

        if(tmp<0||tmp>13) {
            char*buffer;

            sprintf(buffer,"Odwo\210anie do rejestru musi się odbywa\206 poprzez liczb\251 od 0 do 13 %d lini!",line);
            logError(buffer);
            exit(1);
        } else {
            addressA=tmp;
        }
    } else {
        char*buffer;

        sprintf(buffer,"Odwo\210anie do rejestru musi się odbywa\206 poprzez liczb\251 od 0 do 13 %d lini!",line);
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

                            sprintf(buffer,"Odwo\210anie do rejestru musi się odbywa\206 poprzez liczb\251 od 0 do 13 %d lini!",line);
                            logError(buffer);
                            exit(1);
                        } else {
                            addressB=tmp;
                        }
                    } else {
                        char*buffer;

                        sprintf(buffer,"Odwo\210anie do rejestru musi się odbywa\206 poprzez liczb\251 od 0 do 13 %d lini!",line);
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

    args=str_split(strdup(line),' ',3);

    if(checkIfKeyWord(*args.array)) {
        args=str_split(line,' ',2);
    }

    if(args.length>3) {
        char*buffer;

        sprintf(buffer,"B\210\245d sk\210adni w %d lini!",index);
        logError(buffer);
        exit(1);
    }
    if(args.length==1) {
        struct Order* order=malloc(sizeof(struct Order));
        order->tag=trim(*args.array);
        order->command=malloc(sizeof(char));
        order->command="";
        addJumpTag(trim(*args.array),index);
        return order;
    }

    
    if(args.length==3) {
        struct Order* order=malloc(sizeof(struct Order));
        order->tag=*args.array;
        order->command=*(args.array+1);
        order->args=*(args.array+2);

        if(strcmp(order->command,"DS")==0)
        {   
            struct Tag tag;
            struct CharArray ar=str_split(strdup(trim(order->args)),'*',2);

            if(inputsList.length%10==0&&inputsList.length!=0)
            {
                inputsList.tags=realloc(inputsList.tags,sizeof(struct Tag)*(inputsList.length+10));
            }
            tag.tag=order->tag;
            tag.target=0;
            if(ar.length==1)
                tag.array_len=1;
            else
                tag.array_len=atoi(trim(*ar.array));
            
            *(inputsList.tags+inputsList.length)=tag;
            inputsList.length++;
        }

        if(strcmp(order->command,"DC")!=0&&strcmp(order->command,"DS")!=0)
            addJumpTag(order->tag,index);

        return order;
    } else if(checkIfKeyWord(*args.array)) {
        if(args.length==2) {
            struct Order* order=malloc(sizeof(struct Order));
            order->tag="";
            order->command=trim(*args.array);
            order->args=trim(*(args.array+1));
            return order;
        } else {
            char*buffer;

             sprintf(buffer,"B\210\245d sk\210adni w %d lini!",index);
            logError(buffer);
            exit(1);
        }

    } else {
        char*buffer;
         sprintf(buffer,"B\210\245d sk\210adni w %d lini!",index);
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

    resultList=malloc(sizeof(struct Order*)*100);
    //open script
    fptr = fopen(fileName, "r");
    if (fptr == NULL)
    {
        logError("Nie można otworzyć wskazanego pliku");
        exit(1);
    }

    //read script line by line
    while ((read = getline(&line, &len, fptr)) != -1) {

        if(line[strlen(line)-1]=='\n')
            line[strlen(line)-1] = 0;

        if(trim(line)[0]=='/'&&trim(line)[1]=='/')
            continue;

        if(resultLength%100==0&&resultList!=0) {
            resultList=realloc(resultList,(resultLength+100)*sizeof(struct Order *));
        }

        if(resultLength==0) {
            if(read>2) {
                resultLength=1;
                resultList[0]=parseOrder(line,resultLength-1);
            }
        } else {
            if(read>2) {
                resultLength++;
                resultList[resultLength-1]=parseOrder(line,resultLength-1);
            }
        }
    }
    
    fclose(fptr);    

    list.length=resultLength;
    list.orders=resultList;

    return list;
}

void initMemory() {
     //alocate memory for address linkers
    jumpTags.tags=malloc(sizeof(struct Tag)*10);
    memoryTags.tags=malloc(sizeof(struct Tag)*10);
    inputsList.tags =malloc(sizeof(struct Tag)*10);
}

size_t executeOrder(struct Order* order,int line) {
    struct CharArray arguments=str_split(strdup(order->args),',',100);

    if(strcmp(order->command,"")==0)
        return line;


    //memory allocation
    if(strcmp(order->command,"DC")==0||strcmp(order->command,"DS")==0) {
        struct CharArray mul=str_split(*(arguments.array),'*',3);
        size_t up;
        bool hasTag=false;
        struct Tag t;
        struct CharArray ar;
        char * dType;
        short size;
        long valueInt;


        if(checkIfKeyWord(order->tag)) {
            char*buffer;

            sprintf(buffer,"Z/245a nazwa zmiennej w %d lini!",line);
            logError(buffer);
            exit(1); 
        }

        //check if array
        if(mul.length==1) {
            ar=str_split(trim(*mul.array),'(',3);
            up=1;
        } else {
            ar=str_split(trim(*(mul.array+1)),'(',3);
            up=_atoi64(trim(*(mul.array)));
        }

        if(up<1) {
            char*buffer;

            sprintf(buffer,"Nie mo\276na utworzy\206 tablicy o podanym rozmiarze w %d lini!",line);
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

            sprintf(buffer,"Nieobs\210ugiwany typ danych w %d lini!",line);
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

            sprintf(buffer,"B\210\245d sk\210adni w %d lini!",line);
            logError(buffer);
            exit(1);
        }


        if(strcmp(order->command,"DS")==0) {
            for(size_t i=0;i<inputsList.length;i++) {
                t=*(inputsList.tags+i);
                if(strcmp(t.tag,order->tag)==0) {
                    if(t.array_len==1) {
                        valueInt=t.target;
                    } else if(t.target!=-1) {
                        hasTag=true;
                    }
                    break;
                }
            }
        }

        if(hasTag) {
            struct CharArray ar=str_split(trim(t.add),',',1000);
            //if this field has been completed by user
            if(ar.length!=t.array_len)
            {
                char*buffer;

                sprintf(buffer,"Dana tablica ma z\210y wymiar w %d lini!",0);
                logError(buffer);
                exit(1);
            }

            for(size_t i=0;i<up;i++) {
                size_t a=allocateMemory(order->tag,valueInt,size,i==0,up);
                setMemoryValue(a,atoi(trim(*(ar.array+i))),line);
            }
        } else {
            //alocate single/array cell
            for(size_t i=0;i<up;i++) {
                allocateMemory(order->tag,valueInt,size,i==0,up);
            }
        }
    //math instructions
    } else if(strcmp(order->command,"A")==0) {
        mathOperation(*(arguments.array),*(arguments.array+1),MEMORY,ADD,line);
    } else if(strcmp(order->command,"AR")==0) {
        mathOperation(*(arguments.array),*(arguments.array+1),REGISTER,ADD,line);
    } else if(strcmp(order->command,"S")==0) {
        mathOperation(*(arguments.array),*(arguments.array+1),MEMORY,SUBSTRACT,line);
    } else if(strcmp(order->command,"SR")==0) {
        mathOperation(*(arguments.array),*(arguments.array+1),REGISTER,SUBSTRACT,line);
    } else if(strcmp(order->command,"M")==0) {
        mathOperation(*(arguments.array),*(arguments.array+1),MEMORY,MULTIPLY,line);
    } else if(strcmp(order->command,"MR")==0) {
        mathOperation(*(arguments.array),*(arguments.array+1),REGISTER,MULTIPLY,line);
    } else if(strcmp(order->command,"D")==0) {
        mathOperation(*(arguments.array),*(arguments.array+1),MEMORY,DIVIDE,line);
    } else if(strcmp(order->command,"DR")==0) {
        mathOperation(*(arguments.array),*(arguments.array+1),REGISTER,MULTIPLY,line);
    } else if(strcmp(order->command,"C")==0) {
        mathOperation(*(arguments.array),*(arguments.array+1),MEMORY,COMPARE,line);
    } else if(strcmp(order->command,"CR")==0) {
        mathOperation(*(arguments.array),*(arguments.array+1),REGISTER,COMPARE,line);
    } 
    //data tranfer
    else if(strcmp(order->command,"L")==0) {
        transferData(*(arguments.array),*(arguments.array+1),MtoR,line);
    } else if(strcmp(order->command,"LR")==0) {
        transferData(*(arguments.array),*(arguments.array+1),RtoR,line);
    } else if(strcmp(order->command,"ST")==0) {
        transferData(*(arguments.array),*(arguments.array+1),RtoM,line);
    } else if(strcmp(order->command,"LA")==0) {
        transferData(*(arguments.array),*(arguments.array+1),AtoR,line);
    }
    //jumps
    else if(strcmp(order->command,"J")==0) {
        return(getJumpTarget(*(arguments.array),line)-1);
    } else if(strcmp(order->command,"JN")==0) {
        if(stateRegisterValue==2) return getJumpTarget(*(arguments.array),line)-1;
    } else if(strcmp(order->command,"JP")==0) {
        if(stateRegisterValue==1) return getJumpTarget(*(arguments.array),line)-1;
    } else if(strcmp(order->command,"JZ")==0) {
        if(stateRegisterValue==0) return getJumpTarget(*(arguments.array),line)-1;
    }



    return line;
}

void executeScript(struct OrderList orders){
    for(size_t i=0;i<orders.length;i++) {
        struct Order* order=*(orders.orders+i);

        if(strcmp(order->command,"")!=0)
            i=executeOrder(order,i);
    }
}